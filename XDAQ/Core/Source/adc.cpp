//-------------------------------------------------------------------
//  adc.cpp   --   miniDAX: DAQ for XENON
// 
//  the adc library contains functions to handle the CAEN v1724 fADCs
//
//  29.11.2007 MS  (09.04.08)
//  -----------------
//  26.08.2008 MS	miniDAX version
//  30.05.2015 JW A lot of changes in order to make everything more easy
//-------------------------------------------------------------------

extern "C" {
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <sys/stat.h>
#include <unistd.h>
}

#include <iostream>

#include "global.h"
#include "CAENVMElib.h"
#include "control.h"

#include <TH1D.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TFile.h>
#include <TLatex.h>
 #define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"
//-------------------------------------------------------------------
// performs a single VME write
//   returns -1 on failure, 0 on success
int adc_writereg(crate& crt, 		// crate 
		digitizer& adc,		// adc
		int i,			// the module to set
		u_int32_t addr,		// the register to write to
		u_int32_t data)		// the value to set
{
  char msg[100];

  if (CAENVME_WriteCycle(crt.handle[adc.Crate[i]], adc.Address[i]+addr, 
		  	&data, cvA32_U_DATA, cvD32) != cvSuccess) {
    sprintf(msg,":::: ERROR: VME Write failure at address %08X ::::", (adc.Address[i] + addr));
    errormsg(0,msg);
    return -1;
  }  

  return 0;
}		

//-------------------------------------------------------------------
// performs a single VME read
//   returns -1 on failure, 0 on success
int adc_readreg(crate& crt, 		// crate 
		digitizer& adc,		// adc
		int i,			// the module to set
		u_int32_t addr,		// the register to write to
		u_int32_t& data)	// the value to read
{
   u_int32_t temp;
   char msg[100];
   
   if (CAENVME_ReadCycle(crt.handle[adc.Crate[i]], adc.Address[i]+addr, 
      		&temp, cvA32_U_DATA, cvD32) != cvSuccess) {
     sprintf(msg,":::: ERROR: VME Write failure at address %08X ::::", (adc.Address[i] + addr));
     errormsg(0,msg);
     return -1;		
   }		
   
   data=temp;
   return 0;
}		


//-------------------------------------------------------------------
// read the registers of the ADCs and set some variables
int adc_init(crate& crt, digitizer& adc, int& EnableInt,
	     int& EnableOLIrq, int& EnableBerr, int& EvAlign)
{
  u_int32_t data;
  int MemorySize, EnableVMEIrq, Align64;
  char BoardTypeName[4][6] = {"V1724", "V1721", "V1731", "V1720"};

  for (int i=0; i<adc.NumberOfADCs; i++) {    
    
  	if (adc.BoardInfo) 
    	printf("Board[%d], BoardIdentifier: %d, Crate: %d:\n",i,adc.Id[i],adc.Crate[i]);
    	
  	// Read Board Type and Memory Size
 	adc_readreg(crt,adc,i,BoardInfoReg,data);
	MemorySize=(int)((data >> 8) & 0xFF);
    	if (adc.BoardInfo) printf("  Board Type: %s;  Memory Size: %d MByte per channel\n",
	  				BoardTypeName[data & 0xFF],MemorySize);		

    	// Read Buffer Organization
 	adc_readreg(crt,adc,i,BlockOrganizationReg,data);
    	// Expected Event Size in Words (including Header)
	adc.ExpectedEvSize = (int)((((MemorySize*pow(2,20))/(int)pow(2,data))*8+16)/4);	   
    	adc.NbSamplesEvt =(MemorySize*(int)pow(2,20)) / ((int)pow(2,data)*2); 

    	// Read FrontPanel Input Data Settings (NIM/TTL)
 	adc_readreg(crt,adc,i,FrontPanelIOData,data);	
   	if (adc.BoardInfo) {
	  if (data & 1) printf("  Front Panel Input Signal: TTL\n"); 
     	           else printf("  Front Panel Input Signal: NIM\n"); 
	}	   

    	// Read Acquisition Control Register
	adc_readreg(crt,adc,i,ChannelConfigReg,data);	
    	// ZeroAlg: 0=Nothing, 1=Full Suppression, 2=ZLE, 3=On Threshold Suppress
	if (((data>>16) & 0xF)==2) adc.ZLE=1;
			      else adc.ZLE=0;
    	if (adc.BoardInfo) {
	  if (((data>>16) & 0xF)==2) printf("  Zero Length Encoding activated\n");
	              else printf("  NO Zero Length Encoding\n"); 
	}

    	// Read Monitor Configuration and DAC
	adc_readreg(crt,adc,i,MonitorModeReg,data);	
    	if (adc.BoardInfo) {
	  switch (data&0xFFF) {
	    case 0: printf("  Monitor: Trigger Majority Mode\n");
	      break;
	    case 1: printf("  Monitor: Test Mode\n");
	      break;
	    case 2: printf("  Monitor: Analog Monitor Mode\n");
	      break;
	    case 3: printf("  Monitor: Buffer Occupancy Mode\n");
	      break;
	    case 4: printf("  Monitor: Voltage Level Mode\n");
	      break;
	  } 
	}
  		  	
    	// Read VME control Register
	adc_readreg(crt,adc,i,VMEControlReg,data);	  
    		EnableBerr = (data>>4) & 1;
		EnableOLIrq  = (data & 0x8);
		EnableVMEIrq = (data & 0x7);
	    	EnableInt =  EnableVMEIrq | EnableOLIrq;
    		Align64   = (data>>5) & 1;
    
    	if (adc.BoardInfo) {
	  if (EnableOLIrq)  printf("  OLINK Interrupt enabled.\n");
    	  if (EnableVMEIrq) printf("  VME Interrupt %d enabled.\n", EnableVMEIrq);
	  if (!EnableInt)   printf("  No interrupts enabled.\n");
	}  

    	// Read BLT Event Number Register
	adc_readreg(crt,adc,i,BltEvNumReg,data);	
    	EvAlign = data;

    	// Read Firmware Revisions
	adc_readreg(crt,adc,i,ChannelFWRevision,data);	
    	if (adc.BoardInfo) printf("  Firmware: Mezzanine: %d.%d (%x/%d/%d), ", 
		data>>8 & 0xFF, data & 0xFF, data>>16 & 0xFF, data>>24 & 0xF, 2000 + (data>>28 & 0xF));
	adc_readreg(crt,adc,i,MotherBoardFWRevision,data);	
    	if (adc.BoardInfo) printf("Mother Board: %d.%d (%x/%d/%d)\n", 
		data>>8 & 0xFF, data & 0xFF, data>>16 & 0xFF, data>>24 & 0xF, 2000 + (data>>28 & 0xF));

    	// Read PostTrigger Setting
	if (adc.BoardInfo){
	  adc_readreg(crt,adc,i,PostTriggerReg,data);	
    	  printf("  PostTrigger Setting: %d samples = %.2f us\n", (int)data*2,data*0.02);
	}

	// Read DAC Status
	if (adc.BoardInfo){
	  adc_readreg(crt,adc,i,0x1788,data);
	  std::cout << std::hex << data << std::endl;
	  if (data&4) printf("  DAC Status: busy\n");
	         else printf("  DAC Status: ok\n");
	}
					
  } // end: for loop to initialize boards
  
  return 0;
}

//-------------------------------------------------------------------
// enable the ADCs
int adc_enable(crate& crt, digitizer& adc)
{
  u_int32_t data;
    
  // (read status of AcqControlReg first and then enable bit 0x4=100)
  for (int i=0; i<adc.NumberOfADCs; i++) {
    adc_readreg(crt,adc,i,AcquisitionControlReg,data);
    data |= 0x4;
    adc_writereg(crt,adc,i,AcquisitionControlReg,data);  
  }  

  return 0;
}


//-------------------------------------------------------------------
// disable the ADCs
int adc_disable(crate& crt, digitizer& adc)
{
  u_int32_t data;
    
  // (read status of AcqControlReg first and then disable bit 0x4=100)
  for (int i=0; i<adc.NumberOfADCs; i++) {
    adc_readreg(crt,adc,i,AcquisitionControlReg,data);
    data &= 0xFFFFFFFB;
    adc_writereg(crt,adc,i,AcquisitionControlReg,data);  
  }  

  return 0;
}


//-------------------------------------------------------------------
// make MBLT from one ADC module
int adc_mblt(crate& crt, digitizer& adc, int i, u_int32_t *buff)
{
  int blt_bytes=0, nb, ret;
  
  do { 
//    ret = CAENVME_FIFOBLTReadCycle(crt.handle[adc.Crate[i]], adc.Address[i], 
//		((unsigned char*)buff)+blt_bytes, adc.BltSize, cvA32_U_MBLT, cvD64, &nb);
    ret = CAENVME_FIFOBLTReadCycle(crt.handle[adc.Crate[i]], adc.Address[i], 
		((unsigned char*)buff)+blt_bytes, adc.BltSize, cvA32_U_BLT, cvD32, &nb);
    if ((ret != cvSuccess) && (ret != cvBusError)) {
      errormsg(1,(char*)":::: ERROR: CAENVME_FIFOBLTReadCycle Error ::::");
      col((char*)"red"); 
	  printf("     [Module %d, return value %d]\n",i,ret); 
	  col((char*)"blue"); // DM changed black to blue
//      return -1;     
      printf("%d bytes read\n",nb);
    }
    blt_bytes += nb;
    if (blt_bytes > adc.BufferSize) {
      errormsg(1,(char*)":::: ERROR: BUFFER_SIZE too small! Please increase it and run again. ::::");
      return -1;  
    }
  } while (ret != cvBusError); 
  
  return blt_bytes;
}


//-------------------------------------------------------------------
// determine baseline (in baseline-mode):
//   Board must be configured in non ZLE mode
int adc_getbaseline(crate& crt,	digitizer& adc)
{

  int blt_bytes,pnt,Size,cnt; 
  int maxval, minval, val;
  int maxIteration=50;		// number of iterations to adjust baseline
  double mean;  
  u_int32_t *base;
  float diff;
  float newDAC;
  int okFlag[adc.NumberOfADCs][8];	// array to store already finished boards
  double meanVal[adc.NumberOfADCs][8];	// mean value of channel
  int stillToDo=0;			// number of Boards to be optimized
  char msg[100];
  float MaxDev=2.0;			// maximal accepted deviation of baseline
  int samples=8192;    			// lenght of waveforms in samples
 
  for (int i=0; i<adc.NumberOfADCs; i++) 
    for (int j=0; j<8; j++)  {
      meanVal[i][j]=0;
      okFlag[i][j]=0;
      stillToDo++;
    }  
  
  // allocate memory for buffer
  if ( (base = (u_int32_t *)malloc(adc.BufferSize)) == NULL) {  
    sprintf(msg,":::: ERROR: Can't allocate memory buffer of %d kB ::::", adc.BufferSize/1024);
    errormsg(0,msg);
    return -1;
  }
  
  col("green"); 
  printf("\nBaseline determination started...\n  Set baseline to %d\n",adc.baseline); 
  printf(RESET);

  int iteration=0;
  while (stillToDo>0 && iteration<=maxIteration) {  
	pnt=0;
    iteration++; 
    printf("  Iteration %d: \n",iteration);
    printf("  still %d channels to adjust...\n",stillToDo);
   // if (iteration==(int)(maxIteration*0.66)) MaxDev=MaxDev*2.5;	    

    // here starts the part to determine the baseline ----------------------
    usleep(50000);

    // enable adcs to get event
    adc_enable(crt,adc);
    usleep(1000);

    // internal trigger
    u_int32_t data = 0x1;
    for (int i=0; i<adc.NumberOfADCs; i++) adc_writereg(crt,adc,i,SoftwareTrigger,data);  

    // disable adcs 
    adc_disable(crt,adc);		  

    // read the event
    for (int i=0; i<adc.NumberOfADCs; i++) { // loop over all boards
            
      // Read data from module i in MBLT mode into buff
      blt_bytes=adc_mblt(crt,adc,i,base);
      if (blt_bytes<0) return -1;
	 // error handling if there is an invalid entry after an event
    if (base[0]==0xFFFFFFFF) pnt++;
    
    // check header
    if ((base[pnt]>>20)==0xA00 && (base[pnt+1]>>28)==0x0) {
      Size=((base[pnt]&0xFFFFFFF)-4);                   // size of full waveform (all channels)
      pnt++;
 
      int ChannelMask=base[pnt] & 0xFF;                 pnt++;    
    
      // Get size of one waveform by dividing through the number of channels
      cnt=0;
      for (int j=0; j<8; j++) if ((ChannelMask>>j)&1) cnt++;
      Size=Size/cnt;
      pnt+=2; // ignore EventConter and TTT
      
      
	    	  	  
      // analyze Event and calculate Baseline
//      if (Size==8192) {
  //     
 //     } 
//	  else{
//	       col("red"); printf(":::: ERROR: module %d\n",i);  printf(RESET);
  //         errormsg(0,":::: ERROR: wrong Buffer Organization setting (expect 8192 samples) ::::");
//	       return -1;
//	  }
    
      pnt=4;
      for (int channel=0; channel<8; channel++) {
        minval=16384; maxval=0; mean=0.;
        for (int j=0; j<(int)(samples/2); j++) {
          for (int k=0; k<2; k++) {
            if (k==0) val=base[pnt+j]&0xFFFF;
	         else val=(base[pnt+j]>>16)&0xFFFF;
            if (val>maxval) maxval=val;
  	    if (val<minval) minval=val;
	    mean+=(double)val;
          }
        } 
        pnt=pnt+(int)(samples/2);		    
        mean=mean/(float)samples;	// baseline mean value


	if (okFlag[i][channel]) continue;
	
        // Read current DAC setting
	adc_readreg(crt,adc,i,DACRegister+(channel*0x100),data);
				
        if ((maxval-mean)<500 && (mean-minval)<500) {  // flat baseline?
	  diff=(float)adc.baseline-mean;	 // calculate difference to wanted value

	  if (diff<=MaxDev && diff>=MaxDev*(-1.)) { // nothing to do
	    okFlag[i][channel]=data;
	    stillToDo--;	    
	  } else { // adjust baseline	    
	    if (diff>MaxDev) { 
 	      if (diff>8) {                  
		  if (diff>50) newDAC=(data+diff/(-0.0264)); 
	 	  	else newDAC=data-30;
	      } else newDAC=data-15;
	    }  
	    else if (diff<MaxDev*(-1.)) {
	      if (diff<-8) {
		if (diff<-50) newDAC=(data+diff/(+0.0264)); 
                         else newDAC=data+30;
	      } else newDAC=data+15;
	    }  
	    else newDAC=data;	    
	  }  
           
	  // printf intermediate step for remaining channels
	  if (!okFlag[i][channel]) {
            printf(KMAG);
	    printf("  ToDo: Baseline %3d.%d: %5.1f, DAC = 0x%04X, diff = %3.1f\n",
 	    	adc.Id[i],channel,mean,(int)data,diff); 
	    printf(RESET);
          }
 
	  // print result if channel was successful
	  if (okFlag[i][channel] || iteration==maxIteration) {
            col("green");
	    printf("Baseline %3d.%d: %5.1f, DAC = 0x%04X, diff = %3.1f\n",
 	    	adc.Id[i],channel,mean,(int)data,diff); 
	    printf(RESET);
	    if (iteration==maxIteration) {
	      okFlag[i][channel]=data;  // store all DAC settings
	      stillToDo--;
	    }  
	    continue;
	  }  
        } else { // probably not only baseline but a signal
		 // do nothing, wait for next iteration
 	  printf("  %d.%d: signal in baseline? %f, %f\n",adc.Id[i],channel,maxval-mean, minval-mean);
        }        
        
	// Set new DAC setting
	data=(int)newDAC;
	std::cout << std::dec <<  data << std::endl;
        adc_writereg(crt,adc,i,DACRegister+(channel*0x100),data);  
      
      } // end: for (channel)       
      } // end Check Header 
    else return -1;    
    } // end: for loop over boards	
  } // end iteration
    	  
  free(base);
  
  // print DAC settings to file ----------------------------------------
  
  // create filename for old ini-file  
  char timestr[100], cmnd[300];
  control_gettimestring(timestr,'f');
  FILE *dacfile;
  dacfile=fopen("miniDAXBaseline.ini","w");
  if (dacfile==NULL) {
    errormsg(0,(char*)":::: ERROR: cannot open file to write baseline settings ::::");
    return -1;
  }
  
  fprintf(dacfile,"#***************************************************************\n");
  fprintf(dacfile,"# ZDAQ --  DAQ for UZH XENON\n#\n"); //
  fprintf(dacfile,"# This is the baseline configuration file. The information\n");
  fprintf(dacfile,"# here sets all ADC channels to the same baseline level.\n#\n");
  fprintf(dacfile,"# generated: %s\n",timestr);
  fprintf(dacfile,"#***************************************************************\n\n");
  
  for (int i=0; i<adc.NumberOfADCs; i++) 
    for (int j=0; j<8; j++)  {
      fprintf(dacfile,"WRITE_MODULE %d %4d %04X\n",adc.Id[i],1098+j*100,okFlag[i][j]);
    }  
    		  
  fclose(dacfile);
  printf("miniDAXBaseline.ini successfully written\n");
  
  return 0;
}	

//-------------------------------------------------------------------
// analyze baseline (mean, RMS, spikes)
int adc_analyzebaseline(crate& crt, digitizer& adc)	
{
  int blt_bytes; 
  int minval, val, pnt;
  int maxIteration=15;		// number of iterations to adjust baseline
  double mean;  
  u_int32_t *base;

  double meanVal[adc.NumberOfADCs][8];	// mean value of channel
  double RMSVal[adc.NumberOfADCs][8];	// mean RMS of channel
  double spikeVal[adc.NumberOfADCs][8];	// max spike of channel
  char filename[100];
  TH1D *hh = new TH1D("hh","hh",1000,15384,16384);

  char msg[100];
  int samples=8192;    			// lenght of waveforms in samples

  for (int i=0; i<adc.NumberOfADCs; i++) 
    for (int j=0; j<8; j++)  {
      meanVal[i][j]=0;
      RMSVal[i][j]=0;
      spikeVal[i][j]=0;
    }  
  
  // allocate memory for buffer
  if ( (base = (u_int32_t *)malloc(adc.BufferSize)) == NULL) {  
    sprintf(msg,":::: ERROR: Can't allocate memory buffer of %d kB ::::", adc.BufferSize/1024);
    errormsg(0,msg);
    return -1;
  }
  
  col((char*)"green"); 
  printf("\nBaseline analysis started...\n"); 
  col((char*)"blue"); // DM changed black to blue

  for (int iteration=0; iteration<maxIteration; iteration++) {
    printf("  *\n");
    // here starts the part to determine the baseline ----------------------
    usleep(200000);
    // enable adcs to get event
    adc_enable(crt,adc);

    // internal trigger
    u_int32_t data = 0x1;
    for (int i=0; i<adc.NumberOfADCs; i++) adc_writereg(crt,adc,i,SoftwareTrigger,data);  

    // disable adcs 
    adc_disable(crt,adc);		  
		  
    // read the event
    for (int i=0; i<adc.NumberOfADCs; i++) { // loop over all boards
//printf("Board %d\n",i);            
      // Read data from module i in MBLT mode into buff
      blt_bytes=adc_mblt(crt,adc,i,base);
      if (blt_bytes<0) { errormsg(0,(char*)":::: ERROR ::::"); return -1; }
//if (i==2) {
//  for (int k=0; k<100; k++) printf("%08X\n",base[k]);
//}	    	  	  
      // analyze Event and calculate Baseline
      if ((base[0]&0xFFFFFFF)!=0x8004) {
        col((char*)"red"); 
		printf(":::: Problem with module %d ::::\n",i); 
		col((char*)"black");
        errormsg(0,(char*)":::: ERROR: wrong Buffer Organization setting (expect 8192 samples) ::::");
        return -1;
      } 
    
      pnt=4;
      for (int channel=0; channel<8; channel++) {
        minval=16384; mean=0.;
	hh->Reset();
        for (int j=0; j<(int)(samples/2); j++) {
          for (int k=0; k<2; k++) {
            if (k==0) val=base[pnt+j]&0xFFFF;
	         else val=(base[pnt+j]>>16)&0xFFFF;
  	    if (val<minval) minval=val;
	    mean+=(double)val;
	    hh->Fill(val);
          }
        } 
        pnt=pnt+(int)(samples/2);		    
        meanVal[i][channel]+=mean/(double)samples;	// baseline mean value
	spikeVal[i][channel]+=(double)minval;		// maximal negative spike
	RMSVal[i][channel]+=hh->GetRMS();		// RMS value
//        if (i==15 && channel==5) printf("%f\n",hh->GetRMS());
      } // end: for (channel)          
    } // end: for loop over boards	
  } // end for(iteration)
    	  
  free(base);
  
  // print baseline analysis results to file ----------------------------------------
  
  // create filename for old ini-file  
  char timestr[200];
  control_gettimestring(timestr,'f');
  
  FILE *basefile;
  sprintf(filename,"noise_%s.dat",timestr);
  basefile=fopen(filename,"w");
  if (basefile==NULL) {
    errormsg(0,(char*)":::: ERROR: cannot open file to store baseline analysis results ::::");
    return -1;
  } 
  
  fprintf(basefile,"#***************************************************************\n");
  fprintf(basefile,"# ZDAQ --  DAQ for UZH XENON\n#\n");
  fprintf(basefile,"# This file contains baseline information in the format\n");
  fprintf(basefile,"#   PMT mean RMS spike\n#\n");
  fprintf(basefile,"# generated: %s\n",timestr);
  fprintf(basefile,"#***************************************************************\n\n");
  
  int m, c;
  for (int i=1; i<=adc.pmtmax; i++) { 
    control_getadc(adc,i,m,c);
    fprintf(basefile,"%3d %.1lf %.2lf %.1lf\n",i,meanVal[m][c]/(double)maxIteration,
			RMSVal[m][c]/(double)maxIteration,
			(meanVal[m][c]-spikeVal[m][c])/(double)maxIteration);
  }  
//  printf("%f\n",RMSVal[15][5]/(double)maxIteration);
    		  
  fclose(basefile);
  col((char*)"green"); 
  printf("Resuls stored to file %s\n",filename); 
  col((char*)"blue"); // DM changed black to blue
  
  return 0;
}

//-------------------------------------------------------------------
// Close connection to VME crate(s)
int adc_closecrates(crate& crt)
{
  char msg[100];

  if (crt.handle[0] != -1) {
  int ret;
    for (int crate=0; crate<crt.NumberOfCrates; crate++) {
      if ((ret=CAENVME_End(crt.handle[crate]))!=cvSuccess) {
        sprintf(msg,":::: ERROR: CAENVME_End crate %d failed; return code: %d ::::",crate,ret);        
	errormsg(0,msg);
      }	
    }
  }

  return 0;
}
		
		



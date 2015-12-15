//C++ Libraries
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <cmath>
#include <map>
#include <math.h>
#include <vector>
#include <sys/time.h>
#include <string>
#include <Riostream.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <string>
#include "ADCManager1720.h"
#include "global.h"

//Root Libaries
#include <TH1D.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TFile.h>
#include <TLatex.h>
#include "TTree.h"
#include "TFile.h"
#include "TROOT.h"


ADCManager1720::ADCManager1720()
{
	m_CrateHandle= m_ADCaddr=0;
	m_EnableVMEIrq=m_Align64=m_EnableBerr=m_EnableOLIrq=m_EnableInt=m_EvAlign=m_Frequency=m_Baseline=m_resDAC=m_resDAC=m_Voltage=m_nbCh=m_triggertyp=m_SoftwareRate=0;
	for(int i=0;i<8;i++){
		m_DACTarget[i]=14000;
	}
}

ADCManager1720::~ADCManager1720()
{
}

int ADCManager1720::Init(){

	printf(KYEL);
	printf("\nReset the ADC . . .\n");
	printf(RESET);

	//Reset the board first
    m_hex=0x1;
    adc_writereg(0xEF24,m_hex);
    sleep(3);

	//Set Register Setting from default file (only to give the user some freedom)
   	RegisterWriting(m_RegisterFileName);

	//Set Register/other Settings from xml-file
	ApplyXMLFile();

	//Read Settings
	RegisterReading();

	printf(KYEL);
	printf("\nCalibrate the ADC . . .\n\n");
	printf(RESET);

	
	return 0;
}

int ADCManager1720::Enable(){
	
		//Aquisition
    	// enable adcs to get event
		m_hex = 0x4;
    	adc_writereg(AcquisitionControlReg,m_hex); 
    	usleep(1000);
		return 0;
}

int ADCManager1720::Disable(){
	
		//Aquisition
    	// disable adcs to get event
		m_hex =0x00;
    	adc_writereg(AcquisitionControlReg,m_hex);
    	usleep(1000);
		return 0;
}





//-------------------------------------------------------------------
// Function to generate a Software Trigger
int ADCManager1720::ApplySoftwareTrigger(){

		    
		//internal trigger
		m_hex = 1;
		adc_writereg(SoftTriggerReg,m_hex);  
	    
        // Read data from module i in MBLT mode into buff
		blt_bytes = adc_readblt();
		std::cout  << blt_bytes << std::endl;		

	// error handling if there is an invalid entry after an event
    	if (blt_bytes<0) return -1;   
  
   return 0;
}


//-------------------------------------------------------------------
// Read BaseLine 
int ADCManager1720::ReadBaseLine(){
	RegisterWriting(m_BaselineFileName);
	sleep(3);
}


//-------------------------------------------------------------------
// Calculate BaseLine 
int ADCManager1720::CalculateBaseLine(){
	
	printf(KYEL);
	std::cout <<  "Calculate Baseline " << std::endl << std::endl;
	printf(KGRN);

	//Use only 5 Iterations for the Baseline Calculation
   	for(int i=0;i<4;i++){

	std::cout << std::endl << "	Iteration: " << i << std::endl << std::endl;
	this->Enable();
	this->ApplySoftwareTrigger();
	this->Disable();

    //Start from the first word
    pnt =0;

    //Check first Word
    if (buffer[0]==0xFFFFFFFF) pnt++;

    // check header
    if ((buffer[pnt]>>20)==0xA00 && (buffer[pnt+1]>>28)==0x0) {
        Size=((buffer[pnt]&0xFFFFFFF)-4);                   // size of full waveform (all channels)
        pnt++;
 
	//Read ChannelMask (Handbook)
        int ChannelMask=buffer[pnt] & 0xFF;                 

	pnt++;    
    
        // Get size of one waveform by dividing through the number of channels
        cnt=0;
        for (int j=0; j<8; j++) if ((ChannelMask>>j)&1) cnt++;
        Size=Size/cnt;

 	// ignore EventConter and TTT
        pnt+=2;
      	
        for (int j=0; j<8; j++) { // read all channels
		m_mean=0;
	
		// read only the channels given in ChannelMask
		if ((ChannelMask>>j)&1) CurrentChannel=j;
                else continue;
		
		if (CurrentChannel!=j) { pnt+=Size; continue; }
      		else pnt++;

		if (j>j) return 0;	
	      
		cnt=0;                              // counter of waveform data
		wavecnt=0;                          // counter to reconstruct times within waveform
      		while (cnt<(Size-(CORRECTION/2)))
      		{		
			m_mean+= (double)((buffer[pnt]&0xFFFF));
			m_mean+= (double)(((buffer[pnt]>>16)&0xFFFF));
          		pnt++; wavecnt+=2; cnt++;
      		} // end while(cnt...)
		m_mean=m_mean/wavecnt;
		//m_diff=m_mean-m_DACTarget[j];
		m_diff=m_mean-4000;
		//if(m_diff<3.0)continue;
		m_correction= ((2.0/pow(2.0,12))*m_diff)/((2.0)/pow(2,16));		//(mV of one Count ADC * difference)/mV of one Count of DAC
		adc_readreg(DACRegister+(j*0x100),m_hex);	
		m_hex=m_hex+m_correction;
		m_DACLevel[j]=m_hex;
		std::cout << "::::::: Channel: " << j << " :::::::" << std::endl ;
		std::cout << "	Mean: " << m_mean << " Target : " << m_Baseline<< " Diff: " <<  m_diff << " DAC: " << m_DACLevel[j] <<  std::endl << std::endl;
		adc_writereg(DACRegister+(j*0x100),m_hex);
		
     	} // end for-loop
    }
    else{
		printf(KRED);
    	std::cout << "	Error in Reading the Event" << std::endl;
    	printf(RESET);
	return -1;
    }
	sleep(3);
    }
    printf(RESET);
    
    // create filename for Baseline.ini
    FILE *dacfile;
    dacfile=fopen("DACBaseline.ini","w");
    if (dacfile==NULL) {
    	std::cout << ":::: ERROR: cannot open file to write baseline settings ::::" << std::endl;
    	return -1;
    }
  
  fprintf(dacfile,"#***************************************************************\n");
  fprintf(dacfile,"# JDAQ --  DAQ for UZH \n#\n"); //
  fprintf(dacfile,"# This is the baseline configuration file. The information\n");
  fprintf(dacfile,"# here sets the ADC channels to the user baseline level.\n#\n");
  fprintf(dacfile,"# generated: automatically\n");
  fprintf(dacfile,"#***************************************************************\n\n");
  
    for (int j=0; j<8; j++)  {
      fprintf(dacfile,"WRITE_REGISTER %4d %04X\n",1098+j*100,m_DACLevel[j]);
    }  
    		  
    fclose(dacfile);
    printf(KYEL);
    std::cout <<  "DACBaseline.ini successfully written" << std::endl << std::endl;
    printf(RESET);



    return ret; 
}

//Function to aquire data if there is some
int ADCManager1720::CheckEventBuffer(){
 
	// Interrupts: 
    // If enabled, wait for the interrupt request from the digitizer. In this mode,
    // there is no CPU time wasting because the process sleeps until there are at least
    // N event available for reading, where N is the content of the register Interrupt
    // Event Number (0xEF18)
     if (m_EnableInt) {		
	  	if (m_EnableOLIrq) IrqMask = 0x01; // IRQ1 is used when interrupt generated via OLINK
		      else IrqMask = 0xFF; // All VME Interrupt Enabled

        CAENVME_IRQEnable(m_CrateHandle, IrqMask); // Enable IRQs
	    ret = CAENVME_IRQWait(m_CrateHandle, IrqMask, 1000); // Wait for IRQ (max 1sec)
	   	//if (ret) continue; 

  	   	ret=CAENVME_IRQCheck(m_CrateHandle, &IrqCheckMask);                           
	   	CAENVME_IRQDisable(m_CrateHandle, IrqMask); // Disable IRQs	    	    	    
     }

	// Read data from ADC  in MBLT mode into buff[i]
     blt_bytes=adc_readblt();
	 if (blt_bytes<0) return -1;;

		return 0;
}


//-------------------------------------------------------------------
//    performs a single VME write
//   returns -1 on failure, 0 on success
int ADCManager1720::adc_writereg(u_int32_t addr,		    // the register to write to
			     u_int32_t data)		    // the value to set
{
if (CAENVME_WriteCycle(m_CrateHandle, m_ADCaddr+addr, &data, cvA32_U_DATA, cvD32) != cvSuccess) {
		printf(KRED);
		printf(":::: VME read error!!! (ADCManager::CAENVME_WriteCycle()) ::::\n");
		printf(RESET);
		return -1;
  }  
  return 0;
}		


//-------------------------------------------------------------------
//    performs a single VME read
//   returns -1 on failure, 0 on success
int ADCManager1720::adc_readreg(u_int32_t addr,		// the register to write to
			    u_int32_t& data)		// the value to read
{
   u_int32_t temp;
   
   if (CAENVME_ReadCycle(m_CrateHandle, m_ADCaddr+addr, &temp, cvA32_U_DATA, cvD32) != cvSuccess) {
		printf(KRED);
		printf(":::: VME read error!!! (ADCManager::CAENVME_ReadCycle()) ::::\n");
		printf(RESET);
		return -1;	
   }		
   
   data=temp;
   return 0;
}	
//-------------------------------------------------------------------

//-------------------------------------------------------------------
//    performs a single VME read
//   returns -1 on failure, 0 on success
int ADCManager1720::adc_readblt()		// the value to read
{
	// read the event
       int nb, ret;     
      // Read data from module i in MBLT mode into buff     
	blt_bytes=0;
   do { 
    ret = CAENVME_FIFOBLTReadCycle(m_CrateHandle, m_ADCaddr, 
		((unsigned char*)buffer)+blt_bytes, 524288, cvA32_U_BLT, cvD32, &nb);
    if ((ret != cvSuccess) && (ret != cvBusError)) {
      std::cout << "Block read error" << std::endl;   
      printf("%d bytes read\n",nb);
      return -1;  
    }
    blt_bytes += nb;
    if (blt_bytes > m_BufferSize) { 
		      std::cout << "negativ transfered" << std::endl;
      return -1;  
    }
  } while (ret != cvBusError); 
  
   return  blt_bytes;
}	

//-------------------------------------------------------------------
//Set Address of the ADC
void ADCManager1720::SetADCAddress(string address){
	m_ADCaddr=StringToHex(address);
	return;
}

//-------------------------------------------------------------------
//Convert String to Hex
u_int32_t ADCManager1720::StringToHex(const string &str){
   stringstream ss(str);
   u_int32_t result;
   return ss >> std::hex >> result ? result : 0;
}

//-------------------------------------------------------------------
// Apply the Settings from the RegisterConfigFile to the ADC
int ADCManager1720::RegisterWriting(string configfilename){
  FILE *f_ini;
  char str[100];
  if( (f_ini = fopen(configfilename.c_str(), "r")) == NULL ) {
	  printf(KRED);
      	  sprintf(str,"Can't open Configuration File %s ::::", configfilename.c_str());
	  printf(RESET);
	  return -1;
  }
  else{ printf(KYEL);
	printf("\nReading Configuration File %s\n", configfilename.c_str());
	std::cout << std::endl;
	printf(RESET);
  }
  
  while(!feof(f_ini)) {
      fscanf(f_ini, "%s", str);
      if (str[0] == '#')
          fgets(str, 1000, f_ini);
      else{
	  // Generic VME Write
          if (strstr(str, "WRITE_REGISTER")!=NULL) {
              fscanf(f_ini, "%x", (int *)&m_addr);
              fscanf(f_ini, "%x", (int *)&m_hex);
	      printf(KGRN);
              printf("	Address: %x Data: %x",m_addr, m_hex);
              printf(RESET);
	      std::cout << std::endl;
              if (adc_writereg(m_addr,m_hex)<0){return -1;} 
          }
      }
   }
   std::cout << std::endl;
   return 0;
}
//-------------------------------------------------------------------
// Reading some Register Settings from the ADC
int ADCManager1720::RegisterReading(){
	printf(KYEL);
	printf("Reading ADC Configurations\n\n");
	printf(RESET);

	printf(KGRN);
	u_int32_t data;

	// Read Board Type and Memory Size
 	adc_readreg(0x8140,data);
	m_MemorySize=(int)((data >> 8) & 0xFF);
	printf("	Board Type: %s;  Memory Size: %d MByte per channel\n", "v1730D",m_MemorySize);

    	// Read Firmware Revisions
	adc_readreg(ChannelFWRevision,data);	
    	printf("	Firmware: Mezzanine: %d.%d (%x/%d/%d), ", data>>8 & 0xFF, data & 0xFF, data>>16 & 0xFF, data>>24 & 0xF, 2000 + (data>>28 & 0xF));

	adc_readreg(MotherBoardFWRevision,data);	
    	printf("	Mother Board: %d.%d (%x/%d/%d)\n", data>>8 & 0xFF, data & 0xFF, data>>16 & 0xFF, data>>24 & 0xF, 2000 + (data>>28 & 0xF));

	adc_readreg(BlockOrganizationReg,data);
	 printf("	Block Organization: %i\n",data);

    	// Expected Event Size in Words (32Bit including Header)
	m_ExpectedEvSize = (int)((((m_MemorySize*pow(2,20))/(int)pow(2,data))*8+16)/4);			//From the Handbook  
  	m_BufferSize = (m_EvAlign&&m_EnableBerr) ? (m_ExpectedEvSize* m_EvAlign*4):(m_ExpectedEvSize*4);
   	m_BufferSize += 65536;

        // allocate memory for buffer
    	if ( (buffer = (u_int32_t*)malloc(m_BufferSize)) == NULL) {  
		printf(KRED);
		printf(":::: ERROR: Can't allocate memory buffer of %d kB ::::", m_BufferSize/1024);
		printf(RESET);
		return -1;
  	}

	// Read DAC Status
	for(int i=0;i<8;i++){
		adc_readreg( 0x1088+(i*0x100), data);
       	        if (data&4) printf("	Channel %i DAC Status: busy\n",i);
       		else printf("	Channel %i DAC Status: ok\n",i);
	}

	//Read Channel Enable Mask
	data=0xFF;
	adc_readreg(0x8120,data);
	m_hex=0;
	for(int i=0; i<8;i++){
		adc_readreg(ChannelTreshold+(i*0x0100),m_hex);
		std::cout << "	Channel: " << i << "	" << ((data >> i) & 0x01) << " Treshold: " << std::dec << m_hex << std::endl;
	}
	
    	// Read BLT Event Number Register
	adc_readreg(0xEF1C,data);	
    	m_EvAlign = data;
	
	//Read VME Control Register
	adc_readreg(0xEF00,data);	  
    m_EnableBerr = (data>>4) & 1;
	m_EnableOLIrq  = (data & 0x8);
	m_EnableVMEIrq = (data & 0x7);
	m_EnableInt =  m_EnableVMEIrq | m_EnableOLIrq;
    	m_Align64   = (data>>5) & 1;

          if (m_EnableOLIrq)  printf("	OLINK Interrupt enabled.\n");
    	  if (m_EnableVMEIrq) printf("	VME Interrupt %d enabled.\n", m_EnableVMEIrq);
	  if (!m_EnableInt)   printf(" 	No interrupts enabled.\n");

    	// Read Monitor Configuration and DAC
	adc_readreg(MonitorModeReg,data);	
	  switch (data&0xFFF) {
	    case 0: printf("	Monitor: Trigger Majority Mode\n");
	      break;
	    case 1: printf("	Monitor: Test Mode\n");
	      break;
	    case 2: printf("	Monitor: Analog Monitor Mode\n");
	      break;
	    case 3: printf("	Monitor: Buffer Occupancy Mode\n");
	      break;
	    case 4: printf("	Monitor: Voltage Level Mode\n");
	      break;
	  } 

    	// Read FrontPanel Input Data Settings (NIM/TTL)
 	adc_readreg(FrontPanelIOData,data);	
	 if (data & 1) printf("	Front Panel Input Signal: TTL\n"); 
     	           else printf("	Front Panel Input Signal: NIM\n"); 

	//Read customsize window
	adc_readreg(CustomWindow,data);
	printf("	Customsize window: %d\n", (int)data*10);
	m_length=(int)data*10-CORRECTION;								//Correction of the Event (last 10 Samples are broken

	//read Posttrigger Settings
	adc_readreg(PostTriggerReg,data);	
    printf("	PostTrigger Setting: %d samples = %.2f us\n", (int)(data)*4,(4*data)*0.002);

	printf(RESET);
	
	if (!m_EnableBerr) {  // BERR must be enabled!
      		std::cout << ":::: ERROR: BERR must be enabled to run the program! ::::" << std::endl;
		return -1;
    }

	return 0;
}

int ADCManager1720::ApplyXMLFile(){
	int temp;  
	char txt[100];
	const char *xstr;
	txt[0]='\0';
	
	// open the XML file -------------------------------------------------------
	XMLNode xMainNode=XMLNode::openFileHelper(m_XmlFileName,"settings");
	// parse global DAQ settings -----------------------------------------------
	XMLNode xNode=xMainNode.getChildNode("global");
	// parse global ADC settings -----------------------------------------------
	xNode=xMainNode.getChildNode("adc").getChildNode("global");
	
	xstr=xNode.getChildNode("nb_chs").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		m_nbCh=(int)(atoi(txt));
	} else error((char*)"XML-nb_chs");


	
	return 0;
}




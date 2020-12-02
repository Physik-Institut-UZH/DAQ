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
#include "ADCManager.h"
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
/*
Author: Julien Wulf UZH
*/


ADCManager::ADCManager()
{
	m_handle= m_ADCaddr=0;
	m_EnableVMEIrq=m_Align64=m_EnableBerr=m_EnableOLIrq=m_EnableInt=m_EvAlign=m_Frequency=m_Baseline=m_resDAC=m_resDAC=m_Voltage=m_nbCh=m_triggertyp=m_SoftwareRate=m_module=0;
	for(int i=0;i<8;i++){
		m_DACTarget[i]=0;
		m_DACLevel[i]=0;
		m_DACFinished[i]=0;
	}
}

ADCManager::~ADCManager()
{
}

//-------------------------------------------------------------------
//Enable the ADC 
int ADCManager::Enable(){
	
	//Aquisition
  // enable adcs to get event
  m_hex = 0x4;
  adc_writereg(AcquisitionControlReg,m_hex); 
  usleep(1000);
  return 0;
}

//-------------------------------------------------------------------
//Disable the ADC
int ADCManager::Disable(){
	
  //Aquisition
  // disable adcs to get event
  m_hex =0x00;
  adc_writereg(AcquisitionControlReg,m_hex);
  usleep(1000);
  return 0;
}


//-------------------------------------------------------------------
// Function to generate a Software Trigger + readout
int ADCManager::ApplySoftwareTrigger(){

  //internal trigger
  m_hex = 1;
  adc_writereg(SoftTriggerReg,m_hex);  

  // Read data from module i in MBLT mode into buff
  blt_bytes = adc_readblt();

  // error handling if there is an invalid entry after an event
  if (blt_bytes<0) return -1;   

  return 0;
}

int ADCManager::SoftwareTrigger(){
	
	//internal trigger
	m_hex = 1;
	adc_writereg(SoftTriggerReg,m_hex);  
	return 0;
}


//-------------------------------------------------------------------
// Read BaseLine 
int ADCManager::ReadBaseLine(){
	RegisterWriting(m_BaselineFileName);
	sleep(3);
}


/* Special Daisy Chain options */



//Syncronize Boards (v1730D)
int ADCManager::SyncBoards(){
	adc_writereg(m_ADCaddr+ClockSyncReg,1);
	sleep(1);
}

//Delay between boards (v1730D)
int ADCManager::DelayBoards(u_int32_t data){
	adc_writereg(m_ADCaddr+DelayReg,data);
	sleep(1);
}


//Board Position
int ADCManager::MCSTBoards(u_int32_t data){
	adc_writereg(m_ADCaddr+MCSTBaseControlReg,data);
	sleep(1);
}

int ADCManager::EnableChannel(int channel){

	m_hex=0;
	m_hex=m_hex+pow(2,channel);	//activate channel 0 for trigger

	//Software + Channel Trigger 
	m_hex=m_hex+pow(2,31);
	adc_writereg(FrontPanelTriggerOutReg,m_hex);
	adc_writereg(TriggerSourceMaskReg,m_hex);

	return 0;
}

int ADCManager::EnableSoftware(){

	//Software + Channel Trigger 
	m_hex=m_hex+pow(2,31);
	adc_writereg(FrontPanelTriggerOutReg,m_hex);
	adc_writereg(TriggerSourceMaskReg,m_hex);

	return 0;
}


/* END */



//-------------------------------------------------------------------
// Calculate BaseLine 
int ADCManager::CalculateBaseLine(){
	
	printf(KYEL);
	std::cout <<  "Calculate Baseline of Module: " <<  m_module << std::endl << std::endl;
	printf(KGRN);

	//Disable Triggerout independet baseline calculation 
	adc_writereg(FrontPanelTriggerOutReg,0);

	//Several Baseline iterations
   	for(int i=0;i<m_iteration;i++){
		//Check if it converged 
		int n_channels=0;		

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
      int temp = buffer[pnt] & 0xFF0000;

      cout<<"Channel Mask in Baseline calculator "<<ChannelMask<<", "<<temp<<endl;

			pnt++;    
		
			// Get size of one waveform by dividing through the number of channels
			cnt=0;
			for (int j=0; j<m_nbCh; j++) if ((ChannelMask>>j)&1){ cnt++; n_channels++; }
			Size=Size/cnt;

			// ignore EventConter and TTT
			pnt+=2;
			
			for (int j=0; j<m_nbCh; j++) { // read all activated channels
				m_mean=0;
			
				// read only the channels given in ChannelMask
				if ((ChannelMask>>j)&1 && m_DACFinished[j]==0){CurrentChannel=j;}
						else continue;
				
				if (CurrentChannel!=j) { pnt+=Size; continue; }

				if (j>j) return 0;	
				  
				cnt=0;                              // counter of waveform data
				wavecnt=0;                          // counter to reconstruct times within waveform
				while (cnt<(Size))
				{		
					m_mean+= (double)((buffer[pnt]&0xFFFF));
					m_mean+= (double)(((buffer[pnt]>>16)&0xFFFF));
					pnt++; wavecnt+=2; cnt++;
				} // end while(cnt...

				m_mean=m_mean/(wavecnt);
				m_diff=m_mean-m_DACTarget[j];

				m_correction= ((m_Voltage/pow(2.0,m_resADC))*m_diff)/((m_Voltage)/pow(2,m_resDAC));		//(mV of one Count ADC * difference)/mV of one Count of DAC
				adc_readreg(DACRegN+(j*0x100),m_hex);	
				m_hex=m_hex+m_correction;
				m_DACLevel[j]=m_hex;

				std::cout << "::::::: Module: " << m_module << " Channel: " << j << " :::::::" << std::endl ;
				std::cout << "	Mean: " << m_mean << " Target : " << m_DACTarget[j] << " Diff: " <<  m_diff << " DAC: " << m_DACLevel[j] <<  std::endl << std::endl;

				adc_writereg(DACRegN+(j*0x100),m_hex);
				if(abs(m_diff)<3.0){
					m_DACFinished[j]=1;
					continue;
				}

			} // end for-loop
		}
		else{
			printf(KRED);
			std::cout << "	Error in Reading the Event" << std::endl;
			printf(RESET);
			return -1;
		}
		//Check if the it converged
		int check=0;
		for(int k=0;k<m_nbCh;k++){
			if(m_DACFinished[k]==1)
				check=check+1;
		}
		if(check==n_channels){
			printf(KYEL);
			std::cout << "	Baseline Calculation Converged after: " << i << " Iterations " << std::endl << std::endl;
			break;
		}
		sleep(1);
    }
    printf(RESET);
    
    // create filename for Baseline.ini
    FILE *dacfile;
    std::string bp = Common::getdotdaqdir();
    bp.append("/Baseline");
    std::stringstream fn;
    system(std::string("mkdir -p ").append(bp).c_str()); // Ensure that the directory exists                                                                                                                                                                       
    fn << "Module_" << m_module << "_DACBaseline.ini";
    bp.append("/");
    bp.append(fn.str());
    dacfile=fopen(bp.c_str(),"w");
    
    if (dacfile==NULL) {
    	std::cout << ":::: ERROR: cannot open file to write baseline settings ::::" << std::endl;
    	return -1;
    }
  
  fprintf(dacfile,"#***************************************************************\n");
  fprintf(dacfile,"# CDAQ --  DAQ for UZH \n#\n"); //
  fprintf(dacfile,"# This is the baseline configuration file. The information\n");
  fprintf(dacfile,"# here sets the ADC channels to the user baseline level.\n#\n");
  fprintf(dacfile,"# generated: automatically\n");
  fprintf(dacfile,"#***************************************************************\n\n");
  
    for (int j=0; j<m_nbCh; j++)  {
      fprintf(dacfile,"WRITE_REGISTER %4d %04X\n",1098+j*100,m_DACLevel[j]);
    }  
    		  
    fclose(dacfile);
    printf(KYEL);
    std::cout << fn.str() <<  " successfully written" << std::endl << std::endl;
    printf(RESET);



    return ret; 
}




//-------------------------------------------------------------------
// Calculate BaseLine Average
double ADCManager::AverageBaseLine(int channel,double& rms){
	adc_readblt();

	double average=0;
	std::vector<double> wave;
	this->ApplySoftwareTrigger();

	//Start from the first word
	pnt =0;
	// check header
	
	if ((buffer[pnt]>>20)==0xA00 && (buffer[pnt+1]>>28)==0x0) {
		Size=((buffer[pnt]&0xFFFFFFF)-4);                   // size of full waveform (all channels)
		pnt++;
	 
		//Read ChannelMask (Handbook)
		int ChannelMask=buffer[pnt] & 0xFF;                 

		pnt++;    
		
		// Get size of one waveform by dividing through the number of channels
		cnt=0;
		for (int j=0; j<m_nbCh; j++) if ((ChannelMask>>j)&1) cnt++;
		Size=Size/cnt;

		// ignore EventConter and TTT
		pnt+=2;
			
		for (int j=0; j<m_nbCh; j++) { // read all activated channels

			// read only the channels given in ChannelMask
			if ((ChannelMask>>j)&1 && m_DACFinished[j]==0) CurrentChannel=j;
				else continue;
			
			if (CurrentChannel!=channel) { pnt+=Size; continue; }
			//	else pnt++;

			if (j>j) return 0;	
				  
			cnt=0;                              // counter of waveform data
			wavecnt=0;                          // counter to reconstruct times within waveform
			while (cnt<(Size))
			{		
				wave.push_back((double)((buffer[pnt]&0xFFFF)));
				wave.push_back((double)(((buffer[pnt]>>16)&0xFFFF)));
				pnt++; wavecnt+=2; cnt++;
			} 

			double sum =0;
			for(int i=0;i<wave.size();i++){
		        	sum +=wave[i];
			}
	       		average = sum/(wave.size());

			for (int i=0; i<wave.size() ;i++){
		        	rms+=(wave[i]-average)*(wave[i]-average);
	       	 	}
			rms = sqrt((1./(wave.size()))*rms);
		}

	}
	else{
		printf(KRED);
		std::cout << "	Error in Reading the Event" << std::endl;
		printf(RESET);
	}
	return average; 
}


int ADCManager::CalculateThresholds(int channel, double baseline){
	
	m_hex=0;
	int tmp = baseline - m_channelThresh[channel];
	adc_writereg(ThresholdRegN+(channel*0x0100),tmp);
	return tmp;

}



//-------------------------------------------------------------------





//Function to aquire data if there is some
int ADCManager::CheckEventBuffer(int eventCounter){
  // Interrupts: 
  // If enabled, wait for the interrupt request from the digitizer. In this mode,
  // there is no CPU time wasting because the process sleeps until there are at least
  // N event available for reading, where N is the content of the register Interrupt
  // Event Number (0xEF18)
  if (m_EnableInt) {		
    if (m_EnableOLIrq) IrqMask = 0x01; // IRQ1 is used when interrupt generated via OLINK
    else IrqMask = 0xFF; // All VME Interrupt Enabled

    CAENVME_IRQEnable(m_handle, IrqMask); // Enable IRQs
    ret = CAENVME_IRQWait(m_handle, IrqMask, 1000); // Wait for IRQ (max 1sec)
    //if (ret) continue; 

    ret=CAENVME_IRQCheck(m_handle, &IrqCheckMask);                           
    CAENVME_IRQDisable(m_handle, IrqMask); // Disable IRQs	    	    	    
  }

  // Read data from ADC  in MBLT mode into buff[i]
  blt_bytes=adc_readblt();
  if (blt_bytes<0) return -1;;

  return 0;
}


//-------------------------------------------------------------------
//    performs a single VME write
//   returns -1 on failure, 0 on success
int ADCManager::adc_writereg(u_int32_t addr,		    // the register to write to
			     u_int32_t data)		    // the value to set
{
  if(addr == PostTriggerReg) cout<<"Post trigger writting! m_CrateHandle = "<<m_CrateHandle<<", Full Address = "<<m_ADCaddr+addr<<" data written is "<<data<<endl;
if (CAENVME_WriteCycle(m_handle, m_ADCaddr+addr, &data, cvA32_U_DATA, cvD32) != cvSuccess) {
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
int ADCManager::adc_readreg(u_int32_t addr,		// the register to write to
			    u_int32_t& data)		// the value to read
{
   u_int32_t temp;
    cout<<"Is this being used? adcReadreg"<<endl; 
   if (CAENVME_ReadCycle(m_handle, m_ADCaddr+addr, &temp, cvA32_U_DATA, cvD32) != cvSuccess) {
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
int ADCManager::adc_readblt()		// the value to read
{

  //uint32_t bufferSize;
  //char* bufferTemp;
  
    cout<<"Is this being used? readblt"<<endl; 
  // read the event
  int nb, ret;     
  // Read data from module i in MBLT mode into buff     
  blt_bytes=0;
  do { 
    ret = CAENVME_FIFOBLTReadCycle(m_handle, m_ADCaddr, 
        ((unsigned char*)buffer)+blt_bytes, (1024*1024*8*10), cvA32_U_BLT, cvD32, &nb);
    //int error;
    //error = CAEN_DGTZ_ReadData(m_handle,CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT,bufferTemp,&bufferSize); //reads raw data stream from CAEN
    //if(error!=0){
    //    printf("Failed to read from CAEN, error %d\n",error);
    //}

    if ((ret != cvSuccess) && (ret != cvBusError)) {
      std::cout << "Block read error" << std::endl;   
      printf("%d bytes read\n",nb);
      return -1;  
    }
    blt_bytes += nb;
    if (blt_bytes > m_BufferSize) { 
      std::cout << "Negativ bytes transfered" << std::endl;
      return -1;  
    }
  } while (ret != cvBusError); 


  return  blt_bytes;
}	

//-------------------------------------------------------------------
// Apply the Settings from the RegisterConfigFile to the ADC
int ADCManager::RegisterWriting(string configfilename){
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
//Set Address of the ADC
void ADCManager::SetADCAddress(string address){
	m_ADCaddr=StringToHex(address);
	return;
}

//-------------------------------------------------------------------
//Convert String to Hex
u_int32_t ADCManager::StringToHex(const string &str){
   stringstream ss(str);
   u_int32_t result;
   return ss >> std::hex >> result ? result : 0;
}
//-------------------------------------------------------------------
//Convert Int to String
string ADCManager::IntToString(const int num)
{
   ostringstream convert;
   convert<<num;
   return convert.str();
}


int ADCManager::Checkkeyboard(char c){
	
	

	if (c == 'w') {
		this->Disable();
		for(int i=0;i<m_nbCh;i++){
			m_channelThresh[i]=m_channelThresh[i]-10;
			adc_writereg(ThresholdRegN+(i*0x0100),m_channelThresh[i]);
		}
		this->Enable();
	}
	
	if (c == 'e') {
		this->Disable();
		for(int i=0;i<m_nbCh;i++){
			m_channelThresh[i]=m_channelThresh[i]+10;
			adc_writereg(ThresholdRegN+(i*0x0100),m_channelThresh[i]);
		}
		this->Enable();
	}
	
	if (c == 'r') {
			this->Disable();
		for(int i=0;i<m_nbCh;i++){
			m_channelThresh[i]=m_channelThresh[i]-1;
			adc_writereg(ThresholdRegN+(i*0x0100),m_channelThresh[i]);
		}
		this->Enable();
	}
	
	if (c == 't') {
		this->Disable();
		for(int i=0;i<m_nbCh;i++){
			m_channelThresh[i]=m_channelThresh[i]+1;
			adc_writereg(ThresholdRegN+(i*0x0100),m_channelThresh[i]);
		}
		this->Enable();
	}
	if (c == 'i') {
		std::cout << std::endl;
		for(int i=0;i<m_nbCh;i++){
			std::cout << "		Threshold: " << m_channelThresh[i];	
		}
		std::cout << "		Posttrigger: " << m_posttrigger ;
		std::cout << std::endl << std::endl;
	}
	
	if (c == 'o') {
		m_posttrigger=m_posttrigger-1;
		adc_writereg(PostTriggerReg,m_posttrigger);
	}
	
	if (c == 'p') {
		m_posttrigger=m_posttrigger+1;
		adc_writereg(PostTriggerReg,m_posttrigger);
	}
	
	return 0;
}









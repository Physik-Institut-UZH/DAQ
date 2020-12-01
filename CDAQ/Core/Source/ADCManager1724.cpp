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
#include "ADCManager1724.h"
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

ADCManager1724::ADCManager1724()
{
	m_CrateHandle= m_ADCaddr=0;
  Event16=NULL; /* generic event struct with 16 bit data (10, 12, 14 and 16 bit digitizers */
	m_EnableVMEIrq=m_Align64=m_EnableBerr=m_EnableOLIrq=m_EnableInt=m_EvAlign=m_Frequency=m_Baseline=m_resDAC=m_resDAC=m_Voltage=m_nbCh=m_triggertyp=m_SoftwareRate=m_module=0;
	for(int i=0;i<8;i++){
		m_DACTarget[i]=0;
	}
}

ADCManager1724::~ADCManager1724()
{
}

int ADCManager1724::Init(){

	printf(KYEL);
	printf("\nReset the ADC Module %i . . .\n\n",m_module);
	printf(RESET);

  //Reset the board first
  m_hex=0x1;
  adc_writereg(SoftwareResetReg,m_hex);
  sleep(3);

  //Set Register Setting from default file (only to give the user some freedom)
  RegisterWriting(m_RegisterFileName);

	//Set Register/other Settings from xml-file
  ApplyXMLFile();

	//Read Settings
	RegisterReading();

  //
 CalculateBaseline();
  startAcq();
	
	return 0;
}

//-------------------------------------------------------------------
// Reading some Register Settings from the ADC
int ADCManager1724::RegisterReading(){
	printf(KYEL);
	printf("Reading ADC Configurations\n\n");
	printf(RESET);

	printf(KGRN);
	u_int32_t data;

	// Read Board Type and Memory Size
 	adc_readreg(0x8140,data);
	m_MemorySize=(int)((data >> 8) & 0xFF);
	printf("	Board Type: %s;  Memory Size: %d MByte per channel\n", "v1724",m_MemorySize);

    	// Read Firmware Revisions
	adc_readreg(FirmwareRegN,data);	
    	printf("	Firmware: Mezzanine: %d.%d (%x/%d/%d), ", data>>8 & 0xFF, data & 0xFF, data>>16 & 0xFF, data>>24 & 0xF, 2000 + (data>>28 & 0xF));

	adc_readreg(MotherBoardFWRevision,data);	
    	printf("	Mother Board: %d.%d (%x/%d/%d)\n", data>>8 & 0xFF, data & 0xFF, data>>16 & 0xFF, data>>24 & 0xF, 2000 + (data>>28 & 0xF));

	adc_readreg(BlockOrganizationReg,data);
	 printf("	Block Organization: %i\n",data);

    	// Expected Event Size in Words (32Bit including Header)
	m_ExpectedEvSize = (int)((((m_MemorySize*pow(2,20))/(int)pow(2,data))*8+16)/4);			//From the Handbook  
  	m_BufferSize = (m_EvAlign&&m_EnableBerr) ? (m_ExpectedEvSize* m_EvAlign*4):(m_ExpectedEvSize*4);
//   	m_BufferSize += 524288;

	m_BufferSize = 1024*1024*8*10;
        // allocate memory for buffer
    	if ( (buffer = (u_int32_t*)malloc(m_BufferSize)) == NULL) {  
		printf(KRED);
		printf(":::: ERROR: Can't allocate memory buffer of %d kB ::::", m_BufferSize/1024);
		printf(RESET);
		return -1;
  	}

	// Read DAC Status
	for(int i=0;i<8;i++){
		adc_readreg( StatusRegN+(i*0x100), data);
       	        if (data&4) printf("	Channel %i DAC Status: busy\n",i);
       		else printf("	Channel %i DAC Status: ok\n",i);
	}

	//Read Channel Enable Mask
	data=0xFF;
	adc_readreg(ChannelEnableMaskReg,data);
	m_hex=0;
	for(int i=0; i<8;i++){
		adc_readreg(ThresholdRegN+(i*0x0100),m_hex);
		std::cout << "	Channel: " << i << "	" << ((data >> i) & 0x01) << " Threshold: " << std::dec << m_hex << std::endl;
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
 	adc_readreg(FrontPanelIODataReg,data);	
	 if (data & 1) printf("	Front Panel Input Signal: TTL\n"); 
     	           else printf("	Front Panel Input Signal: NIM\n"); 

	//Read customsize window
	adc_readreg(CustomWindowReg,data);
	printf("	Customsize window: %d\n", (int)data*2);
	m_length=(int)data*2-CORRECTION;								//Correction of the Event (last 10 Samples are broken

	//read Posttrigger Settings
	adc_readreg(PostTriggerReg,data);	
    printf("	PostTrigger Setting: %d samples = %.2f us\n", (int)(data)*2,(2*data)*0.010);

	printf(RESET);
	
	if (!m_EnableBerr) {  // BERR must be enabled!
      		std::cout << ":::: ERROR: BERR must be enabled to run the program! ::::" << std::endl;
		return -1;
    }

	return 0;
}

int ADCManager1724::ApplyXMLFile(){
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

	m_hex=0x0;
	m_channelThresh = new int[m_nbCh];
	
	for(int i=0;i<8;i++){
			char channel[300];
			sprintf(channel,"ch_%i",i+m_module*m_nbCh);
			xstr=xNode.getChildNode(channel).getText();
			if (xstr) {
				strcpy(txt,xstr);
				temp=atoi(txt);
				m_channelThresh[i]=temp;
				if(temp!=0){
					m_hex=m_hex+pow(2,i);
					adc_writereg(ThresholdRegN+(i*0x0100),temp);
				}
			} else error((char*)channel);
	}
    adc_writereg(ChannelEnableMaskReg,m_hex);
    
    xstr=xNode.getChildNode("memoryorganisation").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		switch (atoi(txt)) {
			case 512: m_hex=0x0; break;
			case 256: m_hex=0x1; break;
			case 128: m_hex=0x2; break;
			case  64: m_hex=0x3; break;
			case  32: m_hex=0x4; break;
			case  16: m_hex=0x5; break;
			case   8: m_hex=0x6; break;
			case   4: m_hex=0x7; break;
			case   2: m_hex=0x8; break;
			case   1: m_hex=0x9; break;
			case   0: m_hex=0xA; break;  // 0.5 k
			default:  m_hex=0x4; 
					  break; 
		}
	  adc_writereg(BlockOrganizationReg,m_hex);
	} else error((char*)"XML-memoryorganisation");
  
	xstr=xNode.getChildNode("custom_size").getText();
	if (xstr) {
        strcpy(txt,xstr);
        temp=(int)(atoi(txt)/2.);       // 2 samples per memory location
	adc_writereg(CustomWindowReg,temp);
	} else error((char*)"XML-custom_size");
	
	
	xstr=xNode.getChildNode("posttrigger").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		temp=((int)atoi(txt))/2;
		m_posttrigger=temp;
		adc_writereg(PostTriggerReg,temp);
	} else error((char*)"XML-posttrigger");
		
	xstr=xNode.getChildNode("baseline").getText();
	if (xstr) {
		strcpy(txt,xstr);
		m_Baseline=(int)atoi(txt); 
		if(m_Baseline==0){
			for(int i=0;i<8;i++){
				char channel[300];
				sprintf(channel,"baseline_%i",i);
				xstr=xNode.getChildNode(channel).getText();
				if (xstr) {
					strcpy(txt,xstr);
					temp=atoi(txt);
					m_DACTarget[i]=temp;
				} else error((char*)channel);
			}
		}
		else{
			for(int i=0;i<8;i++){
				m_DACTarget[i]=m_Baseline;
			}		
		}
		
	} else error((char*)"XML-baseline");
	
	xstr=xNode.getChildNode("baselineiteration").getText();
	if (xstr) {
		strcpy(txt,xstr);
		m_iteration=(int)atoi(txt); 
	} else error((char*)"XML-baselineiteration");
	
	xstr=xNode.getChildNode("sampling_freq").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		m_Frequency=(int)atoi(txt); 
	} else error((char*)"XML-sampling_freq");
	
	xstr=xNode.getChildNode("voltage_range").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		m_Voltage=(int)atoi(txt); 
	} else error((char*)"voltage_range");
	
	xstr=xNode.getChildNode("Sample_size_ADC").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		m_resADC=(int)atoi(txt); 
	} else error((char*)"Sample_size_ADC");
	
	xstr=xNode.getChildNode("Sample_size_DAC").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		m_resDAC=(int)atoi(txt); 
	} else error((char*)"Sample_size_DAC");
	
	// ADC: parse ADC Trigger Settings
	xNode=xMainNode.getChildNode("adc").getChildNode("triggerSettings");

	xstr=xNode.getChildNode("trigger").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		temp=atoi(txt);
		m_triggertyp=temp;
    if (temp==0) {
		//External + Software Trigger always activated 
		m_hex=0xC0000000;
		adc_writereg(FrontPanelTriggerOutReg,m_hex);
		adc_writereg(TriggerSourceMaskReg,m_hex);
	}
  	else if (temp==1){ 
		//External + Software Trigger always activated 
		m_hex=0x80000000;
		adc_writereg(FrontPanelTriggerOutReg,m_hex);	
		adc_writereg(TriggerSourceMaskReg,m_hex);
		
		xstr=xNode.getChildNode("SoftwareRate").getText();;
		if (xstr) {
				strcpy(txt,xstr); 
				m_SoftwareRate=atoi(txt);
		}
		else error((char*)"SoftwareRate");
	}
    else if(temp==2){
		m_hex=0;
		for(int i=0;i<8;i++){
			char logic[300];
			sprintf(logic,"trig%i",i+m_nbCh*m_module);
			xstr=xNode.getChildNode(logic).getText();
			
			if (xstr) {
				strcpy(txt,xstr); 
				if(atoi(txt)==1){
					m_hex=m_hex+pow(2,i);
				}
			}
			else error((char*)logic);
		}
		//Software + Channel Trigger 
		m_hex=m_hex+pow(2,31);
		adc_writereg(FrontPanelTriggerOutReg,m_hex);
		adc_writereg(TriggerSourceMaskReg,m_hex);
	}
    else{
		//Coincidence Trigger TODO
		m_hex=0;
		for(int i=0;i<8;i++){
			char logic[300];
			sprintf(logic,"trig%i",i+m_nbCh*m_module);
			xstr=xNode.getChildNode(logic).getText();
			
			if (xstr) {
				strcpy(txt,xstr); 
				if(atoi(txt)==1){
					m_hex=m_hex+pow(2,i);
				}
			}
			else error((char*)logic);
		}

		//Max Coincidence Window
		//m_hex=m_hex+pow(2,24);
		m_hex=m_hex+pow(2,20);
		m_hex=m_hex+pow(2,21);
		m_hex=m_hex+pow(2,22);
		m_hex=m_hex+pow(2,23);

		//m_hex=m_hex+pow(2,25);
//		std::cout << "Try my best" << std::endl;
		m_hex=m_hex+pow(2,24);	

		m_hex=m_hex+pow(2,31);
		adc_writereg(FrontPanelTriggerOutReg,m_hex);
		adc_writereg(TriggerSourceMaskReg,m_hex);

	 }
   }
   else error((char*)"XML-trigger");

	xstr=xNode.getChildNode("TTL").getText();
	if (xstr) {
		strcpy(txt,xstr);
		m_hex=(int)(atoi(txt));
		if(temp==1){
			adc_writereg(FrontPanelIODataReg,m_hex);
		}
		else{
			adc_writereg(FrontPanelIODataReg,m_hex);
		}
	} else error((char*)"ADC-Manager-XML-TTL");

        xNode=xMainNode.getChildNode("adc").getChildNode("ZLE");
        xstr=xNode.getChildNode("ZLEActivated").getText();
	if (xstr) {
 		strcpy(txt,xstr);
   	        temp=((int)atoi(txt));
		if(temp==1){
			adc_writereg(0x8000,0x20010);
                        adc_writereg(0x8028,0x190019); //50 samples left and right

			for(int i=0;i<8;i++){
				m_hex = m_channelThresh[i];
//				m_hex = m_hex + pow(2,31); //Negative or positive 
//				std::cout << m_hex << std::endl;
               			adc_writereg(ZLEThreshReg+(i*0x0100),m_hex);
			}
        	}
	}
        else error((char*)"ZLE");

        xstr=xNode.getChildNode("ZLELEFT").getText();
        if (xstr) {
                strcpy(txt,xstr);
                temp=((int)atoi(txt));
  //              std::cout << temp << std::endl;
        }
        else error((char*)"ZLE");

        xstr=xNode.getChildNode("ZLERIGHT").getText();
        if (xstr) {
                strcpy(txt,xstr);
                temp=((int)atoi(txt));
//                std::cout << temp << std::endl;
        }
        else error((char*)"ZLE");




	return 0;
}

int ADCManager1724::CalculateBaseline(){
  //return CAEN_DGTZ_Success;
  
  float cal = 1;
  float offset = 0;
  int i = 0, k = 0, p = 0, acq = 0, ch=0;
  CAEN_DGTZ_ErrorCode m_ret;
  CAEN_DGTZ_AcqMode_t mem_mode;
  uint32_t  AllocatedSize;

  uint32_t BufferSize;
	
  CAEN_DGTZ_EventInfo_t       EventInfo;
  char *buffer = NULL;
  char *EventPtr = NULL;
  CAEN_DGTZ_UINT16_EVENT_t    *Event16 = NULL;
  int cal_ok[m_nbCh];

  m_ret = CAEN_DGTZ_GetAcquisitionMode(m_handle, &mem_mode);//chosen value stored
  if (m_ret)
    printf("Error trying to read acq mode!!\n");
  m_ret = CAEN_DGTZ_SetAcquisitionMode(m_handle, CAEN_DGTZ_SW_CONTROLLED);
  if (m_ret)
  cout<<errors[abs((int)m_ret)]<<" (code "<<m_ret<<")"<<endl;
  m_ret = CAEN_DGTZ_SetExtTriggerInputMode(m_handle, CAEN_DGTZ_TRGMODE_DISABLED);
  if (m_ret)
    printf("Error trying to set ext trigger!!\n");
  m_ret = CAEN_DGTZ_SetSWTriggerMode(m_handle, CAEN_DGTZ_TRGMODE_ACQ_ONLY);
  if (m_ret)
    printf("Someshit is bad, maybe try it agian but with less pizazz");
  m_ret = CAEN_DGTZ_SetPostTriggerSize(m_handle, 0);
  if (m_ret)
    printf("Error trying to set post trigger!!\n");
  ///malloc
  m_ret = CAEN_DGTZ_MallocReadoutBuffer(m_handle, &buffer, &AllocatedSize);
  if (m_ret) {
    cout<<errors[abs((int)m_ret)]<<" (code "<<m_ret<<")"<<endl;
    return m_ret;
  }

  m_ret = CAEN_DGTZ_AllocateEvent(m_handle, (void**)&Event16);		
  if (m_ret != CAEN_DGTZ_Success) {
    cout<<errors[abs((int)m_ret)]<<" (code "<<m_ret<<")"<<endl;
  }

  m_ret = CAEN_DGTZ_SWStartAcquisition(m_handle);
  if (m_ret){
    printf("Warning: error starting acq\n");
    cout<<errors[abs((int)m_ret)]<<" (code "<<m_ret<<")"<<endl;
    return m_ret;
  }

  #define NPOINTS 2
  #define NACQS   50
  float avg_value[NPOINTS] = { 0 };
	
  uint32_t dc[NPOINTS] = {25,75}; //test values, baseline at 25% and 75% 

  m_DCoffset = new uint32_t[m_nbCh];
  printf("Calculating baseline...\n");
  for (ch = 0; ch < (int32_t)BoardInfo.Channels; ch++)    {
    if (m_EnableMask & (1 << ch)){
      m_ret = CAEN_DGTZ_SetChannelSelfTrigger(m_handle,CAEN_DGTZ_TRGMODE_DISABLED, (1 << ch));			
      if (m_ret)
        printf("Warning: error disabling ch %d self trigger\n", ch);


      for (p = 0; p < NPOINTS; p++){
        //Make a guess for the DC offset
        m_ret = CAEN_DGTZ_SetChannelDCOffset(m_handle, (uint32_t)ch, (uint32_t)((float)(fabs((int)dc[p] - 100))*(655.35)));
        if (m_ret)
          printf("Warning: error setting ch %d test offset\n", ch);

        //usleep(200000);
        usleep(50000);

        int value[NACQS] = { 0 };
        for (acq = 0; acq < NACQS; acq++){
          CAEN_DGTZ_SendSWtrigger(m_handle);
          m_ret = CAEN_DGTZ_ReadData(m_handle, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT, buffer, &BufferSize);
          if (m_ret) {
            cout<<errors[abs((int)m_ret)]<<" (code "<<m_ret<<")"<<endl;
            exit(0);
            }

          m_ret = CAEN_DGTZ_GetEventInfo(m_handle, buffer, BufferSize, 0, &EventInfo, &EventPtr);
          if (m_ret) {
            cout<<errors[abs((int)m_ret)]<<" (code "<<m_ret<<")"<<endl;
            return m_ret;
          }
          // decode the event //
          m_ret = CAEN_DGTZ_DecodeEvent(m_handle, EventPtr, (void**)&Event16);

          if (m_ret) {
            cout<<errors[abs((int)m_ret)]<<" (code "<<m_ret<<")"<<endl;
            return m_ret;
          }


          for (i = 1; i < 7; i++){ //mean over 6 samples
            value[acq] += (int)(Event16->DataChannel[ch][i]);
          }
          value[acq] = (value[acq] / 6);
        }//for acq

        //check for clean baselines
        //I (N.M.) found this baseline estimation method from someone on github, the basic idea is to 
        //estimate the average baseline from 50 waveforms by looking at the first 6 samples of the 
        //waveform and use that to set your DC offset. This is performed twice for two different baseline guess
        //Some nuisances...
        //Intially the base line is set to be 25% and 75%  and the code sees how well that baseline
        //is maintained after accounting for noise. This estimate is the value Cal
        //The baseline needs to be inserted as a value from 0x0000 to 0xFFFF (i.e. 0 to 65535). If the DAC is set to
        //0x0000, then no DC offset is added, and the range of the input signal goes from -Vpp to 0. Conversely, when the DAC is
        //set to 0xFFFF, the DC offset is Vpp and the range goes from 0 to +Vpp. 
        //When calculating the average baseline, ADC counts are used, this have to be converted into the accetable range to progem the DAQ
        //I have added a XML command to leave some fraction of baseline below the signal incase unexpected signals arrive
        int max = 0;
        int mpp = 0;
        int size = (int)pow(2, (double)BoardInfo.ADC_NBits);//returns 2^14 = 16384
        int *freq = (int*)calloc(size, sizeof(int));

        for (k = 0; k < NACQS; k++){
          if (value[k] > 0 && value[k] < size){
            freq[value[k]]++;
            if (freq[value[k]] > max) {
              max = freq[value[k]]; 
              mpp = value[k]; 
            }
          }
        }

        free(freq);
        int ok = 0;
        for (k = 0; k < NACQS; k++) {
          if (value[k] == mpp || value[k] == (mpp + 1) || value[k] == (mpp - 1)){
            avg_value[p] = avg_value[p] + (float)value[k]; ok++;
          }
        }
        //Convert ADC counts into a percentage
        avg_value[p] = (avg_value[p] / (float)ok)*100. / (float)size; 

      }//close for p
      cal = ((float)(avg_value[1] - avg_value[0]) / (float)(dc[1] - dc[0]));
      offset = (float)(dc[1] * avg_value[0] - dc[0] * avg_value[1]) / (float)(dc[1] - dc[0]);
      //printf("Cal %f   offset %f\n", cal, offset);


      if (m_PulsePolarity[ch] == CAEN_DGTZ_PulsePolarityPositive){
        //DCoffset[ch] = (uint32_t)((float)(fabs(( ((float)dc_file[ch] - offset )/ cal ) - 100.))*(655.35));
        m_DCoffset[ch] = (uint32_t)((float)(fabs(( ( - offset )/ cal ) - 100.))*(655.35) - 65535*m_Baseline);
        if (m_DCoffset[ch] > 65535) m_DCoffset[ch] = 65535;
        if (m_DCoffset[ch] < 0) m_DCoffset[ch] = 0;		      
      }
      else if (m_PulsePolarity[ch] == CAEN_DGTZ_PulsePolarityNegative || 1){
        //m_DCoffset[ch] = (uint32_t)((float)(fabs(( (fabs(dc_file[ch] - 100.) - offset) / cal ) - 100.))*(655.35));
        m_DCoffset[ch] = (uint32_t)((float)(fabs(( ( 100. - offset) / cal ) - 100.))*(655.35) + 65535*m_Baseline);
        if (m_DCoffset[ch] < 0) m_DCoffset[ch] = 0;
        if (m_DCoffset[ch] > 65535) m_DCoffset[ch] = 65535;		      
      }

      //cout<<"Calibrated DC offset (between 0 and 65535)"<<m_DCoffset[ch]<<", frac "<<m_DCoffset[ch]/65535.<<endl;

      m_ret = CAEN_DGTZ_SetChannelDCOffset(m_handle, (uint32_t)ch, m_DCoffset[ch]);
      if (m_ret)
        printf("Warning: error setting ch %d offset\n", ch);
      usleep(200000);
    }//if ch enabled

  }//loop ch

  //printf("DAC Calibration ready\n");

  CAEN_DGTZ_SWStopAcquisition(m_handle);  

  ///free events e buffer
  CAEN_DGTZ_FreeReadoutBuffer(&buffer);
  CAEN_DGTZ_FreeEvent(m_handle, (void**)&Event16);

  //TODO
  SetCorrectThreshold();

  //Restore previous settings
  CAEN_DGTZ_SetPostTriggerSize(m_handle, m_PostTrigger);
  CAEN_DGTZ_SetAcquisitionMode(m_handle, mem_mode);
  CAEN_DGTZ_SetExtTriggerInputMode(m_handle, m_ExtTriggerMode);
  CAEN_DGTZ_GetRecordLength(m_handle,&m_BufferSize);
  if (m_ret)
    printf("Warning: error setting recorded parameters\n");

  //TODO
  //CalibComplete=true;
  
  return CAEN_DGTZ_Success;
}

CAEN_DGTZ_ErrorCode ADCManager1724::SetCorrectThreshold(){
//Threshold needs to be adjusted for DC offset  
  CAEN_DGTZ_UINT16_EVENT_t    *Event16;
  int i = 0,ch=0;
  CAEN_DGTZ_ErrorCode m_ret;
  uint32_t  AllocatedSize;
  
  uint32_t BufferSize;
  CAEN_DGTZ_EventInfo_t       EventInfo;
  char *buffer = NULL;
  char *EventPtr = NULL;
  
  ///malloc
  m_ret = CAEN_DGTZ_MallocReadoutBuffer(m_handle, &buffer, &AllocatedSize);
  if (m_ret) {
    return m_ret;
  }
  
  m_ret = CAEN_DGTZ_AllocateEvent(m_handle, (void**)&Event16);
  if (m_ret != CAEN_DGTZ_Success) {
    return m_ret;
  }
  
  uint32_t mask;
  CAEN_DGTZ_GetChannelEnableMask(m_handle, &mask);

  CAEN_DGTZ_SWStartAcquisition(m_handle);
  //For the 1730, channels are paired such that the trigger conditions are the same for channel N and channel N+1
  for (ch = 0; ch < (int32_t)BoardInfo.Channels; ch+=2){
    if (mask & (1 << ch)){
      int baseline = 0;
      CAEN_DGTZ_SendSWtrigger(m_handle);

      m_ret = CAEN_DGTZ_ReadData(m_handle, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT, buffer, &BufferSize);
      if (m_ret) {
        cout<<errors[abs((int)m_ret)]<<" (code "<<m_ret<<")"<<endl;
        exit(0);
      }

      m_ret = CAEN_DGTZ_GetEventInfo(m_handle, buffer, BufferSize, 0, &EventInfo, &EventPtr);
      if (m_ret) {
        cout<<errors[abs((int)m_ret)]<<" (code "<<m_ret<<")"<<endl;
        return m_ret;
      }
      // decode the event //
      m_ret = CAEN_DGTZ_DecodeEvent(m_handle, EventPtr, (void**)&Event16);

      if (m_ret) {
        cout<<errors[abs((int)m_ret)]<<" (code "<<m_ret<<")"<<endl;
        return m_ret;
      }


      for (i = 1; i < 11; i++){ //mean over 10 samples
        baseline += (int)(Event16->DataChannel[ch][i]);
      }
      baseline = (baseline / 10);

      if (m_PulsePolarity[ch] == CAEN_DGTZ_PulsePolarityPositive){
        m_channelThresh[ch] += (uint32_t)baseline;
      }
      else 	if (m_PulsePolarity[ch] == CAEN_DGTZ_PulsePolarityNegative){
        //cout<<"Baseline calc "<<baseline<<", thesh "<<m_channelThresh[ch]<<endl;
        m_channelThresh[ch] = (uint32_t)baseline - m_channelThresh[ch];
      }

      if (m_channelThresh[ch] < 0) m_channelThresh[ch] = 0;
      int size = (int)pow(2, (double)BoardInfo.ADC_NBits);
      if (m_channelThresh[ch] > (uint32_t)size) m_channelThresh[ch] = size;
      //cout<<"Calibrated threshold: "<<m_channelThresh[ch]<<endl;


      m_ret = CAEN_DGTZ_SetChannelTriggerThreshold(m_handle, ch, m_channelThresh[ch]);
      if (m_ret)
        printf("Warning: error setting ch %d corrected threshold\n", ch);

      //Do channel trigger is external is disabled
      if(m_SelfTriggerMode == CAEN_DGTZ_TRGMODE_ACQ_ONLY )
        CAEN_DGTZ_SetChannelSelfTrigger(m_handle, CAEN_DGTZ_TRGMODE_ACQ_ONLY, (1 << ch));
      else
        CAEN_DGTZ_SetChannelSelfTrigger(m_handle, CAEN_DGTZ_TRGMODE_DISABLED, (1 << ch));

    }
  }
  CAEN_DGTZ_SWStopAcquisition(m_handle);

  CAEN_DGTZ_FreeReadoutBuffer(&buffer);
  CAEN_DGTZ_FreeEvent(m_handle, (void**)&Event16);
  
  return CAEN_DGTZ_Success;
}


int ADCManager1724::CheckEventBuffer(){
  //Do something
  //startAcq();
  CAEN_DGTZ_ErrorCode m_ret = CAEN_DGTZ_Success;

  //Seems like if you put this in the header file, the code crashes...IDK why -N.M.
  CAEN_DGTZ_EventInfo_t       EventInfo;
  double startTimedebug = std::time(nullptr);
  bool Quit = false;
  int eventCounter = 0,loopCounter = 0;
  while(!Quit){
    //cout<<"Top of the loop"<<endl;
    if(std::time(nullptr) - startTimedebug > 2){
      Quit = true;
    }
    //TODO need to add in adcManager->GetTransferedBytes() <= 0, i.e. a non zero amount of data was transfered
    //
    if(m_SWTriggerMode == CAEN_DGTZ_TRGMODE_ACQ_ONLY){
      //usleep 
      usleep(GetSoftwareRate()); 
      CAEN_DGTZ_SendSWtrigger(m_handle);
    }
    /// Read data from the board ///
    m_BufferSize = 0;//Zero out BufferSize before reading data
    m_Ret = CAEN_DGTZ_ReadData(m_handle, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT, m_buffer, &m_BufferSize);
    if (m_Ret) {
      cout<<errors[abs((int)m_Ret)]<<" (code "<<m_Ret<<")"<<endl;
      exit(0);
    }
    uint32_t nEventsPerTrigger = 0;
    if (m_BufferSize != 0) {
      m_Ret = CAEN_DGTZ_GetNumEvents(m_handle,m_buffer, m_BufferSize, &nEventsPerTrigger);
      if (m_Ret) {
        cout<<errors[abs((int)m_Ret)]<<" (code "<<m_Ret<<")"<<endl;
        exit(0);
      }
    }
    /*
    //I don't understand this statement so I am commenting it out - N.M.
    else {
      uint32_t lstatus;
      m_Ret = CAEN_DGTZ_ReadRegister(m_handle, CAEN_DGTZ_ACQ_STATUS_ADD, &lstatus);
      if (m_Ret) {
        printf("Warning: Failure reading reg:%x (%d)\n", CAEN_DGTZ_ACQ_STATUS_ADD, m_Ret);
        cout<<errors[abs((int)m_Ret)]<<" (code "<<m_Ret<<")"<<endl;
      }
      else {
        if (lstatus & (0x1 << 19)) {
          exit(0);
        }
      }
    }
    */
    //TODO add info about data, currently reported somewhere else
    
    // Analyze data //
    for(int i = 0; i < (int)nEventsPerTrigger; i++) {

      //// Get one event from the readout buffer //
      m_ret = CAEN_DGTZ_GetEventInfo(m_handle, m_buffer, m_BufferSize, i, &EventInfo, &m_EventPtr);
      if (m_ret) {
        cout<<errors[abs((int)m_ret)]<<" (code "<<m_ret<<")"<<endl;
        continue;
      }
      //TODO cast Event16 into a vector of Event16's
      m_ret = CAEN_DGTZ_DecodeEvent(m_handle, m_EventPtr, (void**)&Event16);
      if (m_ret) {
        cout<<errors[abs((int)m_ret)]<<" (code "<<m_ret<<")"<<endl;
        Quit = true;
      }
    }
    eventCounter+=nEventsPerTrigger;
    if(eventCounter >=m_NumEvents){
      //cout<<"event 16 size "<<Event16->ChSize[0]<<" Buffer size "<<m_BufferSize<<" number of events "<<nEventsPerTrigger<<endl;
      //cout<<"\tNumber of triggers satisfied, Quit is true"<<endl;
      Quit = true;
    }
  }
  //TODO not sure if this is necessary
  /*
  m_ret = CAEN_DGTZ_SWStopAcquisition( m_handle);
  if (m_ret) {
    cout<<errors[abs((int)m_ret)]<<" (code "<<m_ret<<")"<<endl;
  }
  */
  //cout<<"leaving the checkBuffer"<<endl;
  return 0;
}

bool ADCManager1724::startAcq(){
      
  //if(!CalibComplete) Calibrate_DC_Offset();

  /* 
  Description:  This function starts the acquisition in a board using a software command. When the acquisition starts, the relevant
  RUN LED on the front panel lights up. It is worth noticing that in case of multiple board systems, the software start
  doesn’t allow the digitizer to start synchronously. For this purpose, it is necessary to use to start the acquisition using a
  physical signal, such as the S-IN or GPI as well as the TRG-IN-TRG-OUT Daisy chain. Please refer to Digitizer manual for
  more details on this issue.
  */
  printf("Digitizer: Acquisition started\n");
  u_int32_t numEvents;
  CAEN_DGTZ_GetMaxNumEventsBLT(m_handle,&numEvents);
  CAEN_DGTZ_SetMaxNumEventsBLT(m_handle,1);
  m_NumEvents = 1;

  // Allocate memory for the event data and readout buffer
  m_Ret = CAEN_DGTZ_AllocateEvent(m_handle, (void**)&Event16);

  m_buffer = NULL;
  CAEN_DGTZ_MallocReadoutBuffer(m_handle,&m_buffer,&m_AllocatedSize);
  CAEN_DGTZ_SWStartAcquisition(m_handle);
  RunStartTime = util::markTime();
  //cout.precision(15);
  //cout<<"Digitizer: Start time: "<<RunStartTime<<endl;
  //fman.setRunStartTime(RunStartTime);
  
  //AcqRun = 1;
}

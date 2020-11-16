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

int ADCManager1724::CheckEventBuffer(){
  //Do something
  startAcq();
  CAEN_DGTZ_ErrorCode m_ret = CAEN_DGTZ_Success;

  //Seems like if you put this in the header file, the code crashes...IDK why -N.M.
  CAEN_DGTZ_EventInfo_t       EventInfo;
  double startTimedebug = std::time(nullptr);
  bool Quit = false;
  int eventCounter = 0;
  while(!Quit){
    if(std::time(nullptr) - startTimedebug > 2) Quit = true;
    //TODO need to add in adcManager->GetTransferedBytes() <= 0, i.e. a non zero amount of data was transfered
    //
    if(m_SWTriggerMode == CAEN_DGTZ_TRGMODE_ACQ_ONLY){
      //usleep 
      usleep(GetSoftwareRate()); 
      CAEN_DGTZ_SendSWtrigger(m_handle);
    }
    /// Read data from the board ///
    cout<<"here"<<endl;
    m_BufferSize = 0;//Zero out BufferSize before reading data
    m_Ret = CAEN_DGTZ_ReadData(m_handle, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT, m_buffer, &m_BufferSize);
    cout<<"where"<<endl;
    if (m_Ret) {
      cout<<errors[abs((int)m_Ret)]<<" (code "<<m_Ret<<")"<<endl;
      exit(0);
    }
    uint32_t nEventsPerTrigger = 0;
    if (m_BufferSize != 0) {
      cout<<"herehas"<<endl;
      m_Ret = CAEN_DGTZ_GetNumEvents(m_handle,m_buffer, m_BufferSize, &nEventsPerTrigger);
      if (m_Ret) {
        cout<<errors[abs((int)m_Ret)]<<" (code "<<m_Ret<<")"<<endl;
        exit(0);
      }
    }
    else {
      uint32_t lstatus;
      cout<<"herefgdsbhjfdgshjsfdghjsgfdjhngsfd"<<endl;
      m_Ret = CAEN_DGTZ_ReadRegister(m_handle, CAEN_DGTZ_ACQ_STATUS_ADD, &lstatus);
      cout<<"herefgdsbhjfdgshjsfdghjsgfdjhngsfd"<<endl;
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
    //TODO add info about data, currently reported somewhere else
    
      cout<<"sugar"<<endl;
    // Analyze data //
    for(int i = 0; i < (int)nEventsPerTrigger; i++) {

      cout<<"spice"<<endl;
      //// Get one event from the readout buffer //
      m_ret = CAEN_DGTZ_GetEventInfo(m_handle, m_buffer, m_BufferSize, i, &EventInfo, &m_EventPtr);
      cout<<"spice"<<endl;
      if (m_ret) {
        cout<<errors[abs((int)m_ret)]<<" (code "<<m_ret<<")"<<endl;
        continue;
      }
      //TODO cast Event16 into a vector of Event16's
      m_ret = CAEN_DGTZ_DecodeEvent(m_handle, m_EventPtr, (void**)&Event16);
      cout<<"poop"<<endl;
      if (m_ret) {
        cout<<errors[abs((int)m_ret)]<<" (code "<<m_ret<<")"<<endl;
        Quit = true;
      }
    }
    eventCounter+=nEventsPerTrigger;
    if(eventCounter >=m_NumEvents) Quit = true;
  }

  //TODO not sure if this is necessary
  m_ret = CAEN_DGTZ_SWStopAcquisition( m_handle);
  if (m_ret) {
    cout<<errors[abs((int)m_ret)]<<" (code "<<m_ret<<")"<<endl;
  }
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
  CAEN_DGTZ_MallocReadoutBuffer(m_handle,&m_buffer,&m_AllocatedSize);
  CAEN_DGTZ_SWStartAcquisition(m_handle);
  RunStartTime = util::markTime();
  //cout.precision(15);
  //cout<<"Digitizer: Start time: "<<RunStartTime<<endl;
  //fman.setRunStartTime(RunStartTime);
  
  //AcqRun = 1;
}

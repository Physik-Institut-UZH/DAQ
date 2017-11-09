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
		adc_readreg(TresholdRegN+(i*0x0100),m_hex);
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
	channelTresh = new int[m_nbCh];
	
	for(int i=0;i<8;i++){
			char channel[300];
			sprintf(channel,"ch_%i",i+m_module*m_nbCh);
			xstr=xNode.getChildNode(channel).getText();
			if (xstr) {
				strcpy(txt,xstr);
				temp=atoi(txt);
				channelTresh[i]=temp;
				if(temp!=0){
					m_hex=m_hex+pow(2,i);
					adc_writereg(TresholdRegN+(i*0x0100),temp);
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
				m_hex = channelTresh[i];
				m_hex = m_hex + pow(2,31);
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






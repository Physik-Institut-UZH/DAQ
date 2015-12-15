#ifndef _ADCMANAGER_H_
#define _ADCMANAGER_H_


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "CAENVMElib.h"
#include <vector>
#include <memory>
#include <typeinfo>
#include "xmlParser.h"
#include "common.h"

using namespace std;

/*
Class to manage the CAEN v1730D Flash ADC 
The ADC will be configurated over the register file (NECESSARY)!!! In order to give the user the maxmimum amount of freedom
*/

// Digitizer Register Map 
#define BoardInfoReg                0x8140
#define SoftTriggerReg              0x8108
#define AcquisitionControlReg       0x8100
#define PostTriggerReg              0x8114
#define ChannelEnableMaskReg        0x8120
#define TriggerSourceMaskReg        0x810C
#define ChannelFWRevision           0x108C
#define MotherBoardFWRevision       0x8124
#define EventSizeReg                0x814C
#define BlockOrganizationReg        0x800C
#define ChannelConfigReg            0x8000
#define VMEControlReg               0xEF00
#define BltEvNumReg                 0xEF1C
#define FrontPanelTriggerOut	    0x8110
#define FrontPanelIOData	   		0x811C
#define MonitorDacReg               0x8138
#define MonitorModeReg              0x8144
#define DACRegister		  	  		0x1098	
#define LookBackForward		   		0x8028
#define AnalogMonitorSettings	    0x8150
#define CustomWindow		   		0x8020
#define ChannelTreshold		   		0x1080
#define ChannelRange				0x1028
#define ChannelLogic				0x1084



class ADCManager: public Common
{
public:
    	ADCManager();
    	virtual ~ADCManager();
    
    	//Init Function
    	int Init();
    
    	//Set Functions
	void SetCrateHandle(int handle){m_CrateHandle=handle;}
	void SetADCAddress(string address);
	void SetRegisterFile(string file){m_RegisterFileName=file;}
	void SetBaselineFile(string file="DACBaseline.ini"){m_BaselineFileName=file;}
    void SetXMLFile(char* file){m_XmlFileName=file;}

	//Get Functions
	int GetCrateHandle(){return m_CrateHandle;};
	int GetTransferedBytes(){return blt_bytes;};
	int GetEventLength(){return m_length;}
	int GetTriggerType(){return m_triggertyp;}
	double GetSoftwareRate(){return 1000000./m_SoftwareRate;}
	int* GetTreshold(){return channelTresh;}
	
	u_int32_t* GetBuffer(){return buffer;}

	//Read the current baseline configuration from file
	int ReadBaseLine();

	//Set new Baseline configuration
	int CalculateBaseLine();

	//Trigger Software trigger
	int ApplySoftwareTrigger();
	
	//Aquire Data if there is some in the eventbuffer
	int CheckEventBuffer();
	
	//Enable ADC
	int Enable();
	
	//Disable ADC
	int Disable();
	
	//Check Keyboard
	int Checkkeyboard(char c);

private:
	//Register Read 
	int adc_readreg(u_int32_t addr, u_int32_t &data);

	//Register Write 
	int adc_writereg(u_int32_t addr,u_int32_t data);

	//Read on block of event
	int adc_readblt();

   	//Write Register from Settingfile to ADC
   	int RegisterWriting(string configfilename);

	//Read Register from ADC
	int RegisterReading();

	//Convert a String to Hex
  	u_int32_t StringToHex(const string &str);

	//Convert an int to string
	string IntToString(const int num);
  	
  	int ApplyXMLFile();											//Read XML-Config for the Storage Manager

    u_int32_t *buffer;											//Buffer for the event
	u_int32_t m_ADCaddr;										//ADC hex adress
   	u_int32_t m_addr;											//Variable Register adress
  	u_int32_t m_hex;											//Variable to write to the VME bus
	u_int32_t m_ExpectedEvSize;									//Complete Eventsize
	int m_length;												//length of the event
   	int m_CrateHandle;											//Object to the VME Bus
   	int m_MemorySize, m_BufferSize; 							//Size of the Memory on the ADC and Eventsize
   	int m_EnableVMEIrq, m_Align64, m_EnableBerr, m_EnableOLIrq, m_EnableInt, m_EvAlign;  		//VME, Interrupt etc handling
	CAEN_BYTE IrqMask, IrqCheckMask;															//Interrupt Handling
   	string m_RegisterFileName;																	//Configfile for the Register
	string m_BaselineFileName;																	//Baseline File Name
	char* m_XmlFileName;																		//XML-File
	
	// read the event
    int blt_bytes,pnt,Size,cnt,wavecnt, CurrentChannel, nb, ret;

	//Baseline calculation
	double m_mean, m_diff, m_std, m_correction;
	u_int32_t m_DACLevel[8];
	u_int32_t m_DACTarget[8];
	
	//ADC Properties
	int m_Frequency;									//Sampling frequency
	int m_Baseline;										//Baseline
	int m_resDAC;										//Resolution DAC
	int m_resADC;										//Resolution ADC
	int m_Voltage;										//Voltage range ADC
	int m_nbCh;											//Number of the Channels
	int m_triggertyp;									//External or internal trigger
	int m_nbchs;										//Channel Number
	int* channelTresh;									//Treshold of the channels
	int m_SoftwareRate;									//SoftwaretriggerRate 

};

#endif


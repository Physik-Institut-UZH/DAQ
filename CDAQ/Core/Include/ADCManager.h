#ifndef _ADCManager_H_
#define _ADCManager_H_


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "CAENVMElib.h"
#include "CAENDigitizer.h"
#include <vector>
#include <memory>
#include <typeinfo>
#include "xmlParser.h"
#include "common.h"
#include "util.h"

using namespace std;

/*
Author: Julien Wulf UZH
*/

// Digitizer Register Map v1720, v1724, v1730D
//Description from v1730D (differs for every ADC)

//This register is available to the user for writing and reading for debug purposes (default value is 0).
#define DummyReg               0x1024 

//GAIN value. Sets the analog input dynamic range (default value is 0):
//0 = GAIN is 1 (i.e. input range is 2 Vpp)
//1 = GAIN is 4 (i.e. input range is 0.5 Vpp)
#define GainRegN               0x1028

//Threshold in absolute value (e.g 8000) (0,29), 31 negative or positiv 
#define ZLEThreshReg 0x1024

//Samples before and after the threshold  (0,15) left (16,31) right
#define ZLEConf 0x1028

//Bits[7:0] set the width of the pulse generated when the input signal on channel n crosses the threshold. 
//The value of this register is given in units of trigger clock (i.e. 125 MHz).
#define PulseWidthRegN         0x1070

//Each channel can generate a self-trigger as the digitised signal exceeds the Vth threshold.
//This register allows to set Vth (LSB=input range/14bit).
#define ThresholdRegN   		   0x1080

//The settings of this register are common to couples of adjacent channels (i.e. n = 0, 2, 4, 6, 8, 10, 12, 14).
// Bits[1:0] set the logic to generate the trigger request signal for the couple:
//Bit[2] sets the kind of signal to be generated when the input pulse crosses the programmed digital threshold:
#define LogicRegN			   0x1084

//DAC Busy or Calibration Busy
#define StatusRegN			   0x1088

//Bits [31:16] contain the Revision date in Y/M/DD format.
//Bits [15:0] contain the firmware revision number coded on 16 bit (X.Y format).
#define FirmwareRegN		   0x108C

//Number of Occupied buffers (0..1024)
#define BufferOccupancyRegN	   0x1094

//Bits [15:0] allow to define a DC offset to be added the input signal according to the
//selected range being set (see § 1.4).
#define DACRegN	  	  		   0x1098

//These registers allow to monitor the temperature of ADC chips.
#define TemperaturRegN	  	   0x10A8	

//This register allows to perform settings which apply to all channels. 
//Bit [1] sets the trigger overlap. When two acquisition windows are overlapped, the second trigger can be either accepted or rejected.
//Bit[3] enable a triangular (0<-->3FFF) test wave to be provided at the ADCs input for debug purposes.
#define ChannelConfigReg       0x8000

//Bits set to 1 means that the corresponding bits in the Channel Configuration register are set to 1.
#define ChannelConfigSetReg    0x8004

//Bits set to 1 means that the corresponding bits in the Channel Configuration register are set to 0.
#define ChannelConfigClearReg  0x8008

//The BUFFER CODE allows to divide the available Output Buffer Memory into a certain number
//of blocks.
#define BlockOrganizationReg   0x800C

//Custom Size register allows the user to set the waveform length, that is the number
//samples in the waveforms. According to the formula Ns = NLOC*10.
#define CustomWindowReg		   0x8020

//These registers allow the ADC calibration procedure to be automatically performed on
//all the channels of the board.
#define ChannelCalibrationReg  0x809C

//A lot of settings for the aquistion
#define AcquisitionControlReg  0x8100

//Acquisition Status like temperature events
#define AcquisitionStatusReg  0x8104

//A write access (whatever value) to this location generates a trigger via software
#define SoftTriggerReg        0x8108

//What is triggering the board (check manual)
#define TriggerSourceMaskReg  0x810C

//What is generating the triggerout (front panel)
#define FrontPanelTriggerOutReg	    0x8110

//The register value sets the number of post trigger samples (check manual)
#define PostTriggerReg        0x8114

//Front Panel I/O Data
#define ReadoutIOReg  		  0x8118

//Front Panel I/O Control
#define FrontPanelIODataReg   0x811C

//Channel Enable Mask
#define ChannelEnableMaskReg  0x8120

//Bits [31:16] contain the Revision date in Y/M/DD format.
//Bits [15:0] contain the firmware revision number coded on 16 bit (X.Y format).
#define MotherBoardFWRevision 0x8124

//Number of Event Stored
#define BoardEventReg         0x812C

//Set Monitor DAC
#define MonitorDacReg         0x8138

//SW Clock Sync. Daisy Chain Clock
#define ClockSyncReg          0x813C

//Bits[23:16] give the number of channels equipped on the board. 
//If this number is lower than physical channels, there could be a communication problem with some of the channel mezzanines.
#define BoardInfoReg          0x8140

//Monitor DAC Mode
#define MonitorModeReg        0x8144

//It represents the current available event in 32-bit words. 
//The value is updated after the complete readout of each event.
#define EventSizeReg          0x814C

//This register allows to set the level for the Almost Full generation.
//If this register is set to 0, the Almost Full is a Full.
#define MemoryFullReg         0x816C

//Run/Start/Stop Delay Daisy Chain compensation
#define DelayReg              0x8170

//This register monitors a set of board errors.
#define BoardFailReg          0x8178

//Front Panel LVDS I/O New Features
#define FrontPanelLVDSReg     0x81A0

//Bit[0] of this register performs the shutdown of all the board’s channels.
#define ChannelShutdownReg    0x81C0

//VME Control
#define VMEControlReg         0xEF00

//VME Status
#define VMEStatusReg          0xEF04

//Board ID
#define BoardIdReg            0xEF08

//MCST Base Address and Control
//Bits[9:8] allow to set up the board for Daisy chaining.
#define MCSTBaseControlReg    0xEF0C

//Relocation Address.If address relocation is enabled (see § 1.39), this register sets the VME Base Address of the module.
#define RelocationAddressReg  0xEF10

//This register contains the STATUS/ID that the module places on the VME data bus during the Interrupt Acknowledge cycle.
#define InterruptStatusIDReg  0xEF14

//This register sets the number of events that cause an interrupt request. If interrupts are enabled, the module generates a request whenever it has stored in memory a Number
#define InterruptEventNumberReg 0xEF18

//This register contains the number of complete events which has to be transferred for each block transfer (via VME BLT/CBLT cycles or block readout through Optical Link).
#define BltEvNumReg           0xEF1C

//DK
#define ScratchReg            0xEF20

//A write access to this location of any value, allows to perform a software reset.
#define SoftwareResetReg      0xEF24

//A write access to this location of any value, clears all the internal memories.
#define SoftwareClearReg      0xEF28

//Configuration Reload
#define ConfigurationReloadReg  0xEF34

 





class ADCManager
{
public:
    	ADCManager();
      //To avoid forcing the ADCManager() class which has a hard coded 8 channels, use this temp intializer 
    	ADCManager(bool foo) { };
    	virtual ~ADCManager();
    
    	//Init Function
    	virtual int Init(){};
    
    	//Set Functions
	void SetCrateHandle(int handle){m_handle=handle;}
	void SetADCAddress(string address);
	void SetRegisterFile(string file){m_RegisterFileName=file;}
	void SetBaselineFile(string file="DACBaseline.ini"){m_BaselineFileName=file;}
    	void SetXMLFile(char* file){m_XmlFileName=file;}
	void SetModuleNumber(int i){m_module=i;}										    	//Set ADC Module in the Chain

	//Get Functions
	int GetCrateHandle(){return m_handle;};
	int GetTransferedBytes(){return blt_bytes;};
	int GetEventLength(){return m_length;}
	int GetTriggerType(){return m_triggertyp;}
  double GetSampleFreq(){return m_Frequency;}
  double  GetResolution(){return m_resDAC;}
	double GetSoftwareRate(){return 1000000./m_SoftwareRate;}
	int* GetThreshold(){return m_channelThresh;}

  //Added by Neil
  uint16_t GetEnableMask(){return m_EnableMask;}
  CAEN_DGTZ_UINT16_EVENT_t* Get16BitEvent(){return Event16;}
  u_int32_t GetBufferSize(){return m_BufferSize;}
  //TODO write code to set actual voltage range of ADC
  double GetVRange(){return 2.0;}
	
	virtual u_int32_t* GetBuffer(){return buffer;}
	//virtual char* GetBuffer(){return buffer;}

	//Read the current baseline configuration from file
	int ReadBaseLine();

	//Set new Baseline configuration
	int CalculateBaseLine();

	//Trigger Software trigger
	int ApplySoftwareTrigger();
	
	//Triggerr Software
	int SoftwareTrigger();
	
	//Aquire Data if there is some in the eventbuffer
	virtual int CheckEventBuffer(int);
	
	//Enable ADC
	int Enable();
	
	//Disable ADC
	int Disable();
	
	//Check Keyboard
	int Checkkeyboard(char c);
	
	//Sync Boards
	int SyncBoards();
	
	//Positon of the board in the chain
	int MCSTBoards(u_int32_t data);
	
	//Delay Between the boards
	int DelayBoards(u_int32_t data);

	//Enable Channel bla and disable the other once
	int EnableChannel(int channel);

	//Calculate the average baseline
	double AverageBaseLine(int channel,double& rms);

	//Calculate the dznamic thresholds
	int CalculateThresholds(int channel, double baseline);	

	//Enable Software Trigger 
	int EnableSoftware();

  //TODO
  bool OpenDigitizer(){return 0;};
	

protected:
	//Register Read 
	int adc_readreg(u_int32_t addr, u_int32_t &data);

	//Register Write 
	int adc_writereg(u_int32_t addr,u_int32_t data);

	//Read on block of event from one ADC
	int adc_readblt();

   	//Write Register from Settingfile to ADC
   	int RegisterWriting(string configfilename);

	//Read Register from ADC
	virtual int RegisterReading(){};

	//Convert a String to Hex
  	u_int32_t StringToHex(const string &str);

	//Convert an int to string
	string IntToString(const int num);
  	
  	virtual int ApplyXMLFile(){};																	//Read XML-Config for the Storage Manager

    u_int32_t *buffer;																			//Buffer for the event
	u_int32_t m_ADCaddr;																		//ADC hex adress
   	u_int32_t m_addr;																			//Variable Register adress
  	u_int32_t m_hex;																			//Variable to write to the VME bus
	u_int32_t m_ExpectedEvSize;																	//Complete Eventsize
	int m_length;																				//length of the event
  //TODO outdated variable, needs to be update 	
  int m_CrateHandle;																			//Object to the VME Bus
   	int m_MemorySize;//, m_BufferSize; 															//Size of the Memory on the ADC and Eventsize
   	int m_EnableVMEIrq, m_Align64, m_EnableBerr, m_EnableOLIrq, m_EnableInt, m_EvAlign;  		//VME, Interrupt etc handling
	CAEN_BYTE IrqMask, IrqCheckMask;															//Interrupt Handling
   	string m_RegisterFileName;																	//Configfile for the Register
	string m_BaselineFileName;																	//Baseline File Name
	char* m_XmlFileName;																		//XML-File
	
  // read the event
  int blt_bytes, pnt, Size, cnt, wavecnt, CurrentChannel, nb, ret;

	//Baseline calculation
	double m_mean, m_diff, m_std, m_correction;			//Baseline properties
	u_int32_t m_DACLevel[8];							//DAC Level Current Value
	u_int32_t m_DACTarget[8];							//DAC Level Set Value
	int m_DACFinished[8];
	
	//ADC Properties
	int m_Frequency;									//Sampling frequency
	int m_iteration;									//Number of Baseline iterations
	int m_resDAC;										//Resolution DAC
	int m_resADC;										//Resolution ADC
	int m_nbCh;											//Number of the Channels
	int m_triggertyp;									//External, internal trigger, Daisy Chain
	int m_nbchs;										//Channel Number
	int* m_channelThresh;									//Threshold of the channels
  CAEN_DGTZ_TriggerPolarity_t* m_trigPolarity;     //added by Neil
  CAEN_DGTZ_PulsePolarity_t* m_PulsePolarity;
	int m_SoftwareRate;									//SoftwaretriggerRate 
	int m_module;										//Module Number
	u_int32_t m_posttrigger;							//Postrigger

  //Added by Neil
  char * m_buffer;
  int m_handle;    //
  CAEN_DGTZ_BoardInfo_t       BoardInfo;
  CAEN_DGTZ_ErrorCode m_Ret;

  CAEN_DGTZ_IOLevel_t m_TriggerLogicType;
  CAEN_DGTZ_TriggerMode_t m_ExtTriggerMode;
  CAEN_DGTZ_TriggerMode_t m_SWTriggerMode;
  CAEN_DGTZ_TriggerMode_t m_SelfTriggerMode;

  uint32_t m_RecordLength;
  int m_PostTrigger;
  int  m_NumEvents;
  uint16_t m_EnableMask;
  CAEN_DGTZ_UINT16_EVENT_t    *Event16;
  uint32_t m_AllocatedSize;
  double RunStartTime;
  double nowTime;
	double m_Baseline;	//Baseline
	double m_Voltage;										//Voltage range ADC
  uint32_t m_NEvents;
  uint32_t m_BufferSize;
  uint32_t *m_DCoffset;

  
  char * m_EventPtr;



};
   static const string errors[34] = {"Operation completed successfully", //0
				  "Communication error",  //1
				  "Unspecified error", //2
				  "Invalid parameter", //3
				  "Invalid Link Type", //4
				  "Invalid device handler", //5
				  "Maximum number of devices exceeded", //6
				  "Operation not allowed on this type of board", //7
				  "The interrupt level is not allowed", //8
				  "The event number is bad", //9
				  "Unable to read the registry", //10
				  "Unable to write into the registry", //11
				  "", //12
				  "The Channel is busy", //13
				  "The channel number is invalid", //14
				  "Invalid FPIO Mode", //15
				  "Wrong acquisition mode",//16
				  "This function is not allowed for this module",//17
				  "Communication Timeout",//18
				  "The buffer is invalid", //19
				  "The event is not found", //20
				  "The event is invalid", //21
				  "Out of memory", //22
				  "Unable to calibrate the board", //23
				  "Unable to open the digitizer", //24
				  "The Digitizer is already open", //25
				  "The Digitizer is not ready to operate", //26 
				  "The Digitizer has not the IRQ configured",//27
				  "The digitizer flash memory is corrupted",//28
				  "The digitizer DPP firmware is not supported in this lib version", //29
				  "Invalid Firmware License",//30
				  "The digitizer is found in a corrupted status",//31
				  "The given trace is not supported by the digitizer", //32
				  "The given probe is not supported for the given digitizer's trace"}; //33

#endif


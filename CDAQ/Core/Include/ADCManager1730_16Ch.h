#ifndef _ADCManager1730_16Ch_H_
#define _ADCManager1730_16Ch_H_


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "CAENVMElib.h"
#include <vector>
#include <memory>
#include <typeinfo>
#include "xmlParser.h"
#include "ADCManager.h"
#include "common.h"

using namespace std;

/*
Author: Julien Wulf UZH
Class to manage the CAEN v1730D Flash ADC 
The ADC will be configurated over the register file and over XML File
*/



class ADCManager1730_16Ch: public Common, public ADCManager

{
public:
    	ADCManager1730_16Ch();
    	virtual ~ADCManager1730_16Ch();
    
    	//Init Function
    	int Init();
    
    //Inherieted from ADCManager and override the function to use Digitizer functions
    int CheckEventBuffer();

private:
	
		//Read Register from ADC
		//int RegisterReading();
    CAEN_DGTZ_ErrorCode OrProgrammer();
    int CalculateBaseline();
    CAEN_DGTZ_ErrorCode SetCorrectThreshold();
		int ApplyXMLFile();

    u_int32_t m_DACLevel[16];							//DAC Level Current Value
	  u_int32_t m_DACTarget[16];							//DAC Level Set Value
	  int m_DACFinished[16];
    //TODO do not hard code the size
    //uint32_t m_DCoffset[16];
    
    bool OpenDigitizer();
    bool startAcq();


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


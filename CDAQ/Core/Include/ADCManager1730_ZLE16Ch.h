#ifndef _ADCManager1730_ZLE16Ch_H_
#define _ADCManager1730_ZLE16Ch_H_


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



class ADCManager1730_ZLE16Ch: public Common, public ADCManager

{
public:
    	ADCManager1730_ZLE16Ch();
    	virtual ~ADCManager1730_ZLE16Ch();
    
    	//Init Function
    	int Init();
    
    //Inherieted from ADCManager and override the function to use Digitizer functions
    int CheckEventBuffer(int eventCounter);
    int CheckEventBufferZLE(int eventCounter);

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
  	CAEN_DGTZ_730_ZLE_Event_t      *zleEvent = NULL;


};
 

#endif


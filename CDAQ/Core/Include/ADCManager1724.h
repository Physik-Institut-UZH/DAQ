#ifndef _ADCManager1724_H_
#define _ADCManager1724_H_


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



class ADCManager1724: public Common, public ADCManager

{
public:
    	ADCManager1724();
    	virtual ~ADCManager1724();
    
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
    int RegisterReading();


    u_int32_t m_DACLevel[8];							//DAC Level Current Value
	  u_int32_t m_DACTarget[8];							//DAC Level Set Value
	  int m_DACFinished[8];
    //TODO do not hard code the size
    //uint32_t m_DCoffset[16];
    
    bool OpenDigitizer();
    bool startAcq();


};


#endif


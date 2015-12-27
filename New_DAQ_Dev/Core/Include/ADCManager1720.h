#ifndef _ADCMANAGER1720_H_
#define _ADCMANAGER1720_H_

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
Class to manage the CAEN v1720 Flash ADC 
The ADC will be configurated over the register file (NECESSARY)!!! In order to give the user the maxmimum amount of freedom
*/


class ADCManager1720: public Common, public ADCManager
{
public:
		ADCManager1720();
		virtual ~ADCManager1720();
    
		//Init Function
		int Init();
    
private:

		//Read Register from ADC
		int RegisterReading();

		int ApplyXMLFile();											//Read XML-Config for the Storage Manager
};

#endif


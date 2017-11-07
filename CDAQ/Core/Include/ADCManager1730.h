#ifndef _ADCManager1730_H_
#define _ADCManager1730_H_


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



class ADCManager1730: public Common, public ADCManager

{
public:
    	ADCManager1730();
    	virtual ~ADCManager1730();
    
    	//Init Function
    	int Init();
    

private:
	
		//Read Register from ADC
		int RegisterReading();

		int ApplyXMLFile();
};

#endif


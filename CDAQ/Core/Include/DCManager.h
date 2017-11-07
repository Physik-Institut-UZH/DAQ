#ifndef _DCMANAGER_H_
#define _DCMANAGER_H_

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
*/


class DCManager: public Common, public ADCManager
{
public:
		DCManager();
		virtual ~DCManager();
    
		//Init Function
		int Init();
		int EnableChannel(int channel);
    
private:

		//Read Register from ADC
		int RegisterReading();
		int ApplyXMLFile();											//Read XML-Config for the Storage Manager
};

#endif


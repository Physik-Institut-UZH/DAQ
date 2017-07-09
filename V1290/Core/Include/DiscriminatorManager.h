#ifndef _DISCRIMINATORManager_H_
#define _DISCRIMINATORManager_H_


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "CAENVMElib.h"
#include <vector>
#include <memory>
#include <typeinfo>


using namespace std;



class DiscriminatorManager
{
 public:
	DiscriminatorManager();
   	virtual ~DiscriminatorManager();
   	int Init();

	void SetTreshold(u_int32_t tresh);
   	int  FillThresholds(string tresholdFile);
	int  SetThresholdsDisc(int Nthr);

	void SetDiscAddress(string address);
	void SetCrateHandle(int handle){m_CrateHandle=handle;}
	int GetNthresholds(){return m_thresholds[0].size();}
 
 private:
	u_int32_t StringToHex(const string &str);

	int m_CrateHandle;				//VME-Bus Object
	u_int32_t chHex;				//Variable to write to the VME bus
	vector<u_int32_t> m_thrBuf;			//Tresholds of one Channel
	vector<vector<u_int32_t>> m_thresholds;		//Treshold container with several tresholds, for every channel
    vector<int> m_chanels;				//Container of the channel number
	string m_thresholdFile;				//Path of the Specific Treshold File
	u_int32_t m_discAdr;				//Discriminator hex adress
};	

#endif

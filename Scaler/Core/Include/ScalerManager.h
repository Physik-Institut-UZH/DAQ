#ifndef _SCALERMANAGER_H_
#define _SCALERMANAGER_H_


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "CAENVMElib.h"
#include <vector>
#include <memory>
#include <typeinfo>
#include "keyb.h"

#include "common.h"

using namespace std;

class ScalerManager: public Common
{
 public:
	ScalerManager();
   	virtual ~ScalerManager();
   	int Init();

	void SetDiscAddress(string address);
	void SetCrateHandle(int handle){m_CrateHandle=handle;}
	void SetCycles(int cycles){m_cycles=cycles;};
	void SetRate(int rate){m_rate=rate;};
	void SetScalerActive(int active){m_active=active;}
	void SetActive(int active);
	int  GetActive(){return m_active;}
	void SetPath(string path){m_path=path;}
	int  ReadMultipleCycles();

 private:
	u_int32_t StringToHex(const string &str);
	int m_CrateHandle;					//VME-Bus Object
	int m_cycles;						//Number of measurements Points
	int m_rate;						//Time between measurements
	int m_active;						//Scaler active
	vector<int> m_chanels;					//Store and show which channel stored. Length: 0-15
	u_int32_t chHex;					//Variable to write to the VME bus
	u_int32_t m_scalerAdr;					//Scaler hex adress
	int32_t  currenttime, previoustime; 			//For timing
	vector<double> m_rates;					//Storage of the Scaler
	string m_path;						//Path for storage
	char m_exc;						//keyboard handling

};	

#endif

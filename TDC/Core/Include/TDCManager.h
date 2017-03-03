#ifndef _TDCManager_H_
#define _TDCManager_H_


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

class TDCManager: public Common
{
 public:
	TDCManager();
   	virtual ~TDCManager();
   	int Init();

	void SetDiscAddress(string address);
	void SetCrateHandle(int handle){m_CrateHandle=handle;}
	void SetPath(string path){m_path=path;}
	int test();

 private:
   	u_int32_t *buffer;					//Buffer for the event
	int blt_bytes, pnt, Size, cnt, wavecnt, CurrentChannel, nb, ret;
	u_int32_t StringToHex(const string &str);
	int m_CrateHandle;					//VME-Bus Object
	u_int32_t chHex;					//Variable to write to the VME bus
	u_int32_t m_TDCAdr;					//Scaler hex adress
	int32_t  currenttime, previoustime; 			//For timing
	string m_path;						//Path for storage
	char m_exc;						//keyboard handling
};	

#endif

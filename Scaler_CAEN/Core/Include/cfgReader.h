#ifndef _CFGREADER_H_
#define _CFGREADER_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "CAENVMElib.h"
#include <vector>
#include <memory>
#include <typeinfo>

using namespace std;


class cfgReader
{
 public:
	cfgReader(const char *CfgName_);
	virtual ~cfgReader();
	
	bool ReadMainCfg(const char *CfgName_);
	void ReadConfigIni();
	int ReadThrFile();
	
	int GetfCrateHandle();
	
	int m_fCrateHandle;
	vector<int> m_chanels;
	vector<u_int32_t> m_thresholds;
	vector<vector<u_int32_t>> m_thresholds2d;
	
	
	u_int32_t m_fVMEAddressSca;
	string m_outputRootDir;
	
private:
	
	u_int32_t StringToHex(const string &str);
	u_int32_t StringToInt(const string &str);

	
	
	
	string m_cfgIni;
	string m_thresholdFile;
	vector<double> m_voltSet;
	vector<u_int32_t>  m_discAdr;
	int m_DCevents;
	
	
	u_int32_t m_fVMEAddressDiscriminator;	
	u_int32_t m_fLink;
	

}


#endif

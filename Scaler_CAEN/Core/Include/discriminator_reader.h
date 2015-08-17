#ifndef _DISCRIMINATOR_H_
#define _DISCRIMINATOR_H_


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "CAENVMElib.h"
#include <vector>
#include <memory>
#include <typeinfo>
#include "cfgReader.h"


#include "TROOT.h"
#include <TApplication.h>
#include "TSystem.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TRint.h"
#include "TApplication.h"

using namespace std;



class discriminator
{
 public:
	discriminator(char *CfgName_);
   virtual ~discriminator();
   


   //All functions 0=success negative number=fail
   //void ReadConfigIni();
   ////

	//bool ReadMainCfg();
	int GetfCrateHandle();
	u_int32_t GetVMEAddressSca();
	char* GetRootName(int thrNumber_);
	string GetOutputDrName();
	void DarkCount2Root(int thrNumber_);
	
	int32_t get_time();
	int GetUnixTime();
   
   ////
   //int ReadThrFile(const char* filename_);
   
   //int ReadThrFile();
   void EnableChanels();
   int FillThresholds(int thrMin_, int thrMax_);

   int SetThresholds();
   int SetThresholds2(int Nthrs_);
   int SetThresholds2();

   int FillChanels(int chMin_, int chMax_);
  
   int GetNthresholds();

   static void DarkCountToRoot(string& rootName_);


   char* GetHex(int num);
    ////
   int Test();
   std::string    GetTimeString();
   int       Close();
   
   
   
 private:
 ////
	cfgReader *m_reader;
	string m_outputRootDir;
	//char *m_mainCfgName;
	//int m_fCrateHandle;
	
/////		
	
	vector<int> m_chanels;
	vector<u_int32_t> m_thresholds;
	vector<vector<u_int32_t>> m_thresholds2d;

	u_int32_t GetHex2(int num);
	//u_int32_t StringToInt(const string &str);
	//u_int32_t StringToHex(const string &str);
	//u_int32_t fVMEAddressDiscriminator;
	//int fCrateHandle;
	//u_int32_t fLink;
};

#endif

#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>

#include "scaler.h"

scaler::scaler()
{
   fVMEAddressController=fVMEAddressScaler=fCrateHandle=fChannelMask=fLink=0;
}

scaler::~scaler()
{
}

int scaler::ReadConfigFile(string filename)
{
   ifstream infile;
   infile.open(filename.c_str());
   int i=0;

   while(i<4)  {
      i++;
      string line;
      getline(infile,line);
      istringstream iss(line);
      vector <string> words;
      copy(istream_iterator<string>(iss),istream_iterator<string>(),
	  back_inserter<vector<string> >(words));
      if(words.size()==0) return -1;
      if(words[0] == "CONTROLLER_ADDRESS")	{
	  if(words.size()<2){
       cout<<"Didn't find any CONTROLLER_ADDRESS"<<endl;  
	   return -1;
     }
	 fVMEAddressController = StringToHex(words[1]);	
     cout<<"CONTROLLER_ADDRESS= "<<hex<<fVMEAddressController<<endl;         
      }
      if(words[0] == "SCALER_ADDRESS")      	{	   
	 if(words.size()<2){
       cout<<"Didn't find any SCALER_ADDRESS"<<endl; 
	   return -1;
     }
	 fVMEAddressScaler = StringToHex(words[1]);
	 cout<<"SCALER ADDRESS= "<<hex<<fVMEAddressScaler<<endl;
      }
      if(words[0] == "CHANNEL_MASK") 	{	   
	 if(words.size()<2){
       cout<<"Didn't find any CHANNEL MASK"<<endl;   
	   return -1;}
	 fChannelMask = StringToHex(words[1]);
      cout<<"Channel Mask= "<<hex<<fChannelMask<<endl;
      }
      if(words[0] == "VMELink")	{
	 if(words.size()<2)
	   continue;
	  fLink=StringToInt(words[1]);
      cout<<"fLink= "<<fLink<<endl;
      }      
   }   

   if(fVMEAddressController==0 || fVMEAddressScaler==0 || fChannelMask==0){
       cout<<"Control failed!"<<endl;  
       return -1;
   }
   
   if(CAENVME_Init(cvV2718,fLink,0,&fCrateHandle)!=cvSuccess) {
     cout<<"CAENVME_Init failed!!!"<<endl;
     return -1;
   }
 
   return 0;
}

u_int32_t scaler::StringToHex(const string &str)
{
   stringstream ss(str);
   u_int32_t result;
   return ss >> std::hex >> result ? result : 0;
}

u_int32_t scaler::StringToInt(const string &str)
{
   stringstream ss(str);
   u_int32_t result;
   return ss >> result ? result : 0;
}

int scaler::Reset()
{
   u_int16_t data = 1;
   if(CAENVME_WriteCycle(fCrateHandle,fVMEAddressScaler+0x50,&data,
			 cvA24_U_DATA,cvD16)!=cvSuccess)
     return -1;
   if(CAENVME_WriteCycle(fCrateHandle,fVMEAddressScaler+0x0A,&data,
			 cvA24_U_DATA,cvD16)!=cvSuccess)
     return -1;
   return 0;
}

int scaler::ReadChannel(int channel)
{
   if(((fChannelMask>>channel)&1)==0) return -1;
   int data;
   if(CAENVME_ReadCycle(fCrateHandle,fVMEAddressScaler+0x10+(channel*0x4),&data,
			cvA24_U_DATA,cvD32)!=cvSuccess)
     return -1;
   return (data&0xCFFFFF);
}

int scaler::Test()
{
    u_int16_t data = 1;
    if(CAENVME_WriteCycle(fCrateHandle,fVMEAddressScaler+0x56,&data,cvA24_U_DATA,cvD16)!=cvSuccess)
      return -1;
    return 0;
}

int scaler::ReadFW()
{
  int data;
   if(CAENVME_ReadCycle(fCrateHandle,fVMEAddressScaler+0xFC,&data,
			cvA24_U_DATA,cvD16)!=cvSuccess)
     return -1;
   return data;//(data&0xCFFFFF);
}
int scaler::Close()
{
   if(CAENVME_End(fCrateHandle)!=cvSuccess)
     return -1;
   return 0;
}

string scaler::GetTimeString()
{
  time_t rawtime;
  struct tm *timeinfo;
  char timestring[25];
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  strftime(timestring,25,"%Y.%m.%d [%H:%M]",timeinfo);
  string retstring(timestring);
  return retstring;
}

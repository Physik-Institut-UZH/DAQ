#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>

#include "scaler.hh"

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
     int i =1;
   //while(!infile.eof())  {		     
   while (i<5){
   i++;
     std::cout << i << std::endl;
      string line;
      getline(infile,line);
      istringstream iss(line);
      vector <string> words;
      copy(istream_iterator<string>(iss),istream_iterator<string>(),
	   back_inserter<vector<string> >(words));
      if(words.size()==0) return -1;
      if(words[0] == "CONTROLLER_ADDRESS")	{
	 if(words.size()<2)  
	   return -1;
	 fVMEAddressController = StringToHex(words[1]);	
	 cout<<"CONTROLLER_ADDRESS "<<hex<<fVMEAddressController<<endl;
      }
      if(words[0] == "SCALER_ADDRESS")      	{	   
	 if(words.size()<2)
	   return -1;
	 fVMEAddressScaler = StringToHex(words[1]);
	 cout<<"SCALER_ADDRESS "<<hex<<fVMEAddressScaler<<endl;
      }
      if(words[0] == "CHANNEL_MASK") 	{	   
	 if(words.size()<2)
	   return -1;
	 fChannelMask = StringToHex(words[1]);
 cout<<"CHANNEL_MASK "<<hex<<fChannelMask<<endl;
      }
      if(words[0] == "VMELink")	{
	 if(words.size()<2)
	   continue;
	 fLink=StringToInt(words[1]);
	 cout<<"VMELink "<<hex<<fLink<<endl;}
            
   }

   cout<<"I am going to do the another couple of checks!"<<endl;
   if(fVMEAddressController==0 || fVMEAddressScaler==0 || fChannelMask==0){
     cout<<"Not Enough Arguments"<<endl;
     return -1;}
   if(CAENVME_Init(cvV2718,fLink,0,&fCrateHandle)!=cvSuccess){
     cout<<"VME CONNECTION PROBLEM!"<<endl;
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
			 cvA24_U_DATA,cvD16)!=cvSuccess){
     cout << "First Reset Failed" << endl;
   return -1;}

   if(CAENVME_WriteCycle(fCrateHandle,fVMEAddressScaler+0x0A,&data,
			 cvA24_U_DATA,cvD16)!=cvSuccess){
	cout << "Second Reset Failed" << endl;	
     	return -1;}
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

// returns time since start of UNIX
int scaler::GetUnixTime(){

    time_t current_time;
    current_time = time(NULL);
    return current_time;
}

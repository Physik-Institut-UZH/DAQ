#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <string>
#include "discriminator.h"



discriminator::discriminator()
{
	//ReadMainCfg(CfgName_);
	fVMEAddressDiscriminator=fCrateHandle=fLink=0;
}

discriminator::~discriminator()
{
}


int discriminator::FillThresholds(int thrMin_, int thrMax_){
	for(int i=thrMin_; i<=thrMax_; i++){
		m_thresholds.push_back(i);
	}
	return m_thresholds.size();
}

int discriminator::FillChanels(int chMin_, int chMax_){
        for(int i=chMin_; i<=chMax_; i++){
                m_chanels.push_back(i);
        }
        return m_chanels.size();

}


int discriminator::ReadCfg(const char* fname_){
	ifstream file(fname_,ios::in);
	//cfg = fopen(fname_, "r");
	if(file.good()){
		string str;
		while(getline(file, str)){
			istringstream ss(str);
	       		int num =0, count = 0;
			vector<u_int32_t> thrBuf;
	        	while(ss >> num){     
				//cout << "count" << count << endl;   	
				if(count==0)
					m_chanels.push_back(num);
				else
					thrBuf.push_back(num);
		                count++;
			}
			m_thresholds2d.push_back(thrBuf);
        	}
			
	}else{
		cout << "Error reading cfg file" << endl;
		return 0;
	}
//	fclose(cfg);
	for(int i=0; i<m_chanels.size(); i++){
		cout << "chanel" << "\t" << m_chanels[i] <<  endl;
		cout << "thresholds:" << endl;
		for(int j=0; j<m_thresholds2d[i].size(); j++){
			cout << m_thresholds2d[i][j] << endl;
		}
	}

	//Enable All Channels
        u_int32_t chHex =0xFFFF;
        if(CAENVME_WriteCycle(fCrateHandle,fVMEAddressDiscriminator+0x4A, &chHex,cvA32_U_DATA,cvD16)!=cvSuccess) //0-15 
             cout << "VME write error" << endl;

	//Set Output-Width of all Channels to 40ns
	u_int32_t outHex =0xFF;
        if(CAENVME_WriteCycle(fCrateHandle,fVMEAddressDiscriminator+0x40, &chHex,cvA32_U_DATA,cvD16)!=cvSuccess) //0-7
             cout << "VME write error" << endl;

	if(CAENVME_WriteCycle(fCrateHandle,fVMEAddressDiscriminator+0x42, &chHex,cvA32_U_DATA,cvD16)!=cvSuccess) //8-15
            cout << "VME write error" << endl;

       return 1;
}



int discriminator::ReadConfigFile(string m_cfgIni){
	ifstream infile;
	infile.open(m_cfgIni);
	int i=0;
	while(i<2){
		i++;
		string line;
		getline(infile,line);
		istringstream iss(line);
		vector <string> words;
		copy(istream_iterator<string>(iss),istream_iterator<string>(),
		back_inserter<vector<string> >(words));
		if(words.size()==0)
			fCrateHandle=-1;
		if(words[0] == "SCALER_ADDRESS"){
		if(words.size()<2){
			cout<<"Didn't find any SCALER_ADDRESS"<<endl; 
			fCrateHandle=-1;
		}
		fVMEAddressDiscriminator = StringToHex(words[1]);
		cout<<"SCALER ADDRESS= " << hex << fVMEAddressDiscriminator<<endl;
		}
		if(words[0] == "VMELink"){
			if(words.size()<2)
			continue;
			fLink=StringToInt(words[1]);
			cout<<"fLink= "<<fLink<<endl;
		}
	}

	if(fVMEAddressDiscriminator==0){
		cout<<"Control failed!"<<endl;  
		fCrateHandle=-1;
	}
	//Enable Connection to VME-Controller   
	if(CAENVME_Init(cvV2718,fLink,0,&fCrateHandle)!=cvSuccess) {
		cout<<"CAENVME_Init failed!!!"<<endl;
		fCrateHandle=-1;
	}
	infile.close();
	return fCrateHandle;
}

int discriminator::GetNthresholds(){
	return m_thresholds2d[0].size();	
}

/*string discriminator::GetRootName(int thrNumber_, double &v1_, double &v2_, const char* rDataDir_){
	char name [200];
	sprintf(name, "%s\%.1f_%.1f_%dmV.root", rDataDir_, v1_, v2_, m_thresholds2d[0][thrNumber_]);
	cout << name << endl;
	
	return string(name);
}*/


string discriminator::GetRootName(int thrNumber_, double &v1_){
	char name [200];
	sprintf(name, "%.1f_%dmV.root", v1_, m_thresholds2d[0][thrNumber_]);
	//cout << name << endl;
	return string(name);
}

string discriminator::GetRootName(int thrNumber_, double &v1_, double &v2_){
	char name [200];
	sprintf(name, "%.1f_%.1f_%dmV.root", v1_, v2_, m_thresholds2d[0][thrNumber_]);
	//cout << name << endl;
	
	return string(name);
}

string discriminator::GetRootName(int thrNumber_, double &v1_, double &v2_, double &v3_){
	char name [200];
	sprintf(name, "%.1f_%.1f_%.1f_%dmV.root", v1_, v2_, v3_, m_thresholds2d[0][thrNumber_]);
	//cout << name << endl;
	
	return string(name);
}

int discriminator::SetThresholds2(int Nthr_){
        for(int i=0; i< m_chanels.size(); i++){
   	//	stringstream ss;
	//	u_int32_t data;
	//	ss << m_thresholds2d[i][Nthr_];
	//	ss >> std::hex >> data;

        	if(CAENVME_WriteCycle(fCrateHandle,fVMEAddressDiscriminator+0x00+(m_chanels[i]*0x02),&m_thresholds2d[i][Nthr_],cvA32_U_DATA,cvD16)!=cvSuccess)
				cout << "VME write error" << endl;
			else
				cout << "threshold" << "\t" << m_thresholds2d[i][Nthr_]<< "\t" << "for channel " << m_chanels[i] << " has been set"<< endl;
        }
        return 1;
}

int discriminator::SetThresholds(){
        for(int i=0; i< m_chanels.size(); i++){
	        if(CAENVME_WriteCycle(fCrateHandle,fVMEAddressDiscriminator+0x00+(m_chanels[i]*0x02),&m_thresholds2d[i][0],cvA32_U_DATA,cvD16)!=cvSuccess)
				cout << "VME write error" << endl;
			else
				cout << "threshold " << m_thresholds2d[i][0]<< "has been set"<< endl;
        }
        return 1;
}


int discriminator::Close()
{
   if(CAENVME_End(fCrateHandle)!=cvSuccess)
     return -1;
   return 0;
}

u_int32_t discriminator::StringToHex(const string &str)
{
   stringstream ss(str);
   u_int32_t result;
   return ss >> std::hex >> result ? result : 0;
}

u_int32_t discriminator::StringToInt(const string &str)
{
   stringstream ss(str);
   u_int32_t result;
   return ss >> result ? result : 0;
}




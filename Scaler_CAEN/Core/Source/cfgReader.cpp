#include "cfgReader.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "CAENVMElib.h"
#include <vector>
#include <memory>
#include <typeinfo>




cfgReader::cfgReader(const char *CfgName_){
	if(ReadMainCfg(CfgName_)){
		ReadConfigIni();
		ReadThrFile();
	}
}



bool cfgReader::ReadMainCfg(const char *CfgName_){
	ifstream infile(m_mainCfgName,ios::in);
	
	cout << "reading main cfg file..." << endl; 
	while(true){
	string line;
	getline(infile,line);
	istringstream iss(line);
	vector<string> words;
	copy(istream_iterator<string>(iss),istream_iterator<string>(),back_inserter<vector<string> >(words));
	if(words.size()==0) return false;
	if(words[0] == "ConfigIni"){
		if(words.size()<2){
			cout<<"Error: Can't find ConfigIni"<<endl; 
			return false;
		}else{
			(words[1]== "=") ? m_cfgIni = words[2] : m_cfgIni = words[1];
			cout<<"ConfigIniFile= " << m_cfgIni<<endl;
		}
	}
	if(words[0] == "ThresholdsFile"){
		if(words.size()<2){
			cout<<"Error: Can't find Thresholds files"<<endl; 
			return false;
		}else{
			(words[1]== "=") ? m_thresholdFile=words[2] : m_thresholdFile=words[1];
			
			cout<<"Threshold File= "<<m_thresholdFile<<endl;
		}
	}
	if(words[0] == "fVMEAddressSca"){
		if(words.size()<2){
			cout<<"Error: Can't find fVMEAddressSca"<<endl; 
			return false;
		}else{
			(words[1]== "=") ? m_fVMEAddressSca  = StringToHex(words[2]):  m_fVMEAddressSca  = StringToHex(words[1]);  //??????
			cout<<"fVMEAddressSca= " << hex << m_fVMEAddressSca<<endl;
		}
	}
	if(words[0] == "VoltageSetForChanels"){
		if(words.size()<2){
			cout<<"Error: Can't find voltage values"<<endl; 
			return false;
		}else{
			string buf;
			int bgn;
			(words[1]== "=") ? bgn = 2: bgn = 1;
			for(int i=bgn; i<words.size(); i++){
				buf=words[i];
				istringstream ss(buf);
				double num =0, count = 0;
				ss >> num;
				m_voltSet.push_back(num);
			}
			cout << "voltage set:" << endl; 
			for(int i = 0; i<m_voltSet.size(); i++)
				cout << m_voltSet[i] << "\t";
		}  
	}
	if(words[0] == "AdressesForDisc"){
		if(words.size()<2){
			cout<<"Error: Can't find disc adres"<<endl; 
			return false;
		}else{
			string buf, buf1;
			u_int32_t adrTmp;
			int bgn;
			(words[1]== "=") ? bgn = 2: bgn = 1;
			for(int i=bgn; i<words.size(); i++){
				buf=words[i];
				adrTmp = StringToHex(buf);
				m_discAdr.push_back(adrTmp);
			}
		}
		cout << "\n" << "AdressesForDisc" << endl;
		for(int i = 0; i<m_discAdr.size(); i++)
			cout << hex << m_discAdr[i]<<endl;
	}
	
	if(words[0] == "DCevents"){
		if(words.size()<2){
			cout<<"Error: Can't find number of DC events"<<endl; 
			return false;
		}else{
			string buf;
			(words[1]== "=") ? buf=words[2] : buf=words[1];
			istringstream ss(buf);
			ss >> m_DCevents;
			//cout << "itoa" << endl;
			//m_DCevents = atoi(buf.c_str()); ///
			cout<<" DCevents= "<<m_DCevents<<endl;  /// why in hex?????
		}
	}
	if(words[0] == "OutputRootDir"){
		if(words.size()<2){
			cout<<"Error: Can't find OutputRootDir"<<endl; 
			return false;
		}else{
			(words[1]== "=") ? m_outputRootDir = words[2]: m_outputRootDir = words[1];
			cout<<"OutputRootDir= " << m_outputRootDir<<endl;
		}	
		break;	
	}
	//add other params to read form cfg if it's nessesary:)
	}
	cout << "closing cfg file " << endl;
	infile.close();
	return true;
}


void cfgReader::ReadConfigIni():m_fCrateHandle(0){
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
			m_fCrateHandle=-1;
		if(words[0] == "SCALER_ADDRESS"){
		if(words.size()<2){
			cout<<"Didn't find any SCALER_ADDRESS"<<endl; 
			m_fCrateHandle=-1;
		}
		m_fVMEAddressDiscriminator = StringToHex(words[1]);
		cout<<"SCALER ADDRESS= " << hex << m_fVMEAddressDiscriminator<<endl;
		}
		if(words[0] == "VMELink"){
			if(words.size()<2)
			continue;
			m_fLink=StringToInt(words[1]);
			cout<<"fLink= "<<m_fLink<<endl;
		}
	}
	if(m_fVMEAddressDiscriminator==0){
		cout<<"Control failed!"<<endl;  
		m_fCrateHandle=-1;
	}
	//Enable Connection to VME-Controller   
	if(CAENVME_Init(cvV2718,m_fLink,0,&m_fCrateHandle)!=cvSuccess) {
		cout<<"CAENVME_Init failed!!!"<<endl;
		m_fCrateHandle=-1;
	}
	
	infile.close();
}

int cfgReader::ReadThrFile(){
	ifstream file(m_thresholdFile,ios::in);
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
	for(int i=0; i<m_chanels.size(); i++){
		cout << "chanel" << "\t" << m_chanels[i] <<  endl;
		cout << "thresholds:" << endl;
		for(int j=0; j<m_thresholds2d[i].size(); j++){
			cout << m_thresholds2d[i][j] << endl;
		}
	}
	//Enable All Channels
        u_int32_t chHex =0xFFFF;
        if(CAENVME_WriteCycle(m_fCrateHandle,m_fVMEAddressDiscriminator+0x4A, &chHex,cvA32_U_DATA,cvD16)!=cvSuccess) //0-15 
             cout << "VME write error" << endl;

	//Set Output-Width of all Channels to 40ns
	u_int32_t outHex =0xFF;
        if(CAENVME_WriteCycle(m_fCrateHandle,m_fVMEAddressDiscriminator+0x40, &chHex,cvA32_U_DATA,cvD16)!=cvSuccess) //0-7
             cout << "VME write error" << endl;

	if(CAENVME_WriteCycle(m_fCrateHandle,m_fVMEAddressDiscriminator+0x42, &chHex,cvA32_U_DATA,cvD16)!=cvSuccess) //8-15
            cout << "VME write error" << endl;

       return 1;
}


int cfgReader::GetfCrateHandle(){
	return m_fCrateHandle;
}


u_int32_t cfgReader::StringToHex(const string &str)
{
   stringstream ss(str);
   u_int32_t result;
   return ss >> std::hex >> result ? result : 0;
}


u_int32_t cfgReader::StringToInt(const string &str)
{
   stringstream ss(str);
   u_int32_t result;
   return ss >> result ? result : 0;
}


#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <string>
#include "discriminator.h"
#include <sys/time.h>

//ROOT Libraries
#include "TROOT.h"
#include <TApplication.h>
#include "TSystem.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TRint.h"
#include "TApplication.h"


discriminator::discriminator(char * CfgName_): m_mainCfgName(CfgName_), m_fCrateHandle(0)
{
	fVMEAddressDiscriminator=fCrateHandle=fLink=0;
	if(ReadMainCfg()){
		ReadConfigIni();
		ReadThrFile();
	}
	
}
// returns time in milliseconds
int32_t discriminator::get_time(){
    int32_t time_ms;
    struct timeval t1;
    struct timezone tz;
    gettimeofday(&t1, &tz);
    time_ms = (t1.tv_sec) * 1000 + t1.tv_usec / 1000;
    return time_ms;
}

// returns time since start of UNIX
int discriminator::GetUnixTime(){
    time_t current_time;
    current_time = time(NULL);
    return current_time;
}

discriminator::~discriminator(){}

bool discriminator::ReadMainCfg(){
	ifstream infile;
	infile.open(m_mainCfgName);
	//int i=0;
	while(!infile.eof()){
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
				m_thresholdFile=words[1].c_str();
				cout<<"Threshold File= "<<m_thresholdFile<<endl;
			}
		}
		if(words[0] == "fVMEAddressSca"){//
			if(words.size()<2){
				cout<<"Error: Can't find fVMEAddressSca"<<endl; 
				return false;
			}else{
				
				//u_int32_t num;	        	
				(words[1]== "=") ? (istringstream ss(words[2])) : (istringstream ss(words[1]));
				ss >> m_fVMEAddressSca;
				cout<<" fVMEAddressSca= "<<m_fVMEAddressSca<<endl;
			}
		}
		if(words[0] == "VoltageSetForChanels"){
			if(words.size()<2){
				cout<<"Error: Can't find voltage values"<<endl; 
				return false;
			}else{
				string buf;
				(words[1]== "=") ? buf = words[2]: buf = words[1];
				istringstream ss(buf);
	       		double num =0, count = 0;
				cout << "voltage set:" << endl; 
	        	while(ss >> num){
					m_voltSet.push_back(num);//
					cout<<num<<endl;
				}
			}
		}
		if(words[0] == "AdressesForChanels"){
			if(words.size()<2){
				cout<<"Error: Can't find chanels adres"<<endl; 
				return false;
			}else{
				string buf, buf1;
				u_int32_t adrTmp;
				(words[1]== "=") ? buf = words[2]: buf = words[1];
				//istringstream ss(buf);
				size_t space;
				size_t bgn=0;
				while(!space==string::npos){
					space == record.find(' '); // ??????
					buf1 = buf.substr(bgn, space);
					adrTmp = StringToHex(buf1);
					m_chanelAdr.push_back(adrTmp);
					buf1.clear();
					bgn=space;
				}
			}
		}
		if(words[0] == "DCevents"){
			if(words.size()<2){
				cout<<"Error: Can't find number of DC events"<<endl; 
				return false;
			}else{
				(words[1]== "=") ? m_DCevents=words[2].c_str() : m_DCevents=words[1].c_str();
				cout<<" DCevents= "<<m_DCevents<<endl;
			}
		}
		if(words[0] == "OutputRootDir"){
			if(words.size()<2){
				cout<<"Error: Can't find OutputRootDir"<<endl; 
				return false;
			}else{
				(words[1]== "=") ? m_outputRoodDir = words[2].c_str() : m_outputRoodDir = words[1].c_str();
				cout<<"OutputRootDir= " << m_outputRoodDir<<endl;
			}
		}
		
		//add other params to read form cfg if it's nessesary:)
	}
	infile.close();
	return true;
}

void discriminator::ReadConfigIni(){
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
		m_fCrateHandle=-1;
	}
	//Enable Connection to VME-Controller   
	if(CAENVME_Init(cvV2718,fLink,0,&fCrateHandle)!=cvSuccess) {
		cout<<"CAENVME_Init failed!!!"<<endl;
		m_fCrateHandle=-1;
	}
	m_fCrateHandle = fCrateHandle;
	infile.close();
}

int discriminator::GetfCrateHandle(){
	return m_fCrateHandle;
}

u_int32_t discriminator::GetVMEAddressSca(){
	return m_fVMEAddressSca;
}
   
char * discriminator::GetOutputDrName(){
	return m_outputRootDir;
}


int discriminator::ReadThrFile(){
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

int discriminator::GetNthresholds(){
	return m_thresholds2d[0].size();	
}

char* discriminator::GetRootName(int thrNumber_){
	char *rootName = new char[200];
	char buf[20];
	for(int i=0; i<m_voltSet.size(); i++)
		snprintf(buf, 20, "%.1f_", m_voltSet[i]);
		if(i==0){
			strcpy(rootName, buf);
			
		}else
			strcat(rootName, buf);
	}
	snprintf(buf, 20, "%d", thrNumber_);
	strcat(rootName, ".root");
	
	return rootName;
}

void discriminator::DarkCount2Root(int thrNumber_){
	u_int32_t chHex =0xFFFFFF;
	//u_int32_t fVMEAddressSca=0xDDDD0000;

	char *fullName = new char[200];
	strcpy(fullName, m_outputRoodDir);
	strcat(fullName, GetRootName(thrNumber_));

	TFile* output = new TFile(fullName, "RECREATE");
	cout << "file " << fullName << "\t" << "is created" << endl;

	//Create ROOT Tree with branches

	double time=0;
	TTree* tree = new TTree("T1","");
	const int n = m_voltSet.size();
	vector<double> chIrate(n, 0.0);
	char bufName[20];
	char bufType[30];
	for(int i=0; i<n; i++){
		sprintf(bufName, "Ch%dRate", i);
		sprintf(bufType, "Ch%dRate/D", i);
		tree->Branch(bufName, &chIrate[i], bufType);	
	}
	
	tree->Branch("Time",&time,"Time/D");

	int32_t  currenttime, previoustime;
	int counter=0;

	//chHex=0x01;
	while(counter!=m_DCevents){
  	      //Clear All Counters
		if(CAENVME_WriteCycle(m_fCrateHandle,m_fVMEAddressSca+0x1122, &chHex,cvA32_U_DATA,cvD16)!=cvSuccess) //0-31
                cout << "VME write error" << endl;
        	currenttime = get_time();
        	//Read Channels of the discriminator settings
        	sleep (1);
      		int data;
       		previoustime = get_time();
       		for(int i=0; i<n; i++){
				if(CAENVME_ReadCycle(fCrateHandle,m_fVMEAddressSca+m_chanelAdr[i],&data,cvA32_U_DATA,cvD32)!=cvSuccess)
                cout << "VME read error" << endl;
                chIrate[i] = data/((previoustime-currenttime)/1000.);
			}
        	time = GetUnixTime();
        	tree->Fill();
        	counter++;
        	std::cout<<"\r                                                                                                                                                           \r";
        	//std::cout<<(double)counter/measurement*100<<"% " << "Rate_0: " << ch0rate << " Hz       " << " Rate_1: " << ch1rate << " Hz		" << " Rate_2: " << ch2rate << " Hz";
        	std::cout<<(double)counter/measurement*100<<"% " << " Rate_2: " << ch2rate << " Hz";
       		std::cout.flush();

   }
   tree->Write();
   output->Close();

}


int discriminator::SetThresholds2(int Nthr_){
        for(int i=0; i< m_chanels.size(); i++){
        	if(CAENVME_WriteCycle(fCrateHandle,fVMEAddressDiscriminator+0x00+(m_chanels[i]*0x02),&m_thresholds2d[i][Nthr_],cvA32_U_DATA,cvD16)!=cvSuccess)
				cout << "VME write error" << endl;
			else
				cout << "threshold" << "\t" << m_thresholds2d[i][Nthr_]<< "\t" << "has been set"<< endl;
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


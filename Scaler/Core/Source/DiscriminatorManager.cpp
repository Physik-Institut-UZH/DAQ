#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <string>
#include "DiscriminatorManager.h"
#include "global.h"



DiscriminatorManager::DiscriminatorManager()
{
	m_discAdr=m_CrateHandle=chHex=0;
	m_thrBuf.clear();
	m_thresholds.clear();
	m_chanels.clear();
	for(int i=0;i<16;i++)			//Set all Channels
		m_chanels.push_back(i);
}

DiscriminatorManager::~DiscriminatorManager()
{
}

int DiscriminatorManager::Init()
{

	//Enable 8-Bit Word
        chHex =0xFFFF;

	//Enable All Channels
   if(CAENVME_WriteCycle(m_CrateHandle,m_discAdr+0x4A, &chHex,cvA32_U_DATA,cvD16)!=cvSuccess){ //0-15 
      	printf(KRED);
		printf(":::: VME write error!!! (DiscriminatorManager::Init()) ::::\n");
		printf(RESET);
		return -1;
	}

	//Enable 4-Bit Word
	chHex =0xFF;

	//Set Output-Width of all Channels to 5ns
    if(CAENVME_WriteCycle(m_CrateHandle,m_discAdr+0x40, &chHex,cvA32_U_DATA,cvD16)!=cvSuccess){ //0-7
      printf(KRED);
  		printf(":::: VME write error!!! (DiscriminatorManager::Init()) ::::\n");
  		printf(RESET);
		return -1;
	}

	if(CAENVME_WriteCycle(m_CrateHandle,m_discAdr+0x42,&chHex,cvA32_U_DATA,cvD16)!=cvSuccess){ //8-15
        printf(KRED);
  		printf(":::: VME write error!!! (DiscriminatorManager::Init()) ::::\n");
  		printf(RESET);
		return -1;
	}

	return 0;
}

//Set the treshold from the XML-File
void DiscriminatorManager::SetTreshold(u_int32_t tresh){
	if(tresh==0)
		tresh=255;
	m_thrBuf.push_back(tresh);
	m_thresholds.push_back(m_thrBuf);
	m_thrBuf.clear();
	return ;
}


//Set the tresholds from the specific treshold file
int DiscriminatorManager::FillThresholds(string tresholdFile){

	m_chanels.clear();				//New Channels
	m_thresholdFile=tresholdFile;
	ifstream file(m_thresholdFile,ios::in);
	if(file.good()){
		string str;
		while(getline(file, str)){
			istringstream ss(str);
			int num =0;
			int count = 0;
	        	while(ss >> num){     
				if(count==0)
					m_chanels.push_back(num);
				else
					m_thrBuf.push_back(num);
		                count++;
			}
			m_thresholds.push_back(m_thrBuf);
			m_thrBuf.clear();
        	}
			
	}else{
		printf(KRED);
  		printf(":::: Error reading treshold file!!! (DiscriminatorManager::FillThresholds()) ::::\n");
  		printf(RESET);
		return -1;
	}

	for(int i=0; i<m_chanels.size(); i++){
		cout << endl << "	Channel" << "\t" << m_chanels[i] <<  endl;
		cout << "	thresholds: " << endl;
		for(int j=0; j<m_thresholds[i].size(); j++){
			cout << "	" << m_thresholds[i][j] << "  " ;
		}
		std::cout << std::endl;
	}

	
	return m_thresholds.size();
}


//Set the treshold to the device
int DiscriminatorManager::SetThresholdsDisc(int Nthr){
		printf(KGRN);
  		printf(":::: Message from DiscriminatorManager (SetThresholdsDisc)::::\n");
        for(int i=0; i< m_chanels.size(); i++){
		u_int8_t data = m_thresholds[i][Nthr];
        	if(CAENVME_WriteCycle(m_CrateHandle, m_discAdr+(m_chanels[i]*0x02),&data,cvA32_U_DATA,cvD16)!=cvSuccess){
				printf(KRED);
				printf(":::: VME write error!!! (DiscriminatorManager::SetThresholdsDisc()) ::::\n");
				printf(RESET);
				return -1;
			}
			else
				cout << "	Threshold:" << "\t" << m_thresholds[i][Nthr]<< " mV"<< "\t" << "for channel " << m_chanels[i] << " has been set" << endl;
			}
        printf(RESET);
        std::cout << std::endl;
        return 1;
}

//Set Address of the discriminator
void DiscriminatorManager::SetDiscAddress(string address){
	m_discAdr=StringToHex(address);
	return;
}

//Convert String to Hex
u_int32_t DiscriminatorManager::StringToHex(const string &str)
{
   stringstream ss(str);
   u_int32_t result;
   return ss >> std::hex >> result ? result : 0;
}










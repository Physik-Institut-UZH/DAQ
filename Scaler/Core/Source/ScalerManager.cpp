#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <string>
#include "ScalerManager.h"
#include "global.h"
#include <sys/time.h>
#include <unistd.h>
#include "keyb.h"

ScalerManager::ScalerManager()
{
	m_scalerAdr=m_CrateHandle=chHex=m_Tactiv=m_global=0;
	m_chanels.clear();
	m_rates.clear();
}

ScalerManager::~ScalerManager()
{
}

int ScalerManager::Init()
{		
		//32Bit Word
		chHex =0xFFFFFF;

		//Enable all channels
		if(CAENVME_WriteCycle(m_CrateHandle,m_scalerAdr+0x1100, &chHex,cvA32_U_DATA,cvD32)!=cvSuccess){ //0-31
			printf(KRED);
			printf(":::: VME write error!!! (ScalerManager::Init()) ::::\n");
			printf(RESET);
			return -1;
		}

	return 0;
}

//Set Address of the Scaler
void ScalerManager::SetDiscAddress(string address){
	m_scalerAdr=StringToHex(address);
	return;
}

//Convert String to Hex
u_int32_t ScalerManager::StringToHex(const string &str)
{
   stringstream ss(str);
   u_int32_t result;
   return ss >> std::hex >> result ? result : 0;
}

//Set the channel active or not
void ScalerManager::SetActive(int active){
	m_chanels.push_back(active);
	return ;
}

int ScalerManager::ReadMultipleCycles(){
	
			printf(KGRN);
  		printf(":::: Message from ScalerManager (ReadMultipleCycles)::::\n");
        for(int i=0; i< m_chanels.size(); i++){
				if(m_chanels[i]==1)
					cout << "	Save:" << "\t" << "Channel " << i << endl;
		}
		cout << endl;
		printf(RESET);
		
		if(m_Tactiv==0)
			ioManager = new IOManager(m_path);
		else
			ioManager = new IOManager(m_path,m_global);
		
		int counter=0;
		while(counter!=m_cycles){
			
			m_rates.clear();
			//Clear All Counters
			if(CAENVME_WriteCycle(m_CrateHandle,m_scalerAdr+0x1122, &chHex,cvA32_U_DATA,cvD16)!=cvSuccess){ //0-31
				printf(KRED);
				printf(":::: VME write error!!! (ScalerManager::ReadMultipleCycles()) ::::\n");
				printf(RESET);
				return -1;
			}
			
			currenttime = get_time();
			
			//Read Channels of the discriminator settings
	        	sleep (m_rate);
			for(int i=0; i< m_chanels.size(); i++){
					int data;
					if(m_chanels[i]==1){
				    	if(CAENVME_ReadCycle(m_CrateHandle,m_scalerAdr+0x1000+(i*0x04),&data,cvA32_U_DATA,cvD32)!=cvSuccess){
							printf(KRED);
							printf(":::: VME read error!!! (ScalerManager::ReadMultipleCycles()) ::::\n");
							printf(RESET);
							return -1;
						}	
						previoustime = get_time();
						m_rates.push_back(data/((previoustime-currenttime)/1000.));
					}
			}
			
			//FillContainer and check for abort
			ioManager->FillContainer(m_chanels,m_rates, counter);

			if (kbhit()) m_exc = getch();
           		if (m_exc == 'q') {ioManager->SaveContainer(); m_exc =0; return 1;}

			std::cout << std::endl << std::endl;
			counter++;
	}
	delete ioManager;	
		m_global++;
        std::cout << std::endl;
        return 1;
}

void ScalerManager::Save(){
		ioManager->SaveContainer();
}




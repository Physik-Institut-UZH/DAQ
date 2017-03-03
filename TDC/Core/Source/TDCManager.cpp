#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <string>
#include "TDCManager.h"
#include "IOManager.h"
#include "global.h"
#include <sys/time.h>
#include <unistd.h>
#include "keyb.h"
#include <bitset>

TDCManager::TDCManager()
{
	m_TDCAdr=m_CrateHandle=0;
}

TDCManager::~TDCManager()
{
}

int TDCManager::Init()
{		


	return 0;
}

//Set Address of the Scaler
void TDCManager::SetDiscAddress(string address){
	m_TDCAdr=StringToHex(address);
	return;
}


//Convert String to Hex
u_int32_t TDCManager::StringToHex(const string &str)
{
   stringstream ss(str);
   u_int32_t result;
   return ss >> std::hex >> result ? result : 0;
}


int TDCManager::test()
{
 		  // Get system informations
    
       		UINT16 firmware_rev;
      		UINT8  piggy_back_type;
        	UINT16 serial_number;
		int data;
		if(CAENVME_ReadCycle(m_CrateHandle,m_TDCAdr+0x100E, &data,cvA32_U_DATA,cvD16)!=cvSuccess){
			printf(KRED);
			printf(":::: VME read error!!! (ScalerManager::ReadMultipleCycles()) ::::\n");
			printf(RESET);
			return -1;
		}	

		//Firmware

		if(CAENVME_ReadCycle(m_CrateHandle,m_TDCAdr+0x1000, &data,cvA32_U_DATA,cvD16)!=cvSuccess){
			printf(KRED);
			printf(":::: VME read error!!! (ScalerManager::ReadMultipleCycles()) ::::\n");
			printf(RESET);
			return -1;
		}	
		
		//Bit Set 1 Register
		if(CAENVME_ReadCycle(m_CrateHandle,m_TDCAdr+0x1006, &data,cvA32_U_DATA,cvD16)!=cvSuccess){
			printf(KRED);
			printf(":::: VME read error!!! (ScalerManager::ReadMultipleCycles()) ::::\n");
			printf(RESET);
			return -1;
		}

		
		//Status Register 1
		if(CAENVME_ReadCycle(m_CrateHandle,m_TDCAdr+0x100E, &data,cvA32_U_DATA,cvD16)!=cvSuccess){
			printf(KRED);
			printf(":::: VME read error!!! (ScalerManager::ReadMultipleCycles()) ::::\n");
			printf(RESET);
			return -1;
		}		
		 std::cout<<std::bitset<16>(data)<<std::endl;

		//Control Register 1 
		if(CAENVME_ReadCycle(m_CrateHandle,m_TDCAdr+0x1010, &data,cvA32_U_DATA,cvD16)!=cvSuccess){
			printf(KRED);
			printf(":::: VME read error!!! (ScalerManager::ReadMultipleCycles()) ::::\n");
			printf(RESET);
			return -1;
		}		
		 std::cout<<std::bitset<16>(data)<<std::endl;



		// read the event
       	int nb, ret;  
       	   
      	// Read data from module i in MBLT mode into buff     
	blt_bytes=0; 
    	ret = CAENVME_FIFOBLTReadCycle(m_CrateHandle,m_TDCAdr, ((unsigned char*)buffer)+blt_bytes, (1024*1024), cvA32_U_BLT, cvD32, &nb);
	if ((ret != cvSuccess) && (ret != cvBusError)) {
		std::cout << "Block read error" << std::endl;   
		printf("%d bytes read\n",nb);
		return -1;  
    	}
    	blt_bytes += nb;
    		//	if (blt_bytes > m_BufferSize) { 
		//	std::cout << "Negativ bytes transfered" << std::endl;
		//	return -1;  
    		  // }
  	std::cout <<  blt_bytes << std::endl;

   	return  blt_bytes;

}





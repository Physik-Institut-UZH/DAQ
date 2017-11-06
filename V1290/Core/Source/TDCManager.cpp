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


int TDCManager::test(){

		int data;
		if(CAENVME_ReadCycle(m_CrateHandle,m_TDCAdr+0x1002, &data,cvA32_U_DATA,cvD16)!=cvSuccess){
			printf(KRED);
			printf(":::: VME read error!!! (ScalerManager::ReadMultipleCycles()) ::::\n");
			printf(RESET);
			return -1;
		}	

		std::cout<<std::bitset<16>(data)<<std::endl;
   		int m_MemorySize, m_BufferSize;
		u_int32_t m_ExpectedEvSize;					//Complete Eventsize

	    	// Expected Event Size in Words (32Bit including Header)
	   	m_BufferSize = 524288;

		// allocate memory for buffer
	    	if ( (buffer = (u_int32_t*)malloc(m_BufferSize)) == NULL) {  
			printf(KRED);
			printf(":::: ERROR: Can't allocate memory buffer of %d kB ::::", m_BufferSize/1024);
			printf(RESET);
			return -1;
	  	}

		struct timeval m_begin, m_end, m_total;	
   		double m_time, m_seconds, m_useconds;    			//mili, mikro and mili seconds
		gettimeofday(&m_begin, NULL);
  		gettimeofday(&m_total, NULL);

		// read the event
      	 	int nb, ret;  
		int  m_events=0;
		int m_lastevents=0;
       	   	while(1==1){
			blt_bytes=0;
   			do { 
   			ret = CAENVME_BLTReadCycle(m_CrateHandle, m_TDCAdr, 
			((unsigned char*)buffer)+blt_bytes, 524288, cvA32_U_BLT, cvD32, &nb);
    			if ((ret != cvSuccess) && (ret != cvBusError)) {
				std::cout << "Block read error" << std::endl;   
				printf("%d bytes read\n",nb);
				return -1;  
    			}
    			blt_bytes += nb;
  			} while (ret != cvBusError); 
			 m_events++;

			gettimeofday(&m_end, NULL);
			m_seconds  = m_end.tv_sec  - m_begin.tv_sec;
			m_useconds = m_end.tv_usec - m_begin.tv_usec;
			m_time = ((m_seconds) * 1000 + m_useconds/1000.0) + 0.5;
		
		 if(m_time>1000){
			/* print some progress indication */
			printf(KCYN);
			std::cout << " DAQ-Rate: " << round(((m_events-m_lastevents)/m_time)*1000)<< " Hz " << " Total Events: " << m_events << std::endl;
			gettimeofday(&m_begin, NULL);
			printf(RESET);
			m_lastevents=m_events;
		}
			//std::cout << blt_bytes << std::endl;
		}





/*
 		  // Get system informations
    
       		//UINT16 firmware_rev;
      		//UINT8  piggy_back_type;
        	//UINT16 serial_number;
		int data;
		if(CAENVME_ReadCycle(m_CrateHandle,m_TDCAdr+0x100E, &data,cvA32_U_DATA,cvD16)!=cvSuccess){
			printf(KRED);
			printf(":::: VME read error!!! (ScalerManager::ReadMultipleCycles()) ::::\n");
			printf(RESET);
			return -1;
		}	

		//Firmware
		short firmware;
		if(CAENVME_ReadCycle(m_CrateHandle,m_TDCAdr+0x1000, &firmware,cvA32_U_DATA,cvD16)!=cvSuccess){
			printf(KRED);
			printf(":::: VME read error!!! (ScalerManager::ReadMultipleCycles()) ::::\n");
			printf(RESET);
			return -1;
		}	
		std::cout<<std::hex << (firmware)<<std::endl;
		
		//Bit Set 1 Register
		if(CAENVME_ReadCycle(m_CrateHandle,m_TDCAdr+0x1006, &data,cvA32_U_DATA,cvD16)!=cvSuccess){
			printf(KRED);
			printf(":::: VME read error!!! (ScalerManager::ReadMultipleCycles()) ::::\n");
			printf(RESET);
			return -1;
		}
		std::cout<<std::bitset<8>(data)<<std::endl;
		
		//Status Register 1
		if(CAENVME_ReadCycle(m_CrateHandle,m_TDCAdr+0x100E, &data,cvA32_U_DATA,cvD16)!=cvSuccess){
			printf(KRED);
			printf(":::: VME read error!!! (ScalerManager::ReadMultipleCycles()) ::::\n");
			printf(RESET);
			return -1;
		}		
		 std::cout<< "Status Register:" << std::bitset<9>(data)<<std::endl;

		data=pow(2,2)+pow(2,5)+pow(2,6);
		//Write Control Register 1 
		if(CAENVME_WriteCycle(m_CrateHandle,m_TDCAdr+0x1010, &data,cvA32_U_DATA,cvD16)!=cvSuccess){
			printf(KRED);
			printf(":::: VME read error!!! (ScalerManager::ReadMultipleCycles()) ::::\n");
			printf(RESET);
			return -1;
		}		

		//Control Register 1 
		if(CAENVME_ReadCycle(m_CrateHandle,m_TDCAdr+0x1010, &data,cvA32_U_DATA,cvD16)!=cvSuccess){
			printf(KRED);
			printf(":::: VME read error!!! (ScalerManager::ReadMultipleCycles()) ::::\n");
			printf(RESET);
			return -1;
		}		
		 std::cout<< "Control Register:"<<std::bitset<7>(data)<<std::endl;

	//Status Register 2 
		if(CAENVME_ReadCycle(m_CrateHandle,m_TDCAdr+0x1022, &data,cvA32_U_DATA,cvD16)!=cvSuccess){
			printf(KRED);
			printf(":::: VME read error!!! (ScalerManager::ReadMultipleCycles()) ::::\n");
			printf(RESET);
			return -1;
		}		
		 std::cout<< "Status Register2:" <<std::bitset<8>(data)<<std::endl;

	//Event Counter Low Register 
		if(CAENVME_ReadCycle(m_CrateHandle,m_TDCAdr+0x1024, &data,cvA32_U_DATA,cvD16)!=cvSuccess){
			printf(KRED);
			printf(":::: VME read error!!! (ScalerManager::ReadMultipleCycles()) ::::\n");
			printf(RESET);
			return -1;
		}		
		 std::cout<<std::bitset<8>(data)<<std::endl;

	//Event Counter High Register 
		if(CAENVME_ReadCycle(m_CrateHandle,m_TDCAdr+0x1026, &data,cvA32_U_DATA,cvD16)!=cvSuccess){
			printf(KRED);
			printf(":::: VME read error!!! (ScalerManager::ReadMultipleCycles()) ::::\n");
			printf(RESET);
			return -1;
		}		
		 std::cout<<std::bitset<8>(data)<<std::endl;
		

	//Event Counter High Register 
		if(CAENVME_ReadCycle(m_CrateHandle,m_TDCAdr+0x1032, &data,cvA32_U_DATA,cvD16)!=cvSuccess){
			printf(KRED);
			printf(":::: VME read error!!! (ScalerManager::ReadMultipleCycles()) ::::\n");
			printf(RESET);
			return -1;
		}	
			 std::cout<<std::bitset<16>(data)<<std::endl;



   	return  blt_bytes;
*/
}





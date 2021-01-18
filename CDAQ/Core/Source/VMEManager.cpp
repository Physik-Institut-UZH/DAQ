#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <string>
#include "VMEManager.h"
#include "global.h"

/*
Author: Julien Wulf UZH
*/


VMEManager::VMEManager()
{
	m_CrateHandle= m_Link=0;
}

VMEManager::~VMEManager()
{
}

int VMEManager::Init(int PCIType){

  if(PCIType == 0){
    if(CAENVME_Init(cvV2718,m_Link,m_board,&m_CrateHandle)!=cvSuccess) {
      printf(KRED);
      printf(":::: CAENVME_Init failed!!! (VMEManager) ::::\n");
      printf(RESET);
    m_CrateHandle=-1;
    return -1;
    }
  }
  else if(PCIType == 1){
    //TODO, make this clean
    CAEN_DGTZ_ErrorCode m_Ret = CAEN_DGTZ_Success;
    //Address needs to be zero if optical link is used (IDK maybe it can be any value)
    if(m_ConnectionType == CAEN_DGTZ_OpticalLink){
      m_address = 0;
    }
    //m_Ret = CAEN_DGTZ_OpenDigitizer(CAEN_DGTZ_OpticalLink, m_Link , m_board, m_address, &m_CrateHandle);
    //TODO
    m_Ret = CAEN_DGTZ_OpenDigitizer(m_ConnectionType, m_Link , m_board, m_address, &m_CrateHandle);
    if (m_Ret) {
      printf(KRED);
      printf(":::: CAENVME_Init failed!!! (VMEManager) ::::\n");
      printf(RESET);
      m_CrateHandle=-1;
      return -1;
    }
    return 0;
  }

  return 0;
}

int VMEManager::Init(){

  if(CAENVME_Init(cvV2718,m_Link,m_board,&m_CrateHandle)!=cvSuccess) {
    printf(KRED);
    printf(":::: CAENVME_Init failed!!! (VMEManager) ::::\n");
    printf(RESET);
    m_CrateHandle=-1;
    return -1;
  }

  return 0;
}
int VMEManager::Close(){

   if(CAENVME_End(m_CrateHandle)!=cvSuccess)
     return -1;
   return 0;
}





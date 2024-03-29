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





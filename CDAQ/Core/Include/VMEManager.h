#ifndef _VMEMANAGER_H_
#define _VMEMANAGER_H_


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "CAENVMElib.h"
#include "CAENDigitizer.h"
#include <vector>
#include <memory>
#include <typeinfo>

using namespace std;

/*
Author: Julien Wulf UZH
Class to manage the connection to the VME-Bus/Crate and Controller. 
*/


class VMEManager
{
 public:
   VMEManager();
   virtual ~VMEManager();
   
   //Function to establish and delete connection returns -1 if it fails
   int Init(int num);
   int Init();
   int Close();

   //Set und Get Functions
   void SetPCILink(u_int32_t link){m_Link=link;};
   void SetBoardNumber(u_int32_t number){m_board=number;};
   void SetAddress(string address) {m_address=StringToHex(address);};
   void SetConnectionType(CAEN_DGTZ_ConnectionType type) {m_ConnectionType = type;};
   int GetCrateHandle(){return m_CrateHandle;};

   u_int32_t StringToHex(const string &str){
    stringstream ss(str);
    u_int32_t result;
    return ss >> std::hex >> result ? result : 0;
   }
 private:
   int m_CrateHandle;					//Object to the VME Bus
   u_int32_t m_Link;					//Linknumber of the PCI card
   u_int32_t m_board;					//Board number in the fiber link
   u_int32_t m_address;       //Board Address // should be zero if using optical link
   CAEN_DGTZ_ConnectionType m_ConnectionType;
};

#endif


#ifndef _STORAGEMANAGER_H_
#define _STORAGEMANAGER_H_


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <memory>
#include <typeinfo>
#include "common.h"
#include "xmlParser.h"
#include "CAENDigitizer.h"

//ROOT Libraries
#include "TROOT.h"
#include "TSystem.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TRint.h"

//Multithreading
#include <pthread.h>


using namespace std;

/*
Author: Julien Wulf UZH
Class to save the output of the ADC
*/


class StorageManager: public Common
{
public:
	StorageManager();
	virtual ~StorageManager();

    	//Init Function
	int Init();

	int InitROOT();
	int InitROOTZLE();
	//Set Function
	void SetBuffer(u_int32_t *adcBuffer){buffer=adcBuffer;}
  //TODO SetEventLength is a outdated
  void SetEventLength(int temp){;};
	void SetTransferedBytes(int bytes){blt_bytes=bytes;}
	void SetXMLFile(char* file){m_XmlFileName=file;}
	void SetFolderName(char* file){m_OutputFolder=file;}
	void SetModuleNumber(int i){m_module=i;}
	void SetModuleFolder(string message){m_moduleName=message;}

  //Added by Neil
  void SetEnableMask(uint16_t mask){m_EnableMask = mask;}
  void Set16BitEvent(CAEN_DGTZ_UINT16_EVENT_t *evnt) {Event16 = evnt;}
  void SetBufferSize(u_int32_t buff){m_BufferSize = buff;}
  void SetEventHeaderInfo(int samples, double freq, double volt,double res)
  {
    m_BufferSize = samples;
    m_sampleFreq = freq;
    m_vRange = volt;
    m_resDAC = res;
  };
	
	//Get Function
	int GetNumberEvents(){return m_NoE;} 
	int GetEventsPerFile(){return m_EventsPerFile;}

	//Function for the container
	int FillContainer();
	int SaveContainer();
	int NewFile();

   
private:

  //int FillROOTContainer();
  bool FillROOTContainer();//Created by Neil
	void SaveROOTContainer();
  //TODO fix ZLE
	int FillZLEROOTContainer() {return 1;};
	double m_time;							//Storage of the current time
    string m_command;						//tmp variable
	string m_path;							//Path for storage
	string m_moduleName;					//Folder for the differen Modules
	TFile* output;							//ROOT File
	TTree* tree;							//ROOT Tree
    char* m_OutputFolder;					//Output FileName
	u_int32_t* buffer;						//Buffer of the data to visualize
	uint32_t blt_bytes,pnt,Size,cnt,wavecnt, CurrentChannel;     //readout the channel

  std::vector<int*> wfVec;
 	char* m_XmlFileName;					//XML-File
 	int ApplyXMLFile();						//Read XML-Config for the Storage Manager
 	int m_NoE,m_EventsPerFile,m_WriteToFile,m_nbCh,m_filenumber,m_module;			//Number of events
	int m_custom_size;
 	int *channelActive;						//Channel active
	int m_ZLE;							//ZLE active or not
  
  std::vector<std::vector<int>> m_zle_wfVec;
  std::vector<std::vector<int>> m_zle_cwVec;

  uint16_t  m_EnableMask;
  CAEN_DGTZ_UINT16_EVENT_t * Event16;

  u_int32_t m_BufferSize;
  double m_sampleFreq;
  double m_vRange;
 	int m_resDAC;										//Resolution DAC

};

#endif


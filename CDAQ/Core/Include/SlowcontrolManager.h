#ifndef _SLOWCONTROLMANAGER_H_
#define _SLOWCONTROLMANAGER_H_


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <memory>
#include <typeinfo>
#include "common.h"
#include "xmlParser.h"
#include <sys/time.h>
#include "CAENDigitizer.h"

//ROOT Libraries
#include "TROOT.h"
#include "TSystem.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TRint.h"
#include <iostream>
#include <fstream>

using namespace std;

/*
Author: Julien Wulf UZH
Class to manage the IO/Status and slowcontrol
*/


class SlowcontrolManager: public Common
{
 public:
   SlowcontrolManager();
   virtual ~SlowcontrolManager();
   
   
   
   //Init Function
   int Init();
   
   //Process comandline input
   int ProcessInput(int argc, char *argv[], char *envp[]);

	//Create a copy of the xml file
   int xml_storefile();
   
   //Get Functions
   char* GetXMLFile(){return m_XmlFileName;}			//Get the xmk-file out of the storage
   int GetBaselineCalculation(){return m_baseline;}		//Baseline Calculation active
   int GetGraphicsActive(){return m_graphics;}			//Gaphicmode active
   int GetChannelNumber(){return m_pmtNb;}			    //GetPMTNumber to visualize
   int GetNumberEvents(){return m_events;}			    //Current stored number of events
   int GetTotaltNumberEvents(){return m_totalevents;}	//Total number of events to be stored
   char* GetFolderName(){return m_OutputFolder;}		//Get the outputfolder from comandline m_OutputPath
   char* GetPathName(){return  m_OutputPath;}			//Get the outputfolder from comandline m_OutputPath
   double GetCurrentUnixTime(){return GetUnixTime();}	//Get Current Unix time
   int GetADCInformation(){return m_BoardInfo;};		//Show only configuration
   string GetAddress(int i){return m_address[i];}		//Get ADC adress from xml-file
   int GetADCType(){return m_type;}				        //Which ADC is connected
   int GetNbModules(){return m_Nbmodule;}			    //How many modules in the daisy chain
   int GetLinkInChain(){return m_numberChain;}			//In which position is the Board in the chain,(ConetNode)
   int GetPCILinkNum(){return m_PCILinkNum;} //Return which PCI slot is used V3718 can have 1,2, or 4
   CAEN_DGTZ_ConnectionType GetConnectionType() {return m_ConnectionType;}
   bool UseMCA() {return m_useMCA;}
   
   //Interaction Functions
   int StartAquistion();
   int StopAquistion();
   int ShowStatus(int status=1);
   void AddBytes(int bytes){m_bytes=bytes+m_bytes;m_totalB=m_totalB+bytes/1024.;}			//m_total in Mb


 private:
	int ApplyXMLFile();			//Read XML-Config for the Storage Manager
	char m_OutputFolder[100];	//Outputfolder name (custom)
	char m_OutputPath[100];		//Outputpath name (custom)
	char m_XmlFileName[100];    //XML-Filename
	

  struct timeval m_begin, m_end, m_total;				//Timing of the DAQ 
  double m_time, m_seconds, m_useconds;    			//mili, mikro and mili seconds
  int m_lastevents; 									//Last events since call 	
  int m_events;										//Total events
  int m_totalevents;
  u_int32_t m_bytes,m_totalB;							//Total bytes
  int m_pmtNb,m_BoardInfo,m_baseline,m_errflag,m_graphics,m_Nbmodule ; 
  int m_triggertype;									//Trigger type
	string* m_address;									//ADC Address
	int m_type; //ADC type
  int m_nbCh;
  CAEN_DGTZ_ConnectionType m_ConnectionType;
  int m_numberChain;                                  //ADC number in the link
  int m_PCILinkNum;
  ofstream m_DAQStatus;								//Stores the current DAQ rate
  ofstream m_DAQSummary;								//Stores all relevant parameters
  bool m_useMCA = false;

};

#endif


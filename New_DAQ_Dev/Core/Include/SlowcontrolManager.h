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

//ROOT Libraries
#include "TROOT.h"
#include "TSystem.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TRint.h"


using namespace std;

/*
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
   int GetChannelNumber(){return m_pmtNb;}				//GetPMTNumber to visualize
   int GetNumberEvents(){return m_events;}				//Current stored number of events
   char* GetFolderName(){return m_OutputFolder;}		//Get the outputfolder from comandline
   int GetADCInformation(){return m_BoardInfo;};		//SHow only configuration
   string GetAddress(){return m_address;}				//Get ADC adress from xml-file
   int GetADCType(){return m_type;}						//Which ADC is connected
   
   
   //Interaction Functions
   int StartAquistion();
   int StopAquistion();
   int ShowStatus(int status=1);
   void AddBytes(int bytes){m_bytes=bytes+m_bytes;m_totalB=m_totalB+bytes;}


 private:
	int ApplyXMLFile();			//Read XML-Config for the Storage Manager
	char m_OutputFolder[100];	//Outputfolder name (custom)
	char m_OutputPath[100];		//Outputpath name (custom)
	char m_XmlFileName[100];    //XML-Filename
	

	struct timeval m_begin, m_end, m_total;				//Timing of the DAQ 
    double m_time, m_seconds, m_useconds;    			//mili, mikro and mili seconds
    int m_lastevents; 									//Last events since call 	
    int m_events;										//Total events
    u_int32_t m_bytes,m_totalB;							//Total bytes
	int m_pmtNb,m_BoardInfo,m_baseline,m_errflag,m_graphics,m_module ; 
	int m_triggertype;									//Trigger type
	string m_address;									//ADC Address
	int m_type;											//ADC type
  
};

#endif


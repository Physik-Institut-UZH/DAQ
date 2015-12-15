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

	//Set Function
	void SetBuffer(u_int32_t *adcBuffer){buffer=adcBuffer;} 
	void SetEventLength(int length){m_length=length;}
	void SetTransferedBytes(int bytes){blt_bytes=bytes;}
	void SetXMLFile(char* file){m_XmlFileName=file;}
	void SetFolderName(char* file){m_OutputFolder=file;}
	
	//Get Function
	int GetNumberEvents(){return m_NoE;} 
	int GetEventsPerFile(){return m_EventsPerFile;}

	//Function for the container
	int FillContainer();
	int SaveContainer();
	int NewFile();

   
private:

int FillROOTContainer();
	void SaveROOTContainer();
	double m_time;						//Storage of the current time
    string m_command;					//tmp variable
	string m_path;						//Path for storage
	TFile* output;						//ROOT File
	TTree* tree;						//ROOT Tree
    char* m_OutputFolder;			//Output FileName
	u_int32_t* buffer;					//Buffer of the data to visualize
	int m_length;						//Set length of the waveform
	int blt_bytes,pnt,Size,cnt,wavecnt, CurrentChannel;     //readout the channel
    int* wf0;							//Storage for waveform
	int* wf1;							//Storage for waveform
	int* wf2;							//Storage for waveform
 	int* wf3;							//Storage for waveform
 	int* wf4;							//Storage for waveform
 	int* wf5;							//Storage for waveform
 	int* wf6;							//Storage for waveform
 	int* wf7;							//Storage for waveform
 	char* m_XmlFileName;				//XML-File
 	int ApplyXMLFile();					//Read XML-Config for the Storage Manager
 	int m_NoE,m_EventsPerFile,m_WriteToFile,m_nbchs,m_filenumber;			//Number of events
 	int *channelActive;					//Channel active
};

#endif


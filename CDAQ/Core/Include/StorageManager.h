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
	void SetEventLength(int length){m_length=length;}
	void SetTransferedBytes(int bytes){blt_bytes=bytes;}
	void SetXMLFile(char* file){m_XmlFileName=file;}
	void SetFolderName(char* file){m_OutputFolder=file;}
	void SetModuleNumber(int i){m_module=i;}
	void SetModuleFolder(string message){m_moduleName=message;}
	
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
	int FillZLEROOTContainer();
	double m_time;							//Storage of the current time
    string m_command;						//tmp variable
	string m_path;							//Path for storage
	string m_moduleName;					//Folder for the differen Modules
	TFile* output;							//ROOT File
	TTree* tree;							//ROOT Tree
    char* m_OutputFolder;					//Output FileName
	u_int32_t* buffer;						//Buffer of the data to visualize
	int m_length;							//Set length of the waveform
	uint32_t blt_bytes,pnt,Size,cnt,wavecnt, CurrentChannel;     //readout the channel
    	int* wf0;								//Storage for waveform
	int* wf1;								//Storage for waveform
	int* wf2;								//Storage for waveform
 	int* wf3;								//Storage for waveform
 	int* wf4;								//Storage for waveform
 	int* wf5;								//Storage for waveform
 	int* wf6;								//Storage for waveform
 	int* wf7;								//Storage for waveform
 	char* m_XmlFileName;					//XML-File
 	int ApplyXMLFile();						//Read XML-Config for the Storage Manager
 	int m_NoE,m_EventsPerFile,m_WriteToFile,m_nbchs,m_filenumber,m_module;			//Number of events
	int m_custom_size; 

	
	int *channelActive;						//Channel active
	int m_ZLE;							//ZLE active or not
	std::vector <int> m_zle_wf0;
        std::vector <int> m_zle_cw0;

        std::vector <int> m_zle_wf1;
        std::vector <int> m_zle_cw1;

        std::vector <int> m_zle_wf2;
        std::vector <int> m_zle_cw2;

        std::vector <int> m_zle_wf3;
        std::vector <int> m_zle_cw3;

        std::vector <int> m_zle_wf4;
        std::vector <int> m_zle_cw4;

        std::vector <int> m_zle_wf5;
        std::vector <int> m_zle_cw5;

        std::vector <int> m_zle_wf6;
        std::vector <int> m_zle_cw6;

        std::vector <int> m_zle_wf7;
        std::vector <int> m_zle_cw7;

};

#endif


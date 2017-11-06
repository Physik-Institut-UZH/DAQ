#ifndef _IOANAGER_H_
#define _IOMANAGER_H_


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <memory>
#include <typeinfo>
#include "common.h"
#include <iostream>
#include <fstream>
//ROOT Libraries
#include "TROOT.h"
#include <TApplication.h>
#include "TSystem.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TRint.h"
#include "TApplication.h"

using namespace std;

/*
Class to manage the Input and Output.
*/


class IOManager: public Common
{
public:
	IOManager();
	IOManager(string path);
    IOManager(string path, int Filenumber);
    virtual ~IOManager();
    
    void FillContainer(vector<int> &channel, vector<double> &rates,  int &event);
    void SaveContainer();

   
private:
	ofstream m_stream;						//output into txt file
	int m_flag;							//Flag for first object call
	double m_time=0;						//Storage of the current time
	int m_event;						//Event Number
        string m_command;					//tmp variable
	vector<double> m_rates;				//Storage of the Scaler
	vector<int> m_chanels;				//Store and show which channel. Length: 0-15
	string m_path;						//Path for storage
	TFile* output;						//ROOT File
	TTree* tree;						//ROOT Tree
        char m_OutputFolder[100];
};

#endif


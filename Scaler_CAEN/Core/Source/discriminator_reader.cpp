#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <string>
#include "discriminator.h"
#include <sys/time.h>

#include "cfgReader.h"

//ROOT Libraries
#include "TROOT.h"
#include <TApplication.h>
#include "TSystem.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TRint.h"
#include "TApplication.h"


discriminator::discriminator(char * CfgName_):
{
	cffVMEAddressDiscriminator=fCrateHandle=fLink=0;
	m_reader =  new cfgReader(CfgName_);	
	m_chanels = m_reader->m_chanels;
	//vector<u_int32_t> m_thresholds=m_reader->;
	m_thresholds2d = m_reader->m_thresholds2d;
	m_outputRootDir = m_reader->m_outputRootDir;
}

discriminator::~discriminator(){}

// returns time in milliseconds
int32_t discriminator::get_time(){
    int32_t time_ms;
    struct timeval t1;
    struct timezone tz;
    gettimeofday(&t1, &tz);
    time_ms = (t1.tv_sec) * 1000 + t1.tv_usec / 1000;
    return time_ms;
}

// returns time since start of UNIX
int discriminator::GetUnixTime(){
    time_t current_time;
    current_time = time(NULL);
    return current_time;
}



int discriminator::GetfCrateHandle(){
	return m_reader->m_fCrateHandle;
}

u_int32_t discriminator::GetVMEAddressSca(){
	return m_reader->m_fVMEAddressSca;
}
   
string discriminator::GetOutputDrName(){
	return m_outputRootDir.c_str();
}

int discriminator::GetNthresholds(){
	return m_reader->m_thresholds2d[0].size();	
}

char* discriminator::GetRootName(int thrNumber_){
	char *rootName = new char[200];
	char buf[20];
	for(int i=0; i<m_voltSet.size(); i++){
		char buf2[20];
		snprintf(buf2, 20, "%.1f_", m_voltSet[i]);
		if(i==0){
			strcpy(rootName, buf2);
			
		}else
			strcat(rootName, buf2);
	}
	snprintf(buf, 20, "%d", thrNumber_);
	strcat(rootName, ".root");
	
	return rootName;
}



void discriminator::DarkCount2Root(int thrNumber_){
	u_int32_t chHex =0xFFFFFF;
	//u_int32_t fVMEAddressSca=0xDDDD0000;

	char *fullName = new char[200];
	strcpy(fullName, m_outputRootDir.c_str());
	strcat(fullName, GetRootName(thrNumber_));

	TFile* output = new TFile(fullName, "RECREATE");
	cout << "file " << fullName << "\t" << "is created" << endl;

	//Create ROOT Tree with branches

	double time=0;
	TTree* tree = new TTree("T1","");
	
	const int n = m_voltSet.size();
	
	vector<double> chIrate(n, 0.0);
	char bufName[20];
	char bufType[30];
	for(int i=0; i<n; i++){
		sprintf(bufName, "Ch%dRate", i);
		sprintf(bufType, "Ch%dRate/D", i);
		tree->Branch(bufName, &chIrate[i], bufType);	
	}
	
	tree->Branch("Time",&time,"Time/D");

	int32_t  currenttime, previoustime;
	int counter=0;

	//chHex=0x01;
	cout << "taking events" << m_DCevents << endl;
	while(counter!=m_DCevents){
  	      //Clear All Counters
		if(CAENVME_WriteCycle(m_fCrateHandle,m_fVMEAddressSca+0x1122, &chHex,cvA32_U_DATA,cvD16)!=cvSuccess) //0-31
                cout << "VME write error" << endl;
        	currenttime = get_time();
        	//Read Channels of the discriminator settings
        	sleep (1);
      		int data;
       		previoustime = get_time();
       		time = GetUnixTime();
        	tree->Fill();
        	
        	char buf[200];
       		for(int i=0; i<n; i++){
				if(CAENVME_ReadCycle(fCrateHandle,m_fVMEAddressSca+m_discAdr[i],&data,cvA32_U_DATA,cvD32)!=cvSuccess)
                cout << "VME read error" << endl;
                chIrate[i] = data/((previoustime-currenttime)/1000.);
                std::cout<<"\r                                                                                                                                                           \r";
				sprintf(buf, "%.2f % \t Rate_%d \t %lf Hz", ((double)counter/m_DCevents)*100, i, chIrate[i]);
				cout << buf;
				std::cout.flush();
			}
        	counter++;
        	
   }
   tree->Write();
   output->Close();

}


int discriminator::SetThresholds2(int Nthr_){
        for(int i=0; i< m_chanels.size(); i++){
        	if(CAENVME_WriteCycle(fCrateHandle,fVMEAddressDiscriminator+0x00+(m_chanels[i]*0x02),&m_thresholds2d[i][Nthr_],cvA32_U_DATA,cvD16)!=cvSuccess)
				cout << "VME write error" << endl;
			else
				cout << "threshold" << "\t" << m_thresholds2d[i][Nthr_]<< "\t" << "has been set"<< endl;
        }
        return 1;
}

int discriminator::Close()
{
   if(CAENVME_End(fCrateHandle)!=cvSuccess)
     return -1;
   return 0;
}



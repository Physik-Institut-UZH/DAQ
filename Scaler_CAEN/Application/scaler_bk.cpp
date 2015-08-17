// Julien W. Rizalina M.
// Program to set the threshold for the v895 Leding Edge Discriminator

//C++ Libraries
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include "discriminator.h"
#include <cmath>
#include <map>
#include <math.h>
#include <vector>
#include <sys/time.h>

//ROOT Libraries
#include "TROOT.h"
#include <TApplication.h>
#include "TSystem.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TRint.h"
#include "TApplication.h"


const int measurement = 100;


using namespace std;


// returns time in milliseconds
int32_t get_time()
{
    int32_t time_ms;

    struct timeval t1;
    struct timezone tz;

    gettimeofday(&t1, &tz);
    time_ms = (t1.tv_sec) * 1000 + t1.tv_usec / 1000;
    return time_ms;
}

// returns time since start of UNIX
int GetUnixTime(){

    time_t current_time;
    current_time = time(NULL);
    return current_time;
}



static void DarkCount2Root(const char* rootName_, int& fCrateHandle, const char* rootDir_ = 0){

	u_int32_t chHex =0xFFFFFF;
	u_int32_t fVMEAddressSca=0xDDDD0000;

	char *fullName = new char[200];
	strcpy(fullName, rootDir_);
	strcat(fullName, rootName_);

	TFile* output = new TFile(fullName, "RECREATE");
	cout << "file " << fullName << "\t" << "is created" << endl;

	//Create ROOT Tree with branches
	double ch0rate =0;
	double ch1rate =0;
	double ch2rate =0;
	double time=0;
	TTree* tree = new TTree("T1","");

	tree->Branch("Ch0Rate",&ch0rate,"Ch0Rate/D");
	tree->Branch("Ch1Rate",&ch1rate,"Ch1Rate/D");
	tree->Branch("Ch2Rate",&ch2rate,"Ch2Rate/D");
	tree->Branch("Time",&time,"Time/D");

	int32_t  currenttime, previoustime;
	int counter=0;

	//chHex=0x01;
	while(measurement!=counter){

  	      //Clear All Counters
		if(CAENVME_WriteCycle(fCrateHandle,fVMEAddressSca+0x1122, &chHex,cvA32_U_DATA,cvD16)!=cvSuccess) //0-31
                cout << "VME write error" << endl;
        	currenttime = get_time();
        	//Read Channels of the discriminator settings
        	sleep (1);
      		int data;
       		previoustime = get_time();

        	if(CAENVME_ReadCycle(fCrateHandle,fVMEAddressSca+0x1000,&data,cvA32_U_DATA,cvD32)!=cvSuccess)
              		cout << "VME read error" << endl;

        	ch0rate =data/((previoustime-currenttime)/1000.);

        	if(CAENVME_ReadCycle(fCrateHandle,fVMEAddressSca+0x1004,&data,cvA32_U_DATA,cvD32)!=cvSuccess)
                	cout << "VME read error" << endl;

        	ch1rate =data/((previoustime-currenttime)/1000.);

		if(CAENVME_ReadCycle(fCrateHandle,fVMEAddressSca+0x1008,&data,cvA32_U_DATA,cvD32)!=cvSuccess)
			cout << "VME read error" << endl;

        	ch2rate =data/((previoustime-currenttime)/1000.);

        	time = GetUnixTime();
        	tree->Fill();
        	counter++;
        	std::cout<<"\r                                                                         \r";
        	std::cout<<(double)counter/measurement*100<<"% " << "Rate_0:	" << ch0rate << " Hz       " << " Rate_1:	" << ch1rate << " Hz		" << " Rate_2:	" << ch2rate << " Hz";
        	std::cout.flush();

   }
   tree->Write();
   output->Close();

}




int main(int argc, char *argv[]){ // argv[1] - name txt file with ch-thresholds; argv[2] - name for root file


	// OR argv[2] -  raw data dir; argv[3], argv[4], argv[5] -voltages for whatever-  in this case root  name is creating by method from discriminator
	//Set Thresholds
	discriminator fDisc;
	int fCrateHandle=fDisc.ReadConfigFile("scaler.ini");
	fDisc.ReadCfg(argv[1]);
	int Nthr = fDisc.GetNthresholds();

	//Save Directory
	if(argc > 2){
		//Enable All Channels
		u_int32_t chHex =0xFFFFFF;
		u_int32_t fVMEAddressSca=0xDDDD0000;

		if(CAENVME_WriteCycle(fCrateHandle,fVMEAddressSca+0x1100, &chHex,cvA32_U_DATA,cvD32)!=cvSuccess) //0-31
			cout << "VME write error" << endl;
		else
			cout << "VME write successful" << endl;

		double v1;
		double v2;
		double v3;
		sscanf(argv[3],"%lf",&v1);
		sscanf(argv[4],"%lf",&v2);
		sscanf(argv[5],"%lf",&v3);
		cout << "voltage 1st SiPM:" << v1 << "\t" <<"voltage 2nd SiPM:" << v2 << "\t volt PMT \t"<< v3 << endl;
		//cout << "volt PMT \t"<< v1 << endl;
		for(int i=0; i< Nthr; i++){
			cout <<endl << "setting tresholds..." << endl;
			fDisc.SetThresholds2(i);
			DarkCount2Root(fDisc.GetRootName(i, v1, v2, v3).c_str(), fCrateHandle, argv[2]);
			//DarkCount2Root(fDisc.GetRootName(i, v1).c_str(), fCrateHandle, argv[2]);
		}
	}
	else{
		fDisc.SetThresholds();
		DarkCount2Root(argv[2], fCrateHandle);
	}
	fDisc.Close();
	std::cout << endl << endl;

	return 0;
}

   
   
  

/******************************************************************************


  @author  :  Julien Wulf
  @Comment :  DC meausrments over the ADC1724 standard firmware

 
******************************************************************************/


//C++ Libraries
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <cmath>
#include <map>
#include <math.h>
#include <vector>
#include <sys/time.h>
#include <string>
#include <Riostream.h>
#include <iostream>

//Some Classes
#include "keyb.h"	// getch() and kbhit() for Linux
#include <signal.h>	// to catch for Ctrl-C 
#include <global.h>
#include <VMEManager.h>
#include <ADCManager1730.h>
#include <ADCManager1724.h>
#include <ADCManager1720.h>
#include "ADCManager.h"
#include <ScopeManager.h>
#include <StorageManager.h>
#include <SlowcontrolManager.h>
#include <time.h>
#include "global.h"
#include "common.h"
#include "DCManager.h"

//Root Libaries
#include <TH1D.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TFile.h>
#include <TLatex.h>
#include "TTree.h"
#include "TFile.h"
#include "TROOT.h"

//--------------------------------------------------------------------------
// catch CTRL-C, otherwise files etc. are not closed properly
void sigproc(int sig)
{ 		 
  signal(SIGINT, sigproc); 
  printf(KRED);
  printf("	WARNING: please use 'q' to stop the DAQ\n");
  printf(RESET);
}



//--------------------------------------------------------------------------
// MAIN
int main(int argc, char *argv[], char *envp[] )
{
	signal(SIGINT, sigproc);

    	//SlowcontrolManager
	SlowcontrolManager* slowcontrolManager = new SlowcontrolManager();
	slowcontrolManager->ProcessInput(argc, argv, envp);
	if(slowcontrolManager->Init()==-1)
		return 0;
		
	//VME-Manger
	VMEManager* vManager = new VMEManager();

	vManager->SetPCILink(0);
    vManager->SetBoardNumber(slowcontrolManager->GetLinkInChain());
    
    if(vManager->Init()==-1)
		return 0;

	//DC-Manger
	ADCManager* adcManager = new ADCManager();
	adcManager = new DCManager();

	adcManager->SetCrateHandle(vManager->GetCrateHandle());
	adcManager->SetADCAddress(slowcontrolManager->GetAddress(0));
	adcManager->SetRegisterFile("RegisterConfig.ini");
        std::string bp = Common::getdotdaqdir();
        bp.append("/Baseline/Module_0_DACBaseline.ini");
        adcManager->SetBaselineFile(bp.c_str());
	adcManager->SetXMLFile(slowcontrolManager->GetXMLFile());

	if(adcManager->Init()==-1);
	if(slowcontrolManager->GetADCInformation()) return 0;
	if(slowcontrolManager->GetBaselineCalculation()){
		adcManager->EnableSoftware();
		adcManager->CalculateBaseLine();
		return 0;
	}
	else
		adcManager->ReadBaseLine();

	//Scope-Manager
	ScopeManager* scopeManager = new ScopeManager();
	scopeManager->SetBuffer(adcManager->GetBuffer());
	scopeManager->SetEventLength(adcManager->GetEventLength());
	scopeManager->SetXMLFile(slowcontrolManager->GetXMLFile());
	scopeManager->SetModuleNumber(1);
	scopeManager->SetChannelThresh(adcManager->GetThreshold());
	scopeManager->SetThreshold();
	if(slowcontrolManager->GetGraphicsActive()){
		//ROOT Manager
		TApplication *theApp;
		theApp = new TApplication("App", &argc, argv);	
		if(scopeManager->Init()==-1)
			return 0;
	}





    	/*Stuff for the keyboard*/
    	char c;
    	int quit=0; 
    	c=0;
    

    	int countermax=0; 

    	slowcontrolManager->StartAquistion();
    	//adcManager->Enable();
	//adcManager->EnableChannel(0);
	//adcManager->CheckEventBuffer();		//Read Buffer before start aquisition


	double runtimeSlow=0;
	double runtime=0;
	double *baseline;
	int *threshold;
	double *rate;
	double *time;							//Storage of the current time
   	 double *rms;
	baseline = new double[8];
	rms = new double[8];
	threshold = new int[8];
	rate = new double[8];
	time = new double[8];

    	struct timeval m_begin;
    	struct timeval m_end;
		
	//Create a ROOTFile for Storage
	std::string output =  std::string(slowcontrolManager->GetPathName()) + "/" +  std::string(slowcontrolManager->GetFolderName()) + ".root";
	std::cout << "	Storage:	" << output << std::endl << std::endl;
	TFile* gROOTFile = new TFile(output.c_str(), "RECREATE");
	TTree *t1 = new TTree("t1","t1");
	t1->Branch("Time", time, TString::Format("time[%i]/D", 8));
	t1->Branch("Threshold", threshold, TString::Format("threshold[%i]/I", 8));
	t1->Branch("Baseline", baseline, TString::Format("baseline[%i]/D", 8));
	t1->Branch("RMS", rms, TString::Format("rms[%i]/D", 8));
	t1->Branch("DC", rate, TString::Format("rate[%i]/D", 8));

    	//Properties of the Peaks inside the trace
	for(int i=0;i<8;i++){
		rate[i]=0;
		threshold[i]=0;
		baseline[i]=0;
		rms[i]=0;
		time[i]=0;
	}

	while(slowcontrolManager->GetTotaltNumberEvents()!=countermax && quit!=1){
		// Check keyboard commands in every loop   
		printf(KCYN);
		std::cout << std::endl << "	Event: " << countermax << std::endl << std::endl;

		for(int k=0;k<8;k++){
			int counter=0;
			double rm=0;
			adcManager->Enable();
			adcManager->EnableChannel(k);
			baseline[k]  =  adcManager->AverageBaseLine(k,rm);
			rms[k] = rm;
			threshold[k] =  adcManager->CalculateThresholds(k,baseline[k]);	
			if(slowcontrolManager->GetGraphicsActive()){	
				scopeManager->SetChannelNumber(k);
				scopeManager->SetChannelThresh(threshold); 		
			} 
			for(int i=0;i<500;i++)
				//Check if there have been an accepted trigger before aquisition in the buffer
				if(adcManager->CheckEventBuffer(0)<-1) return 0;	
			gettimeofday(&m_begin, NULL);
  //          		if(k!=0 || k!=1)
//                		continue;
			while(0<1){
				c = 0;
				if (kbhit()) c = getch();
				if (c == 'q' || c == 'Q') quit = 1;	

				//Check keys to change adc settings
				adcManager->Checkkeyboard(c);

				//Check if there have been an accepted trigger
				if(adcManager->CheckEventBuffer(0)<-1) return 0;	

				//Timing
		  		gettimeofday(&m_end, NULL);
				runtimeSlow =  (double) (m_end.tv_usec - m_begin.tv_usec)/ 1000000 + (double) (m_end.tv_sec - m_begin.tv_sec);
				if(runtimeSlow>10){
					rate[k] = counter/runtimeSlow;
					time[k]= slowcontrolManager->GetCurrentUnixTime();
					std::cout<< "Timing:	" << runtimeSlow << "	Counter:	" << counter << "	Channel:	" << k << "	Baseline: " << baseline[k] << "	RMS: " << rms[k] << "	Threshold:	" << threshold[k]  << "	Rate: "  << rate[k] << " [Hz] "<<  std::endl;
					counter=0;
					break;
				}
				else{
					//Skipp events with 0-bytes
					if(adcManager->GetTransferedBytes()<=0){
						continue;
					}
					//Show Event if checked
					if(slowcontrolManager->GetGraphicsActive())
						scopeManager->ShowEvent();
					//status output, Slowcontrol etc
					counter++;
				}
			}
			adcManager->Disable();
		}
		printf(RESET);
		t1->Fill();
		countermax++;
	}
	gROOTFile->cd();
	t1->Write();
	gROOTFile->Close();
	slowcontrolManager->StopAquistion();

}







/******************************************************************************

  DAQ      : DAQ for any kind of Detectors
  @author  : Julien Wulf
  @Comment : DAQ to control a Single CAEN flash ADC

  LogBook:
			JW - 07.12.2015 - First Stable Version v1.0.0 (Some Trigger features are missing)
			JW - 08.12.2015 - Self Trigger implemented, trehshold can be controlled over w, e, r, t and i
			JW - 10.01.2016 - ADC Extended for v1720, v1724 and 1730D, Postrigger controlable over o & p
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
    
    //Managers
    
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
	
	std::vector <ADCManager*> adcs;
	std::vector <StorageManager*> storages;
	char baseline[50];
	int number=slowcontrolManager->GetNbModules();
	for(int i=0;i<slowcontrolManager->GetNbModules();i++){
		if(slowcontrolManager->GetADCType()==0)
			adcs.push_back(new ADCManager1720());	
		else if(slowcontrolManager->GetADCType()==1)
			adcs.push_back(new ADCManager1724());	
		else if(slowcontrolManager->GetADCType()==2)
			adcs.push_back(new ADCManager1730());	
			
		adcs[i]->SetCrateHandle(vManager->GetCrateHandle());
		adcs[i]->SetADCAddress(slowcontrolManager->GetAddress(i));
		adcs[i]->SetModuleNumber(i);
		adcs[i]->SetRegisterFile("RegisterConfig.ini");				//Shpuld be the same for all modules
		sprintf(baseline, "Module_%i_DACBaseline.ini", i);
		std::string bp = Common::getdotdaqdir();
		bp.append("/Baseline/");
		bp.append(baseline);
		adcs[i]->SetBaselineFile(bp.c_str());
		adcs[i]->SetXMLFile(slowcontrolManager->GetXMLFile());

		if(adcs[i]->Init()==-1);
	}
	if(slowcontrolManager->GetADCInformation()) return 0;
	if(slowcontrolManager->GetBaselineCalculation()){
		for(int i=0;i<number;i++){
                     //   adcs[i]->ReadBaseLine();
			adcs[i]->CalculateBaseLine();
		}
		return 0;
	}
	else{
		for(int i=0;i<number;i++){
				adcs[i]->ReadBaseLine();
		}
	}
	//Scope-Manager
	ScopeManager* scopeManager = new ScopeManager();
	scopeManager->SetEventLength(adcs[0]->GetEventLength());				//Master Board
	scopeManager->SetXMLFile(slowcontrolManager->GetXMLFile());
	scopeManager->SetChannelNumber(slowcontrolManager->GetChannelNumber());
	scopeManager->SetModuleNumber(slowcontrolManager->GetNbModules());
	if(slowcontrolManager->GetGraphicsActive()){
		//ROOT Manager
		TApplication *theApp;
		theApp = new TApplication("App", &argc, argv);	
		if(scopeManager->Init()==-1)
			return 0;
	}

	//StorageManager
	for(int i=0;i<slowcontrolManager->GetNbModules();i++){
		storages.push_back(new StorageManager());	
		storages[i]->SetBuffer(adcs[i]->GetBuffer());
		storages[i]->SetEventLength(adcs[i]->GetEventLength());
		storages[i]->SetXMLFile(slowcontrolManager->GetXMLFile());
		storages[i]->SetFolderName(slowcontrolManager->GetFolderName());
		string folder=Form("Module%i/",i);
		storages[i]->SetModuleFolder(folder);
		storages[i]->SetModuleNumber(i);
		if(storages[i]->Init()==-1);	
	}
	

	//Stuff for the keyboard
    char c;
    int quit=0; 
    int* counter;
	counter = new int[slowcontrolManager->GetNbModules()];
	for(int i=0;i<slowcontrolManager->GetNbModules();i++){
		counter[i]=0;
	}
    c=0;
   
    slowcontrolManager->StartAquistion();
    for(int i=0;i<slowcontrolManager->GetNbModules();i++){
		adcs[i]->Enable();
		adcs[i]->CheckEventBuffer();		//Read Buffer before start aquisition
	}

	while(slowcontrolManager->GetNumberEvents()!=storages[0]->GetNumberEvents() && quit!=1){
		c = 0;  
		if (kbhit()) c = getch();
		if (c == 'q' || c == 'Q') quit = 1;	

		if(slowcontrolManager->GetGraphicsActive())	
			scopeManager->graph_checkkey(c);

		if(adcs[0]->GetTriggerType()==1){
				usleep(adcs[0]->GetSoftwareRate());
		}

		//Get Event Check all ADCs always
		for(int i=0;i<slowcontrolManager->GetNbModules();i++){

			if(adcs[0]->GetTriggerType()==1 && i==0){											//software trigger from the master board
				if(adcs[i]->SoftwareTrigger()<-1) return 0;						
			}
			
			if(adcs[i]->CheckEventBuffer()<-1) return 0;							
		

			//Skipp events with 0-bytes
			if(adcs[i]->GetTransferedBytes()<=0 && (i==0)){
				slowcontrolManager->ShowStatus(-1);
				break;
			}
			
			slowcontrolManager->AddBytes(adcs[i]->GetTransferedBytes());
			
			//Show Event if checked
			if(slowcontrolManager->GetGraphicsActive() && scopeManager->GetModule()==i){
				adcs[i]->Checkkeyboard(c);
				scopeManager->SetBuffer(adcs[i]->GetBuffer());
				scopeManager->SetChannelTresh(adcs[i]->GetTreshold());
				scopeManager->ShowEvent();
			}
			
			//Save the events or not :)
			storages[i]->FillContainer();	

			//status output, Slowcontrol etc
			if((i==0))
				slowcontrolManager->ShowStatus();	

			counter[i]++;
/*
			//Create new file if noE is bigger than noEF
			if(counter[i]==storages[i]->GetEventsPerFile() && storages[i]->GetNumberEvents()>slowcontrolManager->GetNumberEvents()){
				counter[i]=0;
				storages[i]->NewFile();
			}
*/		
		}
	}

    for(int i=0;i<slowcontrolManager->GetNbModules();i++){
		adcs[i]->Disable();
		storages[i]->SaveContainer();
	}
	slowcontrolManager->StopAquistion();


    return 0;
}



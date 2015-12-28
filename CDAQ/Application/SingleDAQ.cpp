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
    if(vManager->Init()==-1)
		return 0;

	//ADC-Manger
	ADCManager* adcManager = new ADCManager();
	if(slowcontrolManager->GetADCType()==0)
		adcManager = new ADCManager1720();
	else if(slowcontrolManager->GetADCType()==1)
		adcManager = new ADCManager1724();
	else if(slowcontrolManager->GetADCType()==2)
		adcManager = new ADCManager1730();
	adcManager->SetCrateHandle(vManager->GetCrateHandle());
	adcManager->SetADCAddress(slowcontrolManager->GetAddress());
	adcManager->SetRegisterFile("RegisterConfig.ini");
	adcManager->SetBaselineFile("Module_0_DACBaseline.ini");
	adcManager->SetXMLFile(slowcontrolManager->GetXMLFile());

	if(adcManager->Init()==-1);
	if(slowcontrolManager->GetADCInformation()) return 0;
	if(slowcontrolManager->GetBaselineCalculation()){
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
	scopeManager->SetChannelNumber(slowcontrolManager->GetChannelNumber());
	scopeManager->SetChannelTresh(adcManager->GetTreshold());
	if(slowcontrolManager->GetGraphicsActive()){
		//ROOT Manager
		TApplication *theApp;
		theApp = new TApplication("App", &argc, argv);	
		if(scopeManager->Init()==-1)
			return 0;
	}

	//StorageManager
	StorageManager* storageManager = new StorageManager();
	storageManager->SetBuffer(adcManager->GetBuffer());
	storageManager->SetEventLength(adcManager->GetEventLength());
	storageManager->SetXMLFile(slowcontrolManager->GetXMLFile());
	storageManager->SetFolderName(slowcontrolManager->GetFolderName());
	if(storageManager->Init()==-1);


    /*Stuff for the keyboard*/
    char c;
    int quit=0; 
    int counter=0; 
    c=0;
    
    slowcontrolManager->StartAquistion();
    adcManager->Enable();
	adcManager->CheckEventBuffer();		//Read Buffer before start aquisition
	
	while(slowcontrolManager->GetNumberEvents()!=storageManager->GetNumberEvents() && quit!=1){
		
		// Check keyboard commands in every loop   
		c = 0;  
		if (kbhit()) c = getch();
		if (c == 'q' || c == 'Q') quit = 1;	
		
		if(slowcontrolManager->GetGraphicsActive())	
			scopeManager->graph_checkkey(c);
					
		//Check keys to change adc settings
		adcManager->Checkkeyboard(c);
		
		//Get Event
		if(adcManager->GetTriggerType()==1){
			if(adcManager->ApplySoftwareTrigger()<-1) return 0;
			usleep(adcManager->GetSoftwareRate());
		}
		else
		{
			if(adcManager->CheckEventBuffer()<-1) return 0;			
		}

		//Skipp events with 0-bytes
		if(adcManager->GetTransferedBytes()<=0){
			slowcontrolManager->ShowStatus(-1);
			continue;
		}
		
		slowcontrolManager->AddBytes(adcManager->GetTransferedBytes());
		
		//status output, Slowcontrol etc
		slowcontrolManager->ShowStatus();

		//Save the events or not :)
		storageManager->FillContainer();	
		
		//Show Event if checked
		if(slowcontrolManager->GetGraphicsActive())
			scopeManager->ShowEvent();
			
		counter++;
		//Create new file if noE is bigger than noEF
		if(counter==storageManager->GetEventsPerFile() && storageManager->GetNumberEvents()>slowcontrolManager->GetNumberEvents()){
			counter=0;
			storageManager->NewFile();
		}
	}
	
	slowcontrolManager->StopAquistion();
	adcManager->Disable();
	storageManager->SaveContainer();

    return 0;
}


// Julien W.
// Program to set the the v895 Leding Edge Discriminator and Scaler

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
#include <xml.h> 
#include <global.h>
#include "VMEManager.h"
#include "DiscriminatorManager.h"
#include "ScalerManager.h"



using namespace std;


int main(int argc, char *argv[]){ 
	
	printf("\n");
  	printf("%s*****************************************************************\n",KGRN);
  	printf("%s                   Discriminator Scaler Control                  \n",KGRN); 
  	printf("%s                          version: 3.0                           \n",KGRN);
	printf("%s             With Great Force Comes Great Responsibility 	       \n",KGRN);
	printf("%s                         Julien Wulf (UZH)      	               \n",KGRN);
  	printf("%s*****************************************************************\n\n",KGRN);
  	printf(RESET);
  	
  	//For Timing
	struct timeval begin, end;
    double mtime, seconds, useconds;    
    gettimeofday(&begin, NULL);

	//Create Managers and read XML File

	VMEManager* vManager = new VMEManager();
	DiscriminatorManager* dManager = new DiscriminatorManager();
	ScalerManager* sManager = new ScalerManager();

	xml_readsettings("Settings.xml",vManager,dManager,sManager); 


	 //Init all Manager
	 if(vManager->Init()==-1)
		return 0;

	 dManager->SetCrateHandle(vManager->GetCrateHandle());
	 sManager->SetCrateHandle(vManager->GetCrateHandle());

	 if(dManager->Init()==-1)
        	return 0;
        
    	 if(sManager->Init()==-1)
        	return 0;
       
       
     //Main Program Set only Discriminator or aquire data of the scaler with a certain treshold  
 	 if(sManager->GetActive()==1){
		for(int i=0;i<dManager->GetNthresholds();i++){
			//Set Tresholds from XML or Treshold File
			if(dManager->SetThresholdsDisc(i)==-1)
				return 0;
			//Read Scaler and save data
			if(sManager->ReadMultipleCycles()==1){
				break;
			}
			else{
				return 0;
			}
		} 
     }
     else{
		for(int i=0;i<dManager->GetNthresholds();i++){
			//Set Tresholds from XML or Treshold File
			if(dManager->SetThresholdsDisc(i)==-1)
				return 0;			 
		}
	}
     
    vManager->Close();
    gettimeofday(&end, NULL);
	seconds  = end.tv_sec  - begin.tv_sec;
	useconds = end.tv_usec - begin.tv_usec;
	printf(KGRN);
	std::cout << "	Total Time: " << seconds << "seconds "<< std::endl;
	printf(RESET);
	delete vManager;
	delete dManager;
	delete sManager;
	return 0;
}

   
   
  

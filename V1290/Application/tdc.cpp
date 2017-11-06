// Julien W.
// Program to set and aquire data from the TDC

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
#include "TDCManager.h"

using namespace std;


int main(int argc, char *argv[]){ 
	
	printf("\n");
  	printf("%s*****************************************************************\n",KGRN);
  	printf("%s                  		 TDC Control 		                   \n",KGRN); 
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
	TDCManager* tManager = new TDCManager();

	xml_readsettings("Settings.xml", vManager, tManager); 


	 //Init all Manager
	 if(vManager->Init()==-1)
		return 0;
		
	 if(tManager->Init()==-1)
		return 0;
		
	tManager->SetCrateHandle(vManager->GetCrateHandle());

	
	tManager->test();
	// dManager->SetCrateHandle(vManager->GetCrateHandle());
	// sManager->SetCrateHandle(vManager->GetCrateHandle());

	// if(dManager->Init()==-1)
     //   	return 0;
        
    //	 if(sManager->Init()==-1)
//return 0;
       
       
     //Main Program Set only Discriminator or aquire data of the scaler with a certain treshold  






     
    	vManager->Close();
    	gettimeofday(&end, NULL);
	seconds  = end.tv_sec  - begin.tv_sec;
	useconds = end.tv_usec - begin.tv_usec;
	printf(KGRN);
	std::cout << "	Total Time: " << seconds << "seconds "<< std::endl;
	printf(RESET);
	delete vManager;
	return 0;
}

   
   
  

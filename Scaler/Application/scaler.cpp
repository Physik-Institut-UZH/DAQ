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
#include "keyb.h"


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
	char m_exc;
	
	//Create Managers and read XML File

	VMEManager* vManager = new VMEManager();
	DiscriminatorManager* dManager = new DiscriminatorManager();
	ScalerManager* sManager = new ScalerManager();
	
	
	extern char *optarg;               /* options processing */
	int start;                        /* dummy for options */
	int someArgs=0;
	char m_OutputFolder[100];	//Outputfolder name (custom)
	char m_XmlFileName[100];    //XML-Filename
	int m_errflag=0;
	while ( ( start = getopt( argc, argv, "x:f:w" ) ) != -1 )  {
      someArgs=1;
      switch (start) {
        case 'f': 
          sprintf(m_OutputFolder,"%s",optarg);
          break;
        case 'x': 
          sprintf(m_XmlFileName,"%s",optarg);
          break;
        case 'w':   
	      printf("missed WIMPs protocol:");
		  int i;
	      char txt[100];
          i = (int)((float)rand()/2147483647.*10.);
          printf("  You missed %d WIMPs since stopping DAQ.\n  Better you start it again...\n",i);
	      exit(0);	
	      break;
			default: 
          m_errflag++; 
          break;
       }
	   if (m_errflag || someArgs==0) {
		  printf(KRED);
		  printf("usage: %s [-f file|-x file||-g p|-b|-i|-h|]\n", argv[1] );
		  printf("\t-f write to file_ \n" );
		  printf("\t-x read settings from XML-file file\n");
		  printf("\t-g Oscilloscope: display PMT p (SLOW!)\n");
		  printf("\t-i displays hardware information\n" );
		  printf("\t-b adjust baselines automatically\n" );
		  printf("\t-o show help for oscilloscope mode\n");
		  printf("\t-h display this help\n\n" );
		  printf(RESET);
		  exit( 2 );
	   } 
  }

	xml_readsettings(m_XmlFileName,vManager,dManager,sManager); 




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

			}
			
			if (kbhit()) m_exc = getch();
           	if (m_exc == 's') {
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

   
   
  

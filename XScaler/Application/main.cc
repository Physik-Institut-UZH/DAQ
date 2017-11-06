//Julien W. Shitty but working version

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include "scaler.hh"
#include "keyb.hh"
#include <cmath>


//ROOT Libraries
#include "TROOT.h"
#include <TApplication.h>
#include "TSystem.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TRint.h"
#include "TApplication.h"
#include "keyb.hh"



using namespace std;

int main(int argc, char *argv[])
{

   scaler fScaler;
   if(fScaler.ReadConfigFile("config.ini")!=0)  {
      cout<<"Error reading config file."<<endl;
      return -1;
   }

   time_t starttime;
   struct tm *timeinfo;
   time(&starttime);
   char filestring[100];
   char rootfilestring[100];
   timeinfo=localtime(&starttime);
   strftime(filestring,100,"/home/marmotx/Cloud/Run2/DC/zAfterRecup/0.5PE_%y%m%d_%H%M.dat",timeinfo);
   strftime(rootfilestring,100,"/home/marmotx/Cloud/Run2/DC/zAfterRecup/0.5PE_%y%m%d_%H%M.root",timeinfo);

   double m_rates[16];
   TFile* output = new TFile(rootfilestring , "RECREATE");
   TTree* tree = new TTree("T1","");
   int m_time;
   for(int i=0;i<16;i++){
   	tree->Branch(Form("Ch%iRate",i),&m_rates[i],Form("Ch%iRate/D",i));
    }
   tree->Branch("Time",&m_time,"Time/I");

   ofstream outfile;
   cout<<filestring<<endl;
   outfile.open(filestring);
   if(!outfile) {
     cout<<"Error opening file."<<endl;
     return -1;
   }
   if(fScaler.Reset()!=0) cout<<"Reset failed!"<<endl;
   char c='d';
   int counter=0;
   bool print=false;

   // Input acquisition interval for DC rate estimation
   int interval;
   if (argc==2){                                           //check if path is already given, ask if not
      interval=atoi(argv[1]);
      }
   else{
      cout<<"Enter the acquisition interval in SECONDS: "<<endl;
      cin>>interval;
      }

    cout<<"Enter Number of events: "<<endl;
    cin >> counter;

   // File header
   sleep(1);
   stringstream printstream;

   for(int x=0;x<16;x++)     {
      int chan = fScaler.ReadChannel(x);
      if(chan>0){
         print=true;
         printstream<<"Ch: "<<x<<" ";
      }
   }

   outfile<<fScaler.GetTimeString()<<" Interval: "<<interval<<" seconds "<<printstream.str()<<endl;
   cout<<fScaler.GetTimeString()<< endl;
   int end=0;


   // Start data stream
   while(c!='q' && end!=counter)  {

	  if(fScaler.Reset()!=0){ cout<<"Reset failed!"<<endl; break;}
	  if(kbhit()) c = getch();
	  if(c=='r'){
	  	if(fScaler.Reset()!=0){
	  		cout<<"Failed to reset."<<endl;
  	  		break;
	  	}
     	  }
      	  if(c=='t'){
		if(fScaler.Test()==-1){
			cout<<"Error writing to VME."<<endl;
			break;
		}
	        fScaler.Test();
                fScaler.Test();
                fScaler.Test();
                fScaler.Test();
                fScaler.Test();
                fScaler.Test();
	   	fScaler.Test();

	   }
      	   if(c=='q'){
	   	 break;
	   }
      sleep(interval);
      stringstream printstream;
      stringstream screenstream;
      for(int x=0;x<16;x++){
		 int chan = fScaler.ReadChannel(x);
         	 int freq = (int)((chan/interval)+0.5);
		 m_rates[x]=freq;
		 screenstream<< "Ch "<<x<<": "<<chan<<" "<<"("<<freq<<" Hz)   " << std::endl;
		 printstream<<chan<<" "<<freq<<" ";
      }

      time_t currentTime;
      time(&currentTime);
      m_time = fScaler.GetUnixTime();
      outfile<<fScaler.GetUnixTime()<<" "<<printstream.str()<<endl;
      cout<<fScaler.GetTimeString()<<  "    " << std::dec << end << "/" << std::dec << counter << endl << " "<<screenstream.str() << endl << endl;
      starttime=currentTime;
      tree->Fill();
      end++;

   }
   tree->Write();
   output->Close();
   fScaler.Close();
   outfile.close();
   cout<<"finished."<<endl;
   return 0;
}


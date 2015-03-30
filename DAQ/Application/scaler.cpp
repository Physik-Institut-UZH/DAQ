// Daniel M version, modified from main_backup
// 12.05.2014

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include "scaler.h"
#include "keyb.h"
#include <cmath>
#include <map>
#include <math.h>

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

int main(int argc, char *argv[])
{
   scaler fScaler;
   if(fScaler.ReadConfigFile("scaler.ini")!=0)  {
      cout<<"Error reading config file."<<endl;
      return -1;
   }
   time_t starttime;
   struct tm *timeinfo;
   time(&starttime);
   char filestring[100];
   char rootstring[100];
   timeinfo=localtime(&starttime);
   strftime(filestring,100,"/disk/data1/atp/dmayani/Scaler/scaler_%y%m%d_%H%M.dat",timeinfo);
   strftime(rootstring,100,"scaler_%y%m%d_%H%M.root",timeinfo);
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
   cout<<fScaler.GetTimeString()<<" "<<printstream.str()<<endl;

   TFile *hfile = new TFile(Form("/disk/data1/atp/dmayani/Scaler/%s",rootstring),"RECREATE");
   TTree *DataTree = new TTree("DataTree","data");
   Int_t chans[16];
   DataTree->Branch("chans", chans, "chans[16]/I");


// Start data stream
   while(c!='q')  {
      sleep(1);
      if(counter==0)
	cout<<"r - reset counters, t - test increment counters, q - quit"<<endl;
      counter++;
      if(counter==300) counter=0;
      if(kbhit()) c = getch();
      
      if(c=='r')	{
	 if(fScaler.Reset()!=0) cout<<"Failed to reset."<<endl;
	 c='d';
      }
      if(c=='t'){
	if(fScaler.Test()==-1) cout<<"Error writing to VME."<<endl;
	c='d';
      }
      if(c=='q')
	break;
      
      stringstream printstream;
      stringstream screenstream;

      for(int x=0;x<16;x++)	{	   
	 int chan = fScaler.ReadChannel(x);
	 chans[x]=chan;
         int freq = (int)((chan/interval)+0.5);
	 if(chan>0){
	    print=true;
	    screenstream<<"Ch "<<x<<": "<<chan<<" "<<"("<<freq<<" Hz)   ";
	    printstream<<chan<<" "<<freq<<" ";
	 }
	 DataTree->Fill();	 
      }
      time_t currentTime;
      time(&currentTime);
      if(print && difftime(currentTime,starttime)>interval){
	outfile<<fScaler.GetTimeString()<<" "<<printstream.str()<<endl;
	cout<<fScaler.GetTimeString()<<" "<<screenstream.str()<<endl;
      starttime=currentTime;
      if(fScaler.Reset()!=0) cout<<"Reset failed!"<<endl;
      print=false;
      }
   }



   fScaler.Close();
   outfile.close();
   cout<<"finished."<<endl;
   DataTree->Print();
   hfile->Write();
   return 0;
}

   
   
  

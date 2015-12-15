/********************************************************************\

  Name:         ZDAQ.cpp
  Created by:   Julien Wulf
 		based on drs_example.cpp by Stefan Ritt

  Contents:     Readout DRS4 with some features.
		(Calibration, Root Tree, Graphic Mode, XML Parser)

\********************************************************************/

//Standart c++ Libaries
#include <math.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

//Intenral Created Files
#include "strlcpy.h"
#include "DRS.h"
#include "keyb.h"
#include "xml.h"
#include "DAQContainer.h"

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

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"
/*------------------------------------------------------------------*/

void gettimestring(char* buffer)
{
  time_t curtime;
  struct tm *loctime;
  curtime = time (NULL);                		// Get the current time.
  loctime = localtime (&curtime);      			 // Convert it to local time representation.
  strftime(buffer, 100, "%y%m%d_%H%M", loctime);       // formatted print for filenames

}
/*------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
  DAQContainer* container = new DAQContainer();				     /* DAQContainer */
  extern char *optarg;               /* options processing */
  int start;                         /* dummy for options */
  int errflag=0;                     /* error flag */
  int someArgs=0;
  int ChannelNum=1;
  gettimestring(OutputFolder);
  char OutputFolder[100]="";
  char OutFileName[100]="";
  char XmlFileName[100]="";

  while ( ( start = getopt( argc, argv, "x:g:f:c" ) ) != -1 )  {
	someArgs=1;
      	switch ( start ) { 
		case 'x':
			printf(KYEL);
			std::cout << "WIMP Mode" << std::endl;
			sprintf(XmlFileName,"%s",optarg);
			break;
		case 'g':
			ChannelNum=atoi(optarg);
			if (ChannelNum>3) errflag=1;
			if (ChannelNum<0) errflag=1;
			printf(KYEL);
			printf("\n");
			container->SetGraphicMode();
			std::cout << "Graphic Mode" << std::endl;
			break;
		case 'f':
			sprintf(OutputFolder,"%s",optarg);
			printf(KYEL);
                	std::cout << "Save Data: " << OutputFolder << std::endl;
			break;
		case 'c':
			container->SetCalibrationMode();
			printf(KYEL);
                	std::cout << "Calibrate Board" << std::endl;
			break;
		default:
			errflag=1;
        		break;

        }
 }

  if (errflag || someArgs==0){
	printf(KRED);
	std::cout << "Not Enough Arugements" << std::endl;
	std::cout << "-g (Channel Number), -x (XML-File Necessary!!!), -f (Output filename) or -c (Calibration) " << std::endl;
	printf(RESET);
        exit( 2 );
  }

  printf("\n");
  printf("%s*****************************************************************\n",KGRN);
  printf("%s           DAQ  --  Alternative DAQ for UZH (PSI Board)          \n",KGRN); 
  printf("%s                          version: 1.0                           \n",KGRN);
  printf("%s*****************************************************************\n\n",KGRN);
  printf(RESET);

   char c;
   int quit; 

   quit=0;
   int i, j, nBoards;
   DRS *drs;
   DRSBoard *b;
   float time_array[8][1024];
   float wave_array[8][1024];

   /* do initial scan */
   printf(KRED);
   drs = new DRS();
   /* show any found board(s) */
   for (i=0 ; i<drs->GetNumberOfBoards() ; i++) {
      b = drs->GetBoard(i);
      printf(KYEL);
      printf("Found DRS4 evaluation board, serial #%d, firmware revision %d\n\n", 
         b->GetBoardSerialNumber(), b->GetFirmwareVersion());
      printf(KRED);
   }

   /* exit if no board found */
   nBoards = drs->GetNumberOfBoards();
   if (nBoards == 0) {
      printf(KRED);
      printf("No DRS4 evaluation board found\n");
      printf(RESET);
      return 0;
   }


   /* continue working with first board only */
    b = drs->GetBoard(0);


   /* initialize board */
    b->Init();


   /*Configure Board with XML-File*/
    xml_readsettings(XmlFileName,b,container);	
    
    /* Calibrate Timming */
    if(container->GetCalibrationMode()){
       b->CalibrateVolt(NULL);
       b->CalibrateTiming(NULL);
       printf(RESET);
       delete drs;
       return 0;
    }
    /*Some Root Sutff*/
    TCanvas *win;
    TH1D *g1;
    TH1D *g2;
    TH1D *g3;
    TH1D *g4;
    TApplication *theApp;
    TFile* gROOTFile;

    if(container->GetGraphicMode()){ 
   	 theApp = new TApplication("App", &argc, argv);
  	 win = new TCanvas("win","DAQ -- DAQ for PSI Board DRS4",1);
  	 g1 = new TH1D("ZDAQ - Channel 1","ZDAQ - Channel 1",1023,0,1023);
         g2 = new TH1D("ZDAQ - Channel 2","ZDAQ - Channel 2",1023,0,1023);
         g3 = new TH1D("ZDAQ - Channel 3","ZDAQ - Channel 3",1023,0,1023);
         g4 = new TH1D("ZDAQ - Channel 4","ZDAQ - Channel 4",1023,0,1023);
  	 win->cd();
  	 gStyle->SetOptStat(0000000);
  	 gStyle->SetOptFit(1100);
  	 gStyle->SetTitleFillColor(0);
  	 gStyle->SetTitleBorderSize(0);
  	 gStyle->SetStatColor(0);
  	 win->SetFillColor(0);
  	 win->SetBorderMode(0);

  	 // configure histogram
  	 g1->SetLineColor(2);
   	 g1->GetXaxis()->SetTitle("Sample");
   	 g1->GetYaxis()->SetTitle("mV");
	 g1->GetYaxis()->SetRangeUser(container->GetGraphicYLow(),container->GetGraphicYHigh());

         g2->SetLineColor(3);
   	 g2->GetXaxis()->SetTitle("Sample");
   	 g2->GetYaxis()->SetTitle("mV");
	 g2->GetYaxis()->SetRangeUser(container->GetGraphicYLow(),container->GetGraphicYHigh());

   	 g3->SetLineColor(4);
   	 g3->GetXaxis()->SetTitle("Sample");
   	 g3->GetYaxis()->SetTitle("mV");
	 g3->GetYaxis()->SetRangeUser(container->GetGraphicYLow(),container->GetGraphicYHigh());

	 g4->SetLineColor(6);
   	 g4->GetXaxis()->SetTitle("Sample");
   	 g4->GetYaxis()->SetTitle("mV");
	 g4->GetYaxis()->SetRangeUser(container->GetGraphicYLow(),container->GetGraphicYHigh());

    	 // Startup Window
   	 TLatex * tex = new TLatex(0.0,0.379562,"DAQ");
   	 tex->SetTextColor(2);
   	 tex->SetTextFont(112);
   	 tex->SetTextSize(0.629927);
   	 tex->SetLineWidth(2);
   	 tex->Draw();

   	 tex = new TLatex(0.0900164,0.233577," Written By ");
   	 tex->SetTextSize(0.136253);
   	 tex->SetLineWidth(2);
   	 tex->Draw();

	 tex = new TLatex(0.0600164,0.133577,"Julien Wulf - Jwulf@physik.uzh.ch");
   	 tex->SetTextSize((0.136253)/2);
   	 tex->SetLineWidth(2);
   	 tex->Draw();
   	 win->Modified();
   	 win->cd();
    	 win->SetSelected(win);  
    	 win->Update();
    	 sleep(5);
   } 
    printf(RESET);
    /*Create Output Folder and ROOT Folder*/
    std::string command = "mkdir " + container->GetOutputDir(); 
    system(command.c_str());
    command="";
    command = "mkdir " + container->GetOutputDir() + "/" + OutputFolder; 
    printf(KRED);
    system(command.c_str());
    command= "";
    command= container->GetOutputDir() + "/" + OutputFolder + "/ZDAQ_" + OutputFolder +".root";
 
    //Setting up a tree
    TTree* tree = new TTree("t1","");
    float wf1[1024];
    float wf2[1024];
    float wf3[1024];
    float wf4[1024];
    float timeChn1[1024];
    float timeChn2[1024];
    float timeChn3[1024];
    float timeChn4[1024];
    if(container->GetChannel0()){
    	tree -> Branch("wf1",&wf1,"wf1[1024]/F");
//	tree -> Branch("TimeChn1",&timeChn1,"timeChn1[1024]/F");
    }
    if(container->GetChannel1()){
    	tree -> Branch("wf2",&wf2,"wf2[1024]/F");
//	tree -> Branch("TimeChn2",&timeChn2,"timeChn2[1024]/F");
    }
    if(container->GetChannel2()){
    	tree -> Branch("wf3",&wf3,"wf3[1024]/F");
//	tree -> Branch("TimeChn3",&timeChn3,"timeChn3[1024]/F");
    }
    if(container->GetChannel3()){
   	tree -> Branch("wf4",&wf4,"wf4[1024]/F");
//        tree -> Branch("TimeChn4",&timeChn4,"timeChn4[1024]/F");
    }

    if(container->GetSaveType()==1)
    	gROOTFile = new TFile(command.c_str(), "RECREATE");

    /*Time Meassuring*/
    struct timeval begin, end, total;
    double mtime, seconds, useconds;    
    double lastevents  =0;
    double events=0;
    gettimeofday(&begin, NULL);
    gettimeofday(&total, NULL);

   /* Start Aquisition */
    printf(KGRN);
    std::cout << "Start Data Aquistion" << std::endl << std::endl;
    printf(RESET);

    int counter;	//General Purpose Counter
   while(events!=container->GetNumberOfEvens() && quit==0) {

      /* start board (activate domino wave) */

      b->StartDomino();

      while (b->IsBusy() && quit==0){
		gettimeofday(&end, NULL);
    		seconds  = end.tv_sec  - begin.tv_sec;
      		useconds = end.tv_usec - begin.tv_usec;
      		mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
		if(mtime>4000){
			/* print some progress indication */
			printf(KRED);
			std::cout <<"	No Trigger since a long time!!! " <<  std::endl;
			gettimeofday(&begin, NULL);
      			printf(RESET);
		}
		if (kbhit()) c = getch();
        	if (c == 'q' || c == 'Q'){ 
			if(container->GetSaveType()==1){
   				gROOTFile ->cd();
  				tree ->Write();
  				gROOTFile ->Close();
   			}
			delete drs;
			return 0;
		}
		if(container->GetTriggerType()==3){
                	usleep((1./(container->GetTriggerRate()))*1000000.);
			b->SoftTrigger();
		}
      }
      /* read all waveforms */
      b->TransferWaves(0, 8);

      /* read time (X) array of first channel in ns */
      b->GetTime(0, 0, b->GetTriggerCell(0), time_array[0]);

      /* decode waveform (Y) array of first channel in mV */
      b->GetWave(0, 0, wave_array[0]);

      /* read time (X) array of second channel in ns
       Note: On the evaluation board input #1 is connected to channel 0 and 1 of
       the DRS chip, input #2 is connected to channel 2 and 3 and so on. So to
       get the input #2 we have to read DRS channel #2, not #1. */
      b->GetTime(0, 2, b->GetTriggerCell(0), time_array[1]);

      /* decode waveform (Y) array of second channel in mV */
      b->GetWave(0, 2, wave_array[1]);

       /* read time (X) array of third channel in ns*/
      b->GetTime(0, 4, b->GetTriggerCell(0), time_array[2]);

      /* decode waveform (Y) array of third channel in mV */
      b->GetWave(0, 4, wave_array[2]);

      /* read time (X) array of fourth channel in ns*/
      b->GetTime(0, 6, b->GetTriggerCell(0), time_array[3]);

      /* decode waveform (Y) array of fourth channel in mV */
      b->GetWave(0, 6, wave_array[3]);

      /* Save waveform: X=time_array[i], Yn=wave_array[n][i] */
       for (i=0 ; i<1024 ; i++){
	  if(container->GetSaveType()==1){
		 wf1[i]=wave_array[0][i];
		 wf2[i]=wave_array[1][i];
		 wf3[i]=wave_array[2][i];
		 wf4[i]=wave_array[3][i];
		 timeChn1[i]=time_array[0][i];
		 timeChn2[i]=time_array[1][i];
		 timeChn3[i]=time_array[2][i];
		 timeChn4[i]=time_array[3][i];
	 }
       	 if(container->GetGraphicMode()){
		g1->SetBinContent(i,wave_array[0][i]);
		g2->SetBinContent(i,wave_array[1][i]);
		g3->SetBinContent(i,wave_array[2][i]);
		g4->SetBinContent(i,wave_array[3][i]);
       	 }
      }
      if(container->GetSaveType()==1)
           tree->Fill();
      events++;
      gettimeofday(&end, NULL);
      seconds  = end.tv_sec  - begin.tv_sec;
      useconds = end.tv_usec - begin.tv_usec;
      mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;

      if(mtime>1000){
        /* print some progress indication */
	printf(KCYN);
	std::cout <<"	DAQ-Rate: " << round(((events-lastevents)/mtime)*1000)<< "Hz" << " Board Temperature: " << round(b->GetTemperature()) << " Degrees" << " Total Events: " << events << std::endl;
	lastevents=events;
	gettimeofday(&begin, NULL);
       printf(RESET);
      }

	if(container->GetGraphicMode()){
	   if (kbhit()) c = getch();
            if (c == '+') {ChannelNum++;c=0;}
	  if (kbhit()) c = getch();
          if (c == '-'){ ChannelNum--;c=0;}
	  if(ChannelNum==1)
           g1->Draw();
	  if(ChannelNum==2)
           g2->Draw();
 	  if(ChannelNum==3)
	   g3->Draw();
	  if(ChannelNum==4)
	   g4->Draw();
          win->Modified();
          win->cd();
          win->SetSelected(win);
          win->Update();
	}
	if (kbhit()) c = getch();
        if (c == 'q' || c == 'Q'){ quit = 1; c=0;}

   }
   if(container->GetSaveType()==1){
   	gROOTFile ->cd();
  	tree ->Write();
  	gROOTFile ->Close();
   }
   gettimeofday(&end, NULL);
   seconds  = end.tv_sec  - total.tv_sec;
   useconds = end.tv_usec - total.tv_usec;
   printf(KGRN);
   std::cout << " Summary:" << std::endl;
   std::cout << " Meassured Events: " <<  events << std::endl;
   std::cout << " Meassuring Time: " << seconds<< "s" << std::endl;
   printf(RESET);
   /* delete DRS object -> close USB connection */
   delete drs;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/stat.h>
#include <unistd.h>
#include <unistd.h>
#include <iostream>
#include <sys/time.h>

//#include "global.h"
#include "DRS.h"
#include "xmlParser.h"
#include "DAQContainer.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"


//-------------------------------------------------------------------
// display xml parsing error message
int xml_error(char *node)
{
  char txt[120];
  printf(KRED);
  printf(txt,":::: WARNING: missing information in xml-file (%s) ::::",node);
  printf(RESET);
  return 0;
}

//-------------------------------------------------------------------
// reads the XML settings file
int xml_readsettings(char filename[100], DRSBoard *b,DAQContainer* c)
{
  int temp;  
  char txt[100];
  const char *xstr;
  txt[0]='\0';

  printf(KYEL);
  printf("Reading User Configuration XML-File %s\n\n",filename);
//  printf(RESET);

  // open the XML file -------------------------------------------------------
  XMLNode xMainNode=XMLNode::openFileHelper(filename,"settings");

  //printf("Settings File %s\n", filename);

  xstr=xMainNode.getAttribute("author");
  if (xstr) {
	strcpy(txt,xstr); 
	printf("  (Author: %s,",txt);	  
  } else xml_error((char*)"author"); 
  
  xstr=xMainNode.getAttribute("date");		
  if (xstr) {
	strcpy(txt,xstr);
	printf("  Date: %s)\n\n",txt);
  } else xml_error((char*)"date"); 

  // parse global DAQ settings -----------------------------------------------
  XMLNode xNode=xMainNode.getChildNode("global");
  printf("Global Settings:\n");
  
  xstr=xNode.getChildNode("source").getText();
  if (xstr) {
    strcpy(txt,xstr); 
    printf("  Source:      %s\n",txt); 
  } else xml_error((char*)"source");

  xstr=xNode.getChildNode("notes").getText();
  if (xstr) {
      strcpy(txt,xstr); 
     printf("  Comments:    %s\n",txt);
  } else xml_error((char*)"notes");

  xstr=xNode.getChildNode("path").getText();
  if (xstr) {
     strcpy(txt,xstr); 
     c->SetOutputDir(txt);
     printf("  Path:        %s\n",txt);

  } else xml_error((char*)"path");

  xstr=xNode.getChildNode("daq_version").getText();
  if (xstr) {
	strcpy(txt,xstr); 
        printf("  DAQ Version: %s\n",txt);
  } else xml_error((char*)"daq_version");

  xstr=xNode.getChildNode("nb_evts").getText();
  if (xstr) {
	strcpy(txt,xstr); 
	temp=atoi(txt); 
	c->SetNumberOfEvents(temp);
    if (strstr(txt, "-1")!=NULL) printf("  NoF Events:  infinite\n");
       		            else printf("  NoF Events:  %s\n",txt);
  } else xml_error((char*)"nb_evts");

  xstr=xNode.getChildNode("store_data").getText();
  if (xstr) {
    strcpy(txt,xstr); 
    temp=atoi(txt); 
    c->SetSaveType(temp);
    printf("  File Format: ");
    switch (temp) {
      case 0: printf("Data NOT stored\n"); break;
      case 1: printf("Write to root file\n"); break;
      case 2: printf("Write to binary file\n"); break;
    }	
  } else xml_error((char*)"store_data");

  xstr=xNode.getChildNode("nb_evts_per_file").getText();
  if (xstr) {
	strcpy(txt,xstr); 
	temp=atoi(txt); 
	c->SetNumberPerFile(temp);
        printf("  Events/File: %s\n",txt);
  } else xml_error((char*)"nb_evts_per_file");

  xstr=xNode.getChildNode("displaytime").getText();
  if (xstr) {
	strcpy(txt,xstr); 
         printf("  DisplayTime: %ss\n",txt);
  } else xml_error((char*)"displaytime");
    
  xstr=xNode.getChildNode("user").getText();
  if (xstr) {
	strcpy(txt,xstr); 
        printf("  User:        %s\n\n",txt); 
  } else xml_error((char*)"user");
  
  
  // parse global PSI-Board settings -----------------------------------------------
  xNode=xMainNode.getChildNode("DRS4").getChildNode("global");
  printf("PSI Board: Global settings:\n");
  
  //--- Active channels -------

  xstr=xNode.getChildNode("ch_0").getText();
  if (xstr) {
       strcpy(txt,xstr); 
       temp=atoi(txt); 
       c->SetChannel0(temp);
       printf("  Channel 0:     %s\n",txt); 	
  } else xml_error((char*)"ch_0");
 
 xstr=xNode.getChildNode("ch_1").getText();
  if (xstr) {
	strcpy(txt,xstr); 
        temp=atoi(txt); 
	c->SetChannel1(temp);
        printf("  Channel 1:     %s\n",txt); 	
  } else xml_error((char*)"ch_1");
 
  xstr=xNode.getChildNode("ch_2").getText();
  if (xstr) {
	strcpy(txt,xstr); 
        temp=atoi(txt); 
	c->SetChannel2(temp);
        printf("  Channel 2:     %s\n",txt); 	
  } else xml_error((char*)"ch_2");

  xstr=xNode.getChildNode("ch_3").getText();
  if (xstr) {
	strcpy(txt,xstr); 
        temp=atoi(txt); 
	c->SetChannel3(temp);
        printf("  Channel 3:     %s\n",txt); 	
  } else xml_error((char*)"ch_3");

  xstr=xNode.getChildNode("sampling_freq").getText();
  if (xstr) {
	strcpy(txt,xstr); 
	temp=atoi(txt); 
	/* set sampling frequency GHz*/	
	b->SetFrequency((temp/1000.), true);
        printf("  Frequency:     %s MHz\n",txt);
  } else xml_error((char*)"sampling_freq");

  xstr=xNode.getChildNode("voltage_range").getText();
  if (xstr) {
	strcpy(txt,xstr); 
 	temp=atoi(txt); 
    	printf("  Voltage Midpoint: %f V\n",temp/1000.);
	/* set input range to -0.5V ... +0.5V */
	b->SetInputRange(temp/1000.);
  } else xml_error((char*)"sampling_freq");

  xstr=xNode.getChildNode("internal_clock").getText();
  if (xstr) {
	strcpy(txt,xstr); 
	temp=atoi(txt);
	/* use following line to turn on the internal 100 MHz clock connected to all channels  */
	b->EnableTcal(temp);
        printf("  All Channels 100MHz:     %s \n\n",txt);
  } else xml_error((char*)"internal_clock");

// parse global PSI-Board Trigger settings -----------------------------------------------
 xNode=xMainNode.getChildNode("DRS4").getChildNode("trigger_settings");
 printf("PSI Board: Trigger settings:\n");

 xstr=xNode.getChildNode("trigger").getText();
  if (xstr) {
      strcpy(txt,xstr); 
      temp=atoi(txt); 
      c->SetTriggerType(temp);
      printf("  Trigger: ");
      switch (temp) {
      case 0: printf("External\n");
   	      /* enable transparent mode needed for analog trigger */
   	      b->SetTranspMode(0);
	      /* use following lines to enable the external trigger */
   	     if (b->GetBoardType() == 8) {        // Evaluaiton Board V4
             	b->EnableTrigger(1, 0);           // enable hardware trigger
             	b->SetTriggerSource(1<<4);        // set external trigger as source
             } else {                             // Evaluation Board V3
             	b->EnableTrigger(1, 0);           // lemo on, analog trigger off
                b->SetTriggerSource(1<<4);
    	     }
		 b->SetTriggerLevel(1.00);    	//TTL 1-5V
	     break;

      case 1: printf("Channel ");
	        /* enable transparent mode needed for analog trigger */
   	        b->SetTranspMode(1);
		xstr=xNode.getChildNode("SourceChn").getText();
		if (xstr) {
			strcpy(txt,xstr); 
			temp=atoi(txt);
			if (b->GetBoardType() >= 8) {           // Evaluaiton Board V4&5
      				b->EnableTrigger(1, 0);         // enable hardware trigger
      				b->SetTriggerSource(1<<temp);   // set CHx as source
   			} else if (b->GetBoardType() == 7) { 	// Evaluation Board V3
     				 b->EnableTrigger(0, 1);        // lemo off, analog trigger on
     				 b->SetTriggerSource(1<<temp);     // use CHx as source
   			}
     			printf(" %s\n",txt);
		}
                else xml_error((char*)"SourceChn");

		xstr=xNode.getChildNode("triggerlvl").getText();
		if (xstr) {
			strcpy(txt,xstr); 
			temp=atoi(txt);
			b->SetTriggerLevel((temp/1000.));            
     			printf("  Trigger Level: %smV\n",txt);
		}
		else xml_error((char*)"triggerlvl");
		break;

      case 2: printf("Individual Channel\n"); 
		/* enable transparent mode needed for analog trigger */
   	        b->SetTranspMode(1);
		b->SetTriggerSource(15);
		xstr=xNode.getChildNode("TriggerLvlCh_0").getText();
		if (xstr) {
			strcpy(txt,xstr); 
			temp=atoi(txt);
			b->SetIndividualTriggerLevel(1, (temp/1000.));
     			printf("  Trigger Level Channel 0: %smV\n",txt);
		}
                else xml_error((char*)"TriggerLvlCh_0");

		xstr=xNode.getChildNode("TriggerLvlCh_1").getText();
		if (xstr) {
			strcpy(txt,xstr); 
			temp=atoi(txt);
			b->SetIndividualTriggerLevel(2, (temp/1000.));
     			printf("  Trigger Level Channel 1: %smV\n",txt);
		}
                else xml_error((char*)"TriggerLvlCh_1");

		xstr=xNode.getChildNode("TriggerLvlCh_2").getText();
		if (xstr) {
			strcpy(txt,xstr); 
			temp=atoi(txt);
			b->SetIndividualTriggerLevel(3, (temp/1000.));
     			printf("  Trigger Level Channel 2: %smV\n",txt);
		}
                else xml_error((char*)"TriggerLvlCh_2");

		xstr=xNode.getChildNode("TriggerLvlCh_3").getText();
		if (xstr) {
			strcpy(txt,xstr); 
			temp=atoi(txt);
			b->SetIndividualTriggerLevel(4, (temp/1000.));
     			printf("  Trigger Level Channel 3: %smV\n",txt);
		}
                else xml_error((char*)"TriggerLvlCh_3");
		break;
      case 3: printf("Software\n");
		/* enable transparent mode needed for analog trigger */
   	        b->SetTranspMode(0);
		xstr=xNode.getChildNode("SoftwareTriggerRate").getText();
		if (xstr) {
			strcpy(txt,xstr); 
			temp=atoi(txt);
			c->SetTriggerRate(temp);
     			printf("  Trigger Rate: %sHz\n",txt);
		}
                else xml_error((char*)"SoftwareTriggerRate");
		break;
    }	

    xstr=xNode.getChildNode("triggerpolarity").getText();
		if (xstr) {
			strcpy(txt,xstr); 
			temp=atoi(txt);
			switch (temp) {
     				case 0: b->SetTriggerPolarity(false); break;       //Polarity break;
      				case 1: b->SetTriggerPolarity(true);  break;       //Polarity
   			}	
     			printf("  Trigger Polarity: %s\n",txt);
		}
    else xml_error((char*)"triggerpolarity");

  xstr=xNode.getChildNode("triggerdelay").getText();
		if (xstr) {
			strcpy(txt,xstr); 
			temp=atoi(txt);
		        b->SetTriggerDelayNs(temp);             // zero ns trigger delay
     			printf("  Trigger Delay: %sns\n\n",txt);
		}
    else xml_error((char*)"triggerdelay");



  } else xml_error((char*)"trigger");








  // ADC: parse waveform display options
  xNode=xMainNode.getChildNode("DRS4").getChildNode("graphics");
  
  xstr=xNode.getChildNode("ydisplay").getText();
  if (xstr) {
    strcpy(txt,xstr); 
    temp=atoi(txt);
    c->SetGraphicModeType(temp);
  } else xml_error((char*)"ydisplay");

  xstr=xNode.getChildNode("yaxis_low").getText();
  if (xstr) {
    strcpy(txt,xstr); 
    temp=atoi(txt);
    c->SetGraphicYLow(temp);
  } else xml_error((char*)"yaxis_low");
  
  xstr=xNode.getChildNode("yaxis_high").getText();
  if (xstr) {
    strcpy(txt,xstr); 
    temp=atoi(txt);
    c->SetGraphicYHigh(temp);
  } else xml_error((char*)"yaxis_high");

  return 0;
}

	/*
//-------------------------------------------------------------------
// stores a copy of the XML file with the data
int xml_storefile(char *filename, char *OutFileName)
{
  char cmnd[300];
  sprintf(cmnd,"cp %s %s.xml",filename,OutFileName);
  if (system(cmnd)!=0) errormsg(0,(char*)":::: WARNING: Could not save a copy of XML-file ::::");
  
  return 0;
}
*/


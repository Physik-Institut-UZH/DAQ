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

#include "xmlParser.h"
#include "VMEManager.h"
#include "DiscriminatorManager.h"
#include "ScalerManager.h"
#include "global.h"



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
int xml_readsettings(char filename[100],VMEManager* VME,DiscriminatorManager* disc, ScalerManager* scaler)
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

  // parse global XML settings -----------------------------------------------
  XMLNode xNode=xMainNode.getChildNode("global");
  printf("Global Settings:\n");
  
  xstr=xNode.getChildNode("notes").getText();
  if (xstr) {
      strcpy(txt,xstr); 
     printf("	Comments:	%s\n",txt);
  } else xml_error((char*)"notes");

  xstr=xNode.getChildNode("vmelink").getText();
  if (xstr) {
     strcpy(txt,xstr); 
     temp=atoi(txt); 
     printf("	VME Link:	%i\n",temp);
	VME->SetPCILink(temp);
  } else xml_error((char*)"vmelink");
  
  xstr=xNode.getChildNode("outpath").getText();
  if (xstr) {
     strcpy(txt,xstr); 
     printf("	Outpath:	%s\n",txt);
     scaler->SetPath(txt);
  } else xml_error((char*)"outpath");
  
    xstr=xNode.getChildNode("Scaler").getText();
  if (xstr) {
     strcpy(txt,xstr); 
     temp=atoi(txt); 
     printf("	Scaler Active:	%i\n",temp);
     scaler->SetScalerActive(temp);
  } else xml_error((char*)"Scaler");
  
  
  // parse Discriminator settings -----------------------------------------------
  xNode=xMainNode.getChildNode("Discriminator");
  printf("\nDiscriminator Settings:\n");

  xstr=xNode.getChildNode("address").getText();
  if (xstr) {
     strcpy(txt,xstr); 
     printf("	Discriminator Address:	%s\n",txt);
     disc->SetDiscAddress(txt);
  } else xml_error((char*)"address");
  
  xstr=xNode.getChildNode("SpecificThreshold").getText();
  if (xstr) {
     strcpy(txt,xstr); 
     temp=atoi(txt); 
     printf("	Specific Threshold:	%i\n",temp);
     if(temp==1){
		   xstr=xNode.getChildNode("thresholdpath").getText();
		   if (xstr) {
				strcpy(txt,xstr); 
				printf("	Specific Threshold Path: %s\n",txt);
		        disc->FillThresholds(txt);
		        scaler->SpecificThreshold();
		  }
		  else xml_error((char*)"SpecificThreshold");
	  }
  } else xml_error((char*)"SpecificThreshold");

  if(temp==0){
 	 for(int i=0;i<16;i++){
		 char channel[300];
		 sprintf(channel,"t%i",i);
		 xstr=xNode.getChildNode(channel).getText();
     	  	 if (xstr) {
		 	strcpy(txt,xstr); 
		 	temp=atoi(txt); 
		 	printf("	Channel %i Threshold: %i\n",i,temp);	
		 	disc->SetTreshold(temp);	 
     	  	 } else xml_error((char*)channel);
  	}
  }
  
  
  // parse Scaler settings -----------------------------------------------
  xNode=xMainNode.getChildNode("Scaler");
  printf("\nScaler Settings:\n");


  xstr=xNode.getChildNode("address").getText();
  if (xstr) {
     strcpy(txt,xstr); 
     printf("	Scaler Address:	%s\n",txt);
     scaler->SetDiscAddress(txt);
  } else xml_error((char*)"address");
  
    xstr=xNode.getChildNode("rate").getText();
  if (xstr) {
     strcpy(txt,xstr); 
     temp=atoi(txt); 
     printf("	Aquisition Rate:	%is\n",temp);
     scaler->SetRate(temp);
  } else xml_error((char*)"rate");
  
    xstr=xNode.getChildNode("cycles").getText();
  if (xstr) {
     strcpy(txt,xstr); 
     temp=atoi(txt); 
     printf("	Aquisition cycles:	%i\n",temp);
     scaler->SetCycles(temp);
  } else xml_error((char*)"cycles");
  

    for(int i=0;i<16;i++){
	 char channel[300];
	 sprintf(channel,"s%i",i);
	 xstr=xNode.getChildNode(channel).getText();
     if (xstr) {
		 strcpy(txt,xstr); 
		 temp=atoi(txt); 
		 printf("	Channel %i Active: %i\n",i,temp);	
		 scaler->SetActive(temp);	 
     } else xml_error((char*)channel);
  }

 
 printf("\n");
 printf(RESET);
  


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


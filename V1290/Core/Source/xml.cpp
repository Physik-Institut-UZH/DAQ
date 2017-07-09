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
#include "TDCManager.h"
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
int xml_readsettings(char filename[100],VMEManager* VME,TDCManager *TDC)
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
     TDC->SetPath(txt);
  } else xml_error((char*)"outpath");
  
  
   // parse TDC settings -----------------------------------------------
  xNode=xMainNode.getChildNode("TDC");
  printf("\n	TDC Settings:\n");

  xstr=xNode.getChildNode("address").getText();
  if (xstr) {
     strcpy(txt,xstr); 
     printf("	TDC Address:	%s\n",txt);
     TDC->SetDiscAddress(txt);
  } else xml_error((char*)"address");
 
 
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


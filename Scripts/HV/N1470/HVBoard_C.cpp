//---------------------------------------------------------------------------
//// programme: HVBoard.cpp
//// Purpose: read out settings of HV Board and set properties to 
////  it via USB serial port
////  using:
////      rs232.h written by Andreas James to set up serial port comunication
////      XML parser from Marc Schumann
//// Compiler: gcc C++ compiler version 4.5.1
//// Author: Payam Pakarha <ppakar@physik.uzh.ch> 
//// Date: 21.05.2015
//// Version: 1.0
////---------------------------------------------------------------------------
#define _BSD_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <dirent.h>
#include "rs232.h"
#include <ctype.h>
#include <iostream>
#include <fstream>

using namespace std;


char* SendCommand(const char *command);
int ReadConfig(void);
char* GetProgramPath(char *astr);


char configfile[255];    /* for configuration */

char commandtosend[100];

int cport_nr,   /* Com port parameters */
    bdrate,
    debugmode,
    starttime,
    modulenumber,
    channelnumber,
    datacount = 0;
static float samplingrate;  /* samplingrate in seconds*/

string RorW;
char v_set_str[32], v_mon_str[32], i_mon_str[32];
int garbage;
float v_set_float, v_mon_float, i_mon_float;
double voltage= 0.;
string yn ="n";

int main(int argc, char * argv[]){
	FILE *logFile = fopen("/data/LArSetup/Julien/Raw_Data/Run3/T300K/DarkCount_2", "a");
	//Read the configuration

	modulenumber =0;
	sprintf(configfile, "%s/configuration.cfg", GetProgramPath(argv[0]));
	if(ReadConfig() == 0){
		/* Open Com Port */
		if(OpenComport(cport_nr, bdrate)){
			fprintf(logFile, "\nCan not open comport ttyS%i\n",cport_nr);
			return(0);
		}
		else{
			fprintf(logFile,"\nComport ttyS%i opened\n",cport_nr);
		}
		for(int chan=1;chan<2;chan++){
			fprintf(logFile, "Channel: \t %d \n",chan);
			sscanf(argv[chan+1],"%lf",&voltage);
			sprintf(commandtosend, "$BD:0%i,CMD:SET,CH:%i,PAR:VSET,VAL:%04.1f\r\n",modulenumber,chan,voltage);
			fprintf(logFile, "%s\n", commandtosend);
			cout << commandtosend   << endl;
			sprintf(v_set_str, "%s", SendCommand(commandtosend));
		}
		fprintf(logFile, " New voltage set has beem applied \n");
	}
}

char* SendCommand(const char *command)
{
  unsigned char buf[4096];          /* buffer for serial data */
  int n=0;
  int nowanswer=1;

  cprintf(cport_nr, command); /* Send a command to com port (Read measurement data) */
  usleep(200000);
  buf[0]=0;
  n = PollComport(cport_nr, buf, 4095); /* Get data from com port */
  buf[n]=0;
  while(n==0)
  {
    if (nowanswer%500==0) cprintf(cport_nr, command);
    usleep(200000);
    buf[0]=0;
    n = PollComport(cport_nr, buf, 4095); /* Get data from com port */
	buf[n]=0;
    if (nowanswer >= 5000) /* device unplugged or off */
    {
       //printf("No answer from device for more than 5 seconds!\n");
       return ("-1.0\n");
       break;
    }
    else
    {
      nowanswer++;
    }
  }
  if (strstr((reinterpret_cast<char*>(buf)),".......")) sprintf((reinterpret_cast<char*>(buf)),"-1.0\n");
  if (strstr((reinterpret_cast<char*>(buf)),".....")) sprintf((reinterpret_cast<char*>(buf)),"-1.0\n");
  buf[n] = 0;   /* always put a "null" at the end of a string! */
  printf("BUF=%s\n",buf);
  return strdup ((reinterpret_cast<char*>(buf)));
}


int ReadConfig(void)
{
    /* Set default values in case som lines of or the whole .cfg file gets deleted */
    cport_nr = 0;
    bdrate = 9600;
    samplingrate = 120;
    debugmode = 0;

    /* read config. */
    char cfgbuff[255];
    if (cfgbuff[0]=='[') /* ignore lines without '[' at the beginning (comments etc) */
    {
        //i = 0;
        for( int i = 0; cfgbuff[i] != ']'; i++) /* convert to lowercase */
        {
            cfgbuff[i] = tolower(cfgbuff[i]);
        }
        if (strstr(cfgbuff,"[comport]=")) sscanf(cfgbuff,"[comport]=%i" ,&cport_nr);
        if (strstr(cfgbuff,"[baudrate]="))  sscanf(cfgbuff,"[baudrate]=%i" ,&bdrate);
        if (strstr(cfgbuff,"[debugmode]="))    sscanf(cfgbuff,"[debugmode]=%i" ,&debugmode);
        if (strstr(cfgbuff,"[modulenumber]="))    sscanf(cfgbuff,"[modulenumber]=%i" ,&modulenumber);
        if (strstr(cfgbuff,"[channelnumber]="))    sscanf(cfgbuff,"[channelnumber]=%i" ,&channelnumber);
    }

    /* show some config info*/
    printf("\nFrom %s:\nComport     = ttyS%i\nBaudrate    = %ibps\nSamplingrate= %.2fs\nDebugMode   =  %i\n",configfile,cport_nr,bdrate,samplingrate,debugmode);
    return(0);
    if (!debugmode) usleep(3000000); //because screen gets cleared when collecting data
}




char* GetProgramPath(char *astr) 
{
    char path_save[255];
    char abs_exe_path[255];
    char *p;

    if(!(p = strrchr(astr, '/')))
        getcwd(abs_exe_path, sizeof(abs_exe_path));
    else
    {
        *p = '\0';
        getcwd(path_save, sizeof(path_save));
        chdir(astr);
        getcwd(abs_exe_path, sizeof(abs_exe_path));
        chdir(path_save);
    }

    //printf("Absolute path to executable is: %s\n", abs_exe_path);
    return strdup (abs_exe_path);
}

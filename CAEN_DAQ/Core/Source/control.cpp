//-------------------------------------------------------------------
//  control.cpp   --   miniDAX: DAQ for XENON
// 
//  the control library contains functions to control the miniDAX program
//
//  18.03.2008 MS  (12.04.2008)
//  modified:
//    13.06.2008 MS	Changed Trigger Rate to DAQ Rate
//    24.06.2008 MS	Store DAQ rate in log file (cnt[13])
//			start xed-files with event 0 (was -1)	
//    07.07.2008 MS	create directory for data automatically
//    24.07.2008 MS	include more warning messages (no data read etc.)
//  -----------------
//    26.08.2008 MS	miniDAX version
//    29.11.2010 MS	cosmetics
//    30.05.2015 JW A lot of changes in order to make everything more easy
		


//-------------------------------------------------------------------

extern "C" {


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/stat.h>
#include <unistd.h>
#ifdef WIN32
    #include <sys/timeb.h>
    #include <conio.h>
#else
    #include <sys/time.h>
#endif
int sand=-1;
int jul=1;
char OutPutFolder[100]="";
}

#include "global.h"
#include "CAENVMElib.h"
#include "adc.h"
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"
#include <TH1D.h>
//==================== AB ========================//
#include <Riostream.h>
#include <TTree.h>
#include <TBranch.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TCanvas.h>
int control_printlog(FILE *logfile, int time, char *text);

//-------------------------------------------------------------------
// returns time in milliseconds
int32_t get_time()
{
    int32_t time_ms;

#ifdef WIN32
    struct _timeb timebuffer;

    _ftime( &timebuffer );
    time_ms = (int32_t)timebuffer.time * 1000 + (int32_t)timebuffer.millitm;
#else
    struct timeval t1;
    struct timezone tz;

    gettimeofday(&t1, &tz);
    time_ms = (t1.tv_sec) * 1000 + t1.tv_usec / 1000;
#endif

    return time_ms;
}

//-------------------------------------------------------------------
// changes the screen output color
void col(char *str)
{
  if (strstr(str, "black")!=NULL) printf("\033[22;30m");
  if (strstr(str, "red")!=NULL) printf("\033[22;31m");
  if (strstr(str, "green")!=NULL) printf("\033[22;32m");
  if (strstr(str, "brown")!=NULL) printf("\033[22;33m");
  if (strstr(str, "blue")!=NULL) printf("\033[22;34m");
  if (strstr(str, "magenta")!=NULL) printf("\033[22;35m");
  if (strstr(str, "cyan")!=NULL) printf("\033[22;36m");
  if (strstr(str, "gray")!=NULL) printf("\033[01;30m"); // only dark grey!
  if (strstr(str, "dark gray")!=NULL) printf("\033[01;30m");
  if (strstr(str, "light red")!=NULL) printf("\033[01;31m");
  if (strstr(str, "light green")!=NULL) printf("\033[01;32m");
  if (strstr(str, "yellow")!=NULL) printf("\033[01;33m");
  if (strstr(str, "light blue")!=NULL) printf("\033[01;34m");
  if (strstr(str, "light magenta")!=NULL) printf("\033[01;35m");
  if (strstr(str, "light cyan")!=NULL) printf("\033[01;36m");
  if (strstr(str, "white")!=NULL) printf("\033[01;37m");
}

//-------------------------------------------------------------------
// print something in logfile (without knowing the logfile handler)
int control_log(FILE *file, int mode, char *s)
{
  static FILE *lf;

  if (mode==0) { // initialize function; store file handler
    lf=file;
  } else {	 // write something to logfile
    control_printlog(lf,1,s);
  }

  return 0;
}

//-------------------------------------------------------------------
// Print Error Message on Screen
int errormsg(int flag,char *s)
{
  printf(KRED);
  printf("%s\n",s);
  printf(RESET);

  // write to logfile
  if (flag) {
    FILE *temp = new FILE;
    control_log(temp,1,s);
  }

  return 0;
}

//-------------------------------------------------------------------
// Print the Start Screen
int control_printtitle(char *SwRelease)//, digitizer& adc)
{
  printf(KGRN);
  printf("\n");
  printf("*****************************************************************\n");
  printf("                 ZDAQ  --  DAQ for UZH                           \n"); 
  printf("                    version: %s                                  \n",SwRelease);
  printf("*****************************************************************\n");
  printf(RESET);
  return 0;
}

//-------------------------------------------------------------------
// display help menu for oscilloscope mode
int control_scopehelp()
{
  col((char *)"green"); 
  printf("Quick Reference for Oscilloscope Mode:\n");
  printf(KYEL);
  printf("  In oscilloscope mode (-g or -m), you can use the following keys\n");
  printf("  in order to change the display during run time:\n\n");
  printf("  <+>  switch to next PMT/channel\n");
  printf("  <->  switch to previous PMT/channel\n");
  printf("  <s>  store current waveform to root-file\n\n");
  printf("  <0>  standard display mode for y-axis\n");
  printf("  <1>  manual display mode for y-axis\n");
  printf("  <2>  automatic display mode for y-axis\n");
  printf("  <3>  standard display mode for x-axis\n");
  printf("  <4>  manual display mode for x-axis\n");
  printf("  <5>  automatic display mode for x-axis\n\n");
  printf("  in manual display mode only:\n");
  printf("  <u/U> 'up':    increase lower/upper y-display limit\n");
  printf("  <d/D> 'down':  decrease lower/upper y-display limit\n");
  printf("  <r/R> 'right': increase lower/upper x-display limit\n");
  printf("  <l/L> 'left':  decrease lower/upper x-display limit\n\n");
  printf("  <b>  calculate baseline characteristics\n");
  printf("  <o>  show this help menu\n");
  printf(RESET);
  return 0;
}

//-------------------------------------------------------------------
// returns a string with current date and time
int control_gettimestring(char *timestr, char format)
{
  time_t curtime;
  struct tm *loctime;
  int ret=0;

  curtime = time (NULL);		// Get the current time. 
  loctime = localtime (&curtime);	// Convert it to local time representation. 

  if(sand==-1){
  	if (format=='f')
   	  strftime(timestr, 100, "%y%m%d_%H%M", loctime);	// formatted print for filenames
  }
  else{
      if (format=='f')
	 sprintf(timestr, "%d", sand);
  }
  if (format=='l')
    strftime(timestr, 100, "%a %b %d %Y, %X", loctime);	// long format for logfile
  if (format=='s')
    strftime(timestr, 100, "%X", loctime);	// short format for logfile
  if (format=='u') 
    ret=timegm(loctime);		// Unix Time Format

  srand(curtime);			// random generator seed
  return ret;
}

//-------------------------------------------------------------------
// generates logfile
int control_openlog(FILE **logfile, char *FileName, char *SwRelease)
{
  char LogFileName[100];
  char timestr[100];
  
  sprintf(LogFileName,"%s.log",FileName);	    
  *logfile=fopen(LogFileName,"w");
  if (*logfile==NULL) {
    errormsg(0,(char *)":::: ERROR: cannot open log file! ::::\n");
    return -1; 
  } 	
  
  control_gettimestring(timestr,'l');
  fprintf(*logfile,"**Logfile********************************************************\n");
  fprintf(*logfile,"                 ZDAQ -- DAQ for UZH   				             \n"); 
  fprintf(*logfile,"                    version: %s                                  \n",SwRelease);
  fprintf(*logfile,"*****************************************************************\n");  
  fprintf(*logfile,"program started: %s\n",timestr);
  return 0;
}

//-------------------------------------------------------------------
// print to logfile
int control_printlog(FILE *logfile, int time, char *text)
{
  char timestr[100]="";
  
  if (time) {
    control_gettimestring(timestr,'s');
    fprintf(logfile,"%s: %s\n",timestr,text);
  } else fprintf(logfile,"%s\n",text);
  
  return 0;
}

//-------------------------------------------------------------------
// printf counter values to logfile 
int control_cnts2log(FILE *logfile, scaler& s)
{
  char str[100];
  float delta=(s.stop-s.old)/1000.;
  int now;

  sprintf(str,"Scaler Values: (Delta = %.3f s)",delta);
  control_printlog(logfile, 1, str);
 
  now=control_gettimestring(str, 'u');

  fprintf(logfile, "CNT0 %d %.3f ",now, delta);
  for (int i=0; i<8; i++) fprintf(logfile,"%9u ", s.cnt[i]); fprintf(logfile,"\n");
  fprintf(logfile, "CNT1 %d %.3f ",now, delta);
  for (int i=8; i<16; i++) fprintf(logfile,"%9u ", s.cnt[i]); fprintf(logfile,"\n");
  fprintf(logfile, "CNT2 %d %.3f ",now, delta);
  for (int i=16; i<24; i++) fprintf(logfile,"%9u ", s.cnt[i]); fprintf(logfile,"\n");
  fprintf(logfile, "CNT3 %d %.3f ",now, delta);
  for (int i=24; i<32; i++) fprintf(logfile,"%9u ", s.cnt[i]); fprintf(logfile,"\n");  
  
  // timing with external counter
  s.totaltime+=s.cnt[s.totalcnt]/1.e6;
  s.lifetime+=s.cnt[s.lifecnt]/1.e6;
  s.deadtime+=s.cnt[s.deadcnt]/1.e6;
  
  return 0;
}

//-------------------------------------------------------------------
// Process command line options
int control_cmdline( int argc, char *argv[], char *envp[], int& time, u_int32_t& NoE, 
	 	     var_graphics& graphics, int& baseline, digitizer& adc, 
		     char *OutFileName, char *XmlFileName)
{
  extern char *optarg;          /* options processing */
  int c;                        /* dummy for options */
  int errflag=0;                /* error flag */
  int someArgs=0;
  double x;
  
  // initialize variables
  time=0;
  NoE=0;
  graphics.on=0;
  baseline=0;
  adc.BoardInfo=0;  
  //  search for possible options 
  while ( ( c = getopt( argc, argv, "t:n:f:g:m:j:x:s:ibahow" ) ) != -1 )  {
      someArgs=1;
      switch ( c ) {
        case 'f': 
          sprintf(OutFileName,"%s",optarg);
          break;
        case 'x': 
          sprintf(XmlFileName,"%s",optarg);
          break;
        case 's':
	  sand=atoi(optarg);
	  //sprintf(sand,"%s",optarg.);
          break;
	case 'j':
	  sprintf(OutPutFolder,"%s",optarg);
	  jul=-1;
	  break;
        case 't': 
          time = atoi( optarg );
          if ( time <= 0 ) { errormsg(0,(char *)"Measuring Time must be greater than 0"); errflag++; }
          break;
        case 'n': 
          NoE = atoi( optarg );
          if ( NoE <= 0 ) { errormsg(0,(char *)"Number of Events must be greater that 0"); errflag++; }
          break;
        case 'm': 
          graphics.on = 1;
	  x=atof(optarg);
	  graphics.module=(int)x;
  	  for (int i=0; i<9; i++) 
	    if ((x-graphics.module)*10>(i-0.1) && (x-graphics.module*10)<(i+0.1)) graphics.channel=i;
          if ( graphics.module < 0 ) { errormsg(0,(char *)":::: ERROR: Board identifier must be positive ::::"); errflag++;  }
	  graphics.pmtNb=-1;
          break;
	case 'g':
          graphics.on = 1;
	  graphics.pmtNb=atoi(optarg);
          if ( graphics.pmtNb < 1 ) { errormsg(0,(char *)":::: ERROR: PMT number must be positive ::::"); errflag++;  }
          break;
        case 'i': 
          adc.BoardInfo = 1;
          break;
        case 'b': 
          baseline = 1;		// baseline adjustment
          break;
    	case 'a':
          baseline = 2;		// baseline analysis
          break;
        case 'h': 
          errflag++;
          break;	  
        case 'w':
          // don't take this too serious (-:
	  errormsg(0,(char *)"missed WIMPs protocol:");
	  int i;
	  char txt[100];
	  control_gettimestring(txt,'f');
          i = (int)((float)rand()/2147483647.*10.);
          printf("  You missed %d WIMPs since stopping DAQ.\n  Better you start it again...\n",i);
	  exit(0);	
	  break;
        case 'o':
	  control_scopehelp();
	  exit(0);
        default: 
          errflag++; 
          break;
        }
  }
  if (errflag || someArgs==0) {
      printf(KRED);
      printf("usage: %s [-f file|-x file|-t time|-n evts|-g p|-m m.c|-i|-b|-a|-h]\n", argv[0] );
      printf("\t-f write to file_*.xen \n" );
      printf("\t-x read settings from XML-file file\n");
      printf("\t-t measure for time seconds\n" );
      printf("\t-n measure evts events\n" );
      printf("\t-g Oscilloscope: display PMT p (SLOW!)\n");
      printf("\t-m Oscilloscope: display ADC module.channel (SLOW!)\n" );
      printf("\t-i displays hardware information\n" );
      printf("\t-b adjust baselines automatically\n" );
      printf("\t-a analyze baseline (mean, RMS, spikes)\n");
      printf("\t-o show help for oscilloscope mode\n");
      printf("\t-h display this help\n\n" );
      exit( 2 );
      printf(RESET);
  }
        
  if (graphics.on) errormsg(0,(char *)":::: WARNING: Graphics output is slow! ::::\n"); 
      
  // wait to display program title
  sleep(1);    
  return 0;
}

//-------------------------------------------------------------------
// Integer form of power function
int power(int base, int exponent)
{
  int result=1;
  for (int i=0; i<exponent; i++) result=result*base;
  return result;
}	


//-------------------------------------------------------------------
// Read configuration file and set up VME connection
// returns -1 on error
int control_configfile(	char *ConfigFileName, crate& crt,
			digitizer& adc, scaler& s, 
			multipurposemodule& mp, int& WriteToFile)	
{
  FILE *f_ini;
  char str[100];
  int link, bdnum;
  u_int32_t addr, dummy;
  u_int32_t data;
  char tmp[100];
  CVBoardTypes BType;
  int board;            
  int32_t temp_long;
  int i;
  int pmtNb, module, channel;
  
  if ( (f_ini = fopen(ConfigFileName, "r")) == NULL ) {
      sprintf(str,":::: Can't open Configuration File %s ::::", ConfigFileName);
      errormsg(0,str);
      return -3;
  }
  else printf("Reading Configuration File %s\n", ConfigFileName);
  
  while(!feof(f_ini)) {
      fscanf(f_ini, "%s", str);
      if (str[0] == '#')
          fgets(str, 1000, f_ini);
      else
          {
          // LINK: Open VME master
          if (strstr(str, "LINK")!=NULL) {
              fscanf(f_ini, "%s", tmp);
              if (strstr(tmp, "V1718")!=NULL)
                  BType = cvV1718;
              else if (strstr(tmp, "V2718")!=NULL)
                  BType = cvV2718;
              else {
                  errormsg(0,(char *)":::: ERROR: Invalid VME crate controller type ::::");
                  return -1;
              }
              fscanf(f_ini, "%d", &link);
              fscanf(f_ini, "%d", &bdnum);
              if (CAENVME_Init(BType, link, bdnum, &temp_long) != cvSuccess) {
                  errormsg(0,(char *)":::: ERROR: VME connection cannot be established ::::");
                  return -1;
              }
	      else {
		  crt.handle[crt.NumberOfCrates]=temp_long;
 		  crt.NumberOfCrates++;
		  if (crt.NumberOfCrates>CRATE_MAX) {
		    errormsg(0,(char *)":::: ERROR: too many crates ::::\n");
		    return -1;
		  }		  
	      }
          }

          // Write Data to output Files (0=Don't write, 1=binary, 2=text file, 3=Xe100)
          if (strstr(str, "WRITE_TO_FILE")!=NULL) {
              fscanf(f_ini, "%d", &WriteToFile);
		if (WriteToFile<0 || WriteToFile>8) { // DM added 8
	          WriteToFile=8; // DM set to 8
		  errormsg(0,(char *)":::: WARNING: invalid WRITE_TO_FILE option; use ROOT tree format option 8 :::");
		}
	  }

          // Readout Blt Size (in Bytes) 
          if (strstr(str, "BLT_SIZE")!=NULL)
              fscanf(f_ini, "%d", &adc.BltSize);

          // Base Addresses
          if (strstr(str, "BASE_ADDRESS")!=NULL) {
                fscanf(f_ini, "%x", (int *)&dummy);
		if (adc.NumberOfADCs==0) {
		    adc.Address[0]=dummy;
		    fscanf(f_ini, "%d", &adc.Id[0]);
		} else {
		    for (int i=0; i<adc.NumberOfADCs; i++) 
		       if (adc.Address[i]==dummy) {
		         errormsg(0,(char *)":::: ERROR: two identical BaseAddresses given in Config-file! ::::");
			 return -1;
		       }
		    adc.Address[adc.NumberOfADCs]=dummy;		    
		    fscanf(f_ini, "%d", &adc.Id[adc.NumberOfADCs]);
		} 
   	        fscanf(f_ini, "%d", &i);
		if (i>crt.NumberOfCrates-1) {
		  sprintf(str, ":::: ERROR: Crate %d (board %3d) does not exist! ::::",i,adc.Id[adc.NumberOfADCs]);
		  errormsg(0,str);
		  return -1;
		} else adc.Crate[adc.NumberOfADCs]=i;
		if (adc.BoardInfo) printf("  Module %3d (%d): BaseAdress[%2d] = 0x%08X\n",
		  adc.Id[adc.NumberOfADCs],adc.Crate[adc.NumberOfADCs],
		  adc.NumberOfADCs,adc.Address[adc.NumberOfADCs]);
  	        
		adc.NumberOfADCs++;		      
		if (adc.NumberOfADCs>ADC_MAX) {
		    errormsg(0,(char *)":::: ERROR: too many ADC modules ::::");
		    return -1;
		}
  	  }
	   
	  // ADC Trigger Latency
	  if (strstr(str,"TRIG_LATENCY")!=NULL) {
	    fscanf(f_ini, "%d", &adc.TriggerLatency);
	  }
	   	  	  		
          // Generic VME Write
	  // this is written to all of the boards
          if (strstr(str, "WRITE_REGISTER")!=NULL) {
              fscanf(f_ini, "%x", (int *)&addr);
              fscanf(f_ini, "%x", (int *)&data);
	      for (int i=0; i<adc.NumberOfADCs; i++) {
		  if (adc_writereg(crt,adc,i,addr,data)<0) return -1;
	      }  
          }
	    
	    // generic VME Write to individual module 
	    // this is only written to the module specified
	    // usage: WRITE_MODULE <module> <address> <data>
          if (strstr(str, "WRITE_MODULE")!=NULL) {
	      	fscanf(f_ini, "%d", &board);
		
		// get the BaseAddress from the board identifier
		int i=0;
		while (i<adc.NumberOfADCs+1) {		
		  if (board==adc.Id[i]) break;
		  i++;
		}
                fscanf(f_ini, "%x", (int *)&addr);
                fscanf(f_ini, "%x", (int *)&data);
		if (i<adc.NumberOfADCs) {
		  if (adc_writereg(crt,adc,i,addr,data)<0) return -1;
                } else { 
		  if ((addr&0x00000F00)==0) {
		    sprintf(str, ":::: WARNING: unknown board identifier %d ::::",board);
		    errormsg(0,str);
		  }  
		}
		if (addr==0xEF34) {		// stop program after Configuration ROM update
		    sprintf(str, ":::: Configuration ROM updated, board %d ::::",board);
		    errormsg(0,str);
		    return -2;
		}
          }

	  // read PMT to channel mapping
	  if (strstr(str, "PMT")!=NULL) {
 	    	fscanf(f_ini, "%d", &pmtNb);
		fscanf(f_ini, "%d", &module);
		fscanf(f_ini, "%d", &channel);
		if (module<0 || module>ADC_MAX-1 || channel<0 || channel>7) {
		    sprintf(str, ":::: WARNING: error in PMT mapping (PMT %d %d %d) ::::",pmtNb,module,channel);
		    errormsg(0,str);		  
		}
		else {
 		    if (adc.pmt[module][channel]<0) {
  		      adc.pmt[module][channel]=pmtNb;
		      if (pmtNb>adc.pmtmax) adc.pmtmax=pmtNb;
		      // generate channel mask
		      adc.mask[module]+=power(2,channel);
 		    } else {
		      sprintf(str, ":::: WARNING: channel %d.%d already occupied with PMT %d ::::",
				module,channel,adc.pmt[module][channel]);
		      errormsg(0,str);		  
		    } 
		}
		sprintf(str,"dummy string");
	  }	    
      }
  }
  fclose (f_ini);    
      
  return 0;
}


//-------------------------------------------------------------------
// pre-defined configurations for automatic baseline determination
// returns -1 on error
int control_configbaseline(crate& crt, digitizer& adc)
{  
  for (int i=0; i<adc.NumberOfADCs; i++) {
    // digitize everything, no ZLE
    if (adc_writereg(crt,adc,i,ChannelConfigReg,0x50)<0) return -1;
        
    // shorten the time window to 0x6 = 8192 Samples
    if (adc_writereg(crt,adc,i,BlockOrganizationReg,0x5)<0) return -1;  
  }
  return 0;
}


//-------------------------------------------------------------------
// open file to store data
int control_openfile(FILE **ofile, char *FileName, int& FileCounter, int WriteToFile, 
			digitizer& adc, FILE *logfile)
{    
  char OutFileName[100];
  char text[120];
  u_int32_t header[5];
  
	sprintf(OutFileName,"%s_%05d.xen",FileName,FileCounter);
	*ofile=fopen(OutFileName,"w");
	if (*ofile==NULL) {
		errormsg(1,(char *)":::: ERROR: cannot open data file! ::::");
		return -1; 
	} 	
  
			
  // write a header to the file
  header[0]=3; 			// length of header (including this word)
  header[1]=adc.NumberOfADCs;	// Number of ADCs read
  header[2]=adc.NbSamplesEvt;	// maximal length of Event
  if (adc.ZLE==1) header[2]+=0x80000000;	// ZLE activated
  if (WriteToFile==1) fwrite(header, sizeof(u_int32_t), header[0], *ofile);
  if (WriteToFile==2) for (int i=0; i<(int)header[0]; i++) fprintf(*ofile,"%d\n",header[i]);
	    
  printf("Write to file: %s\n",OutFileName); 	      
  sprintf(text,"data file: %s",OutFileName);
  control_printlog(logfile,1,text);
  
  return 0;
}

// ======================== ROOT file for mode 7, DM version ===============================//
 
int control_openrawrootfile( TFile **orootfile, TTree **t1, char *FileName, int& FileCounter, int WriteToFile, digitizer& adc, FILE *logfile,int wf0[],int wf1[],int wf2[],int wf3[],int wf4[],int wf5[],int wf6[],int wf7[], double& freq)
{    
  	char OutFileName[100];
  	char text[120];
  
  	sprintf(OutFileName,"%s_%05d.root",FileName,FileCounter);	    
	*orootfile = new TFile(OutFileName,"RECREATE");
	*t1 = new TTree("t1","t1");
//	(*t1)->Branch("freq", &freq, "freq/D");
	if (adc.channel[0]) (*t1)->Branch("wf0", wf0, TString::Format("wf0[%i]/I", adc.EventLength));
	if (adc.channel[1]) (*t1)->Branch("wf1", wf1, TString::Format("wf1[%i]/I", adc.EventLength));
	if (adc.channel[2]) (*t1)->Branch("wf2", wf2, TString::Format("wf2[%i]/I", adc.EventLength));
	if (adc.channel[3]) (*t1)->Branch("wf3", wf3, TString::Format("wf3[%i]/I", adc.EventLength));
	if (adc.channel[4]) (*t1)->Branch("wf4", wf4, TString::Format("wf4[%i]/I", adc.EventLength));
	if (adc.channel[5]) (*t1)->Branch("wf5", wf5, TString::Format("wf5[%i]/I", adc.EventLength));
	if (adc.channel[6]) (*t1)->Branch("wf6", wf6, TString::Format("wf6[%i]/I", adc.EventLength));
	if (adc.channel[7]) (*t1)->Branch("wf7", wf7, TString::Format("wf7[%i]/I", adc.EventLength));


	if (orootfile==NULL) {
		errormsg(1,(char*)":::: ERROR: cannot open ROOT file! ::::");
    	return -1; 
	}
	
	printf("Write to file: %s\n",OutFileName); 	      
	sprintf(text,"data file: %s",OutFileName);
	control_printlog(logfile,1,text);

	return 0;
}

//-------------------------------------------------------------------
// generate file with basic information to be read by Slow Control
int control_scfile(int time, float average, int stopped)
{
  FILE *scfile;
  char txt[100];

  scfile=fopen(SC_FILENAME,"w");
  if (scfile!=NULL) {
    if (stopped) {
      fprintf(scfile,"stopped\n");
    } else {
      control_gettimestring(txt,'s');
      fprintf(scfile,"%s\n",txt);
      fprintf(scfile,"AveragingTime %d\n",time);
      fprintf(scfile,"Average %f\n",average);
    }
    fclose(scfile);
  }
  else 
    errormsg(1,(char *)":::: WARNING: Cannot create file to pass information to Slow Control! ::::");

  return 0;
}


//-------------------------------------------------------------------
// calculate and print count and trigger rates
int control_calcrate(timeandtrigger& t, scaler &s, int FileEvtCounter, double& freq)
{
  int32_t ElapsedTime;
  float TPrate, TRGrate, av10;
  static int FileECOld;
  static int nodata;
  static int display;
  static float Rate, Trgs;
			
  t.CurrentTime = get_time(); // Time in milliseconds
  ElapsedTime = t.CurrentTime - t.PreviousTime;
  freq= 0 ;
  if (ElapsedTime > 1000) {
    t.MeasSeconds++;
    s.timecnt++;
    	    
    // init static variables when the function is called first
    if (t.MeasSeconds==1) {
      FileECOld=-1;
      nodata=0;
      display=0;
      Rate=0.;
      Trgs=0.;
    }
    display++;
	    
    TPrate = ((float)(t.totnb - t.last_totnb) / ElapsedTime)*1000.0;     // Bytes/second
    TRGrate = ((float)(t.tottrg - t.last_tottrg) / ElapsedTime)*1000.0;  // Triggers/second
    s.events+=(t.tottrg - t.last_tottrg);				 // Nb of Events
    Rate+=TPrate;
    Trgs+=TRGrate;

    if (display >= t.DisplayTime) {
      // calculate average count rate
      for (int i=9; i>0; i--) t.store10[i]=t.store10[i-1];		 
      t.store10[0]=t.tottrg - t.last_ave;
      av10=0;
      for (int i=0; i<10; i++) av10+=t.store10[i];
      av10=av10/(t.DisplayTime*10.);
      printf(KCYN);	
      printf("%d MB @ %.4fMB/s  DAQ = %.1f Hz  %ds Ave = %.1f Hz  Evts = %d\n", 
        	t.totnb/1048576, Rate/display/1048576, Trgs/display, t.DisplayTime*10, av10, t.tottrg);
      printf(RESET);
      control_scfile(t.DisplayTime*10,av10,0);
	freq= Trgs/display;
      display=0;
      t.last_ave=t.tottrg;
      Rate=0.;
      Trgs=0.;
    }	
				    		
    // check if events are recorded							
    if (FileEvtCounter==FileECOld) nodata++;
    else { 
      FileECOld=FileEvtCounter; 
      nodata=0; 
    }
    // exit if no counts for NO_DATA_TIME seconds
    if (nodata==NO_DATA_TIME) {
      errormsg(1,(char *)":::: ERROR: timeout; no trigger seen for too long time ::::");
      return -1;  
    }
                				
    t.last_totnb = t.totnb;
    t.last_tottrg = t.tottrg;
    t.PreviousTime = t.CurrentTime;
  }
  return 0;
}

//-------------------------------------------------------------------
// writes data to file
// returns the number of new triggers on success
// returns -1 if failure
int control_writedata(	digitizer& adc, int i, FILE *ofile, 
			int WriteToFile, int blt_bytes,	u_int32_t *buff)	
{
  int reduce;
  int retval=0;
  int32_t pnt;

  if (WriteToFile==1 && buff[0] != FILLER && blt_bytes>0) { // write binary data	  
	  
    // check if last read word is not valid
    if (buff[blt_bytes/4-1]==FILLER) reduce=1;
	                        else reduce=0;	    

    // Counters for Event/Trigger
    if ((buff[0]>>20)==0xA00 && (buff[1]>>28)==0x0 && i==0 && blt_bytes>0) {
        retval++;
    }
	    
    // code board identifier in ChannelMask Word
    buff[1]=buff[1]&0xF0FFFFFF;
    buff[1]=buff[1]+(adc.Id[i]*0x100000);	    
    
    fwrite(buff, sizeof(u_int32_t), blt_bytes/sizeof(u_int32_t)-reduce, ofile);
  }
  else if (WriteToFile==2) { // just write plain data stream (text) 
    pnt = 0;
    while ((pnt < (blt_bytes/4)) && (buff[pnt] != FILLER)) {

      if ((buff[pnt]>>20)==0xA00 && (buff[pnt+1]>>28)==0x0) { // TTT can also start with 0xA	
         if (i==0) { // count only triggers on one board
           retval++;
         }  
 	 if (WriteToFile==2) fprintf(ofile,"#%d#",adc.Id[i]);
      }		    

      // fprintf(ofile,"%5d\n%5d\n", (buff[pnt]>>16)&0xFFFF, buff[pnt]&0xFFFF);
      fprintf(ofile,"%08X\n",buff[pnt]);	      
      pnt++;
    } // end: Analyze-Data while-loop 
  }

 
    else { // store nothing but record triggers
    if ((buff[0]>>20)==0xA00 && (buff[1]>>28)==0x0 && i==0 && blt_bytes>0) retval++;
  } // end: if (WriteToFile==1)
	 
  return retval;
}	



//=============== Write to root option 7, DM version ================= 
	 //save only waveforms in root tree format
int control_writerootrawdata(digitizer& adc, int i, TFile *orootfile, TTree *t1, int WriteToFile, int blt_bytes,	u_int32_t *buff, int wf0[], int wf1[], int wf2[], int wf3[], int wf4[], int wf5[], int wf6[], int wf7[], double& freq)	
{
    int reduce;
    int retval=0;
  
  
    float wvf[8][adc.EventLength];
    for(int jj=0; jj<8;jj++) for(int ii=0; ii<adc.EventLength; ii++) wvf[jj][ii]=0;
 
 
 
        
     int pnt=0; 
     int CurrentChannel;
     int Size, cnt, wavecnt;
 // error handling if there is an invalid entry after an event
  if (buff[0]==0xFFFFFFFF) pnt++;
    
  // check header
  if ((buff[pnt]>>20)==0xA00 && (buff[pnt+1]>>28)==0x0) {
    Size=((buff[pnt]&0xFFFFFFF)-4);                   // size of full waveform (all channels)
    pnt++;
    int ChannelMask=buff[pnt] & 0xFF;	    	      pnt++;    
	if (i==0) { // count only triggers on one board
        retval++;
      }  
    
    // Get size of one waveform by dividing through the number of channels
    cnt=0;
    for (int j=0; j<8; j++) if ((ChannelMask>>j)&1) cnt++;
    Size=Size/cnt;
    
    pnt+=2; // ignore EventConter and TTT

    for (int j=0; j<1; j++) { // read all channels
      // read only the channels given in ChannelMask
      if ((ChannelMask>>j)&1) CurrentChannel=j;
                              else continue;

      if (CurrentChannel!=j) { pnt+=Size; continue; }
      			      else pnt++;
      if (j>j) return 0;	

			      
      cnt=0;                              // counter of waveform data
      wavecnt=0;                          // counter to reconstruct times within waveform
      while (cnt<Size)
      {

// save waveform in array
     	 		 wvf[j][wavecnt]=(double)((buff[pnt]&0xFFFF));
          		wvf[j][wavecnt+1]=(double)(((buff[pnt]>>16)&0xFFFF));
          		pnt++; wavecnt+=2; cnt++;
      } // end while(cnt...)
    } // end for-loop

      	for (int ii=0; ii<adc.EventLength; ii++) {
			if (adc.channel[0]) wf0[ii]=wvf[0][ii];
			if (adc.channel[1]) wf1[ii]=wvf[1][ii];
			if (adc.channel[2]) wf2[ii]=wvf[2][ii];
			if (adc.channel[3]) wf3[ii]=wvf[3][ii];
			if (adc.channel[4]) wf4[ii]=wvf[4][ii];
			if (adc.channel[5]) wf5[ii]=wvf[5][ii];
			if (adc.channel[6]) wf6[ii]=wvf[6][ii];
			if (adc.channel[7]) wf7[ii]=wvf[7][ii];
		}
		t1->Fill();
    } // end Check Header 
    else return -1; 
    
 
  	return retval;
} // end write root raw data



//-------------------------------------------------------------------
// generates a filename using path and the current dat/time
int control_generatefilename(char *OutFileName, char *path, int baseline)
{
  if(!baseline){
  	char timestr[100];
  	char cmnd[300];
  	if(jul==1){
  		control_gettimestring(timestr,'f');
		sprintf(cmnd,"mkdir %s%s",path,timestr);
		if (system(cmnd)!=0) errormsg(0,(char *)":::: WARNING: Could not create data directory ::::");
  		sprintf(OutFileName,"%s%s/%s_%s",path,timestr,FILE_ID,timestr);
	}
	else{
		std::cout << OutPutFolder << std::endl;
		sprintf(cmnd,"mkdir %s%s",path,OutPutFolder);
		std::cout << cmnd << std::endl;
		if (system(cmnd)!=0) errormsg(0,(char *)":::: WARNING: Could not create data directory ::::");
  		sprintf(OutFileName,"%s%s/%s_%s",path,OutPutFolder,FILE_ID,OutPutFolder);

	}
 

  	
 
  }
  else{

  }
  return 0;
}

//-------------------------------------------------------------------
// calculate and print time in "good" format
int control_calctime(timeandtrigger& t, int32_t ms, char *text)
{
  int days=0;
  int hours;
  int minutes;
  int seconds;  

  hours=(int)((t.MeasSeconds+ms/1000.)/3600.);
  minutes=(int)(((t.MeasSeconds+ms/1000.)-(hours*3600))/60.);  seconds=(int)(t.MeasSeconds+ms/1000.)-(hours*3600)-(minutes*60);
  days=(int)(hours/24);
  hours=hours-(days*24);
  
//   sprintf(text,"Total Measuring Time = %d.%03ld s = %.2f h",
//     t.MeasSeconds,ms,(t.MeasSeconds+ms/1000.)/3600.);
        printf(KCYN);	
  if (days>0)	
    sprintf(text,"Total Measuring Time = %d.%03d s = %02d days, %02d:%02d:%02d.%03d h",    
      t.MeasSeconds,ms,days,hours,minutes,seconds,ms);
  else
    sprintf(text,"Total Measuring Time = %d.%03d s = %02d:%02d:%02d.%03d h",    
      t.MeasSeconds,ms,hours,minutes,seconds,ms);
    printf(RESET);
  return 0;
}

//-------------------------------------------------------------------
// set variables to default values
int control_setdefault(	digitizer& adc, scaler& s, timeandtrigger& t, 
			crate& crt, multipurposemodule& mp, var_graphics& g)
{
  // adcs
  adc.NumberOfADCs=0;
  adc.BltSize=256;    
  adc.baseline=16000;
  adc.busy=1;
  adc.TriggerLatency=54;	// experimentally determined value

  // initialize PMT mapping
  for (int i=0; i<ADC_MAX; i++) {
    for (int j=0; j<8; j++) adc.pmt[i][j]=-1;
    adc.mask[i]=0x0;
  }
  adc.pmtmax=0;

  // crate
  crt.NumberOfCrates=0;

  // time and trigger
  t.DisplayTime=3;

  // general purpose module
  mp.HEVeto=1;     

  // scaler
  s.scalertime=60;		// read every x seconds
  s.totaltime=0.;		// for timing with ext. counter
  s.lifetime=0.; 		
  s.deadtime=0.;		
  s.totalcnt=29;		// default values for counter channels
  s.lifecnt=30;
  s.deadcnt=31;

  // graphics
  g.x_display=0;		// standard
  g.y_display=0;		// standard
  g.x_low=0;
  g.x_high=32767;
  g.y_low=0;
  g.y_high=16384;  
  g.x_autorange=300;		// range for automatic x-display

  return 0;
}


//-------------------------------------------------------------------
// extracts module and channel of a given PMT number
int control_getadc(digitizer& adc, int pmt, int& module, int& channel)
{

  for (module=0; module<ADC_MAX; module++)
    for (channel=0; channel<8; channel++) {
      if (adc.pmt[module][channel]==pmt) return 0;	
    }  
  
  return -1;
}



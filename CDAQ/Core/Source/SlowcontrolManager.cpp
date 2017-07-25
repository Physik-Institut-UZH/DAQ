#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <string>
#include "SlowcontrolManager.h"
#include "global.h"


SlowcontrolManager::SlowcontrolManager()
{
	 m_pmtNb,m_BoardInfo,m_baseline,m_errflag,m_graphics,m_lastevents,m_events,m_bytes,m_totalB,m_Nbmodule,m_triggertype=m_totalevents=m_type=m_numberChain=0;
	printf("\n");
	printf("%s*****************************************************************\n",KGRN);
	printf("%s                   DAQ  --  Data Aquistion Program 		    \n",KGRN); 
	printf("%s                            version: 4.0                          \n",KGRN);
	printf("%s*****************************************************************\n\n",KGRN);
	printf(RESET);
	sleep(1);
	gettimestring(m_OutputFolder); //Standart save format
	stringstream ss;
	string s;
	ss << GetUnixTime();
	ss >> s;
	
	/*Slowcontrol Folder*/
    	string command= "mkdir SlowControl";
    	system(command.c_str());
    	std::cout << std::endl;
    
    	/*Generate Current Rate File*/
    	string OutputFolder = m_OutputFolder;
	string status = "SlowControl/Rate_" + OutputFolder +  ".txt" ; 
	m_DAQStatus.open(status.c_str());
}

SlowcontrolManager::~SlowcontrolManager()
{
}

int SlowcontrolManager::Init(){

    ApplyXMLFile();
    return 0;
}


//-------------------------------------------------------------------
// Process command line options
int SlowcontrolManager::ProcessInput( int argc, char *argv[], char *envp[])
{
  extern char *optarg;               /* options processing */
  int start;                          /* dummy for options */
  int someArgs=0;

 
  //  search for possible options 
 while ( ( start = getopt( argc, argv, "x:g:f:biow" ) ) != -1 )  {
      someArgs=1;
      switch (start) {
        case 'f': 
          sprintf(m_OutputFolder,"%s",optarg);
          break;
        case 'x': 
          sprintf(m_XmlFileName,"%s",optarg);
          break;
	    case 'g':
          m_graphics = 1;
	      m_pmtNb=atoi(optarg);
          break;
        case 'i': 
          m_BoardInfo = 1;
          break;
        case 'b': 
          m_baseline = 1;		// baseline adjustment
          break;
        case 'w':   
	      printf("missed WIMPs protocol:");
		  int i;
	      char txt[100];
		  gettimestring(txt);
          i = (int)((float)rand()/2147483647.*10.);
          printf("  You missed %d WIMPs since stopping DAQ.\n  Better you start it again...\n",i);
	      exit(0);	
	      break;
        case 'o':
	     //control_scopehelp();
	      exit(0);
        default: 
          m_errflag++; 
          break;
        }
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
  
  return 0;
}


int SlowcontrolManager::StartAquistion(){
	
	printf(KYEL);
	printf("\nStart Data Aquisition\n\n");
	printf(RESET);
	gettimeofday(&m_begin, NULL);
    gettimeofday(&m_total, NULL);
	
	
	return 0;
}

int SlowcontrolManager::StopAquistion(){
	
	gettimeofday(&m_end, NULL);
	m_seconds  = m_end.tv_sec  - m_total.tv_sec;
	// print summary and update logfile
	printf(KGRN);
	printf("	\n");    
    	printf("	DAQ stopped\n"); 
    	printf("	Total Measuring time: %.2f s = %f h\n",m_seconds,m_seconds/(60*60));   
    	printf("	Total Number of Events Measured = %i\n",m_events);
   	printf("	\n\n");   
    	printf(RESET);
    	stringstream ss;
	string s;
	ss << GetUnixTime();
	ss >> s;
	
	/*Generate Summary File*/
	string OutputPath = m_OutputPath;
   	string OutputFolder = m_OutputFolder;
    
	string status =  OutputPath + "/"+ OutputFolder + "/Summary_" + OutputFolder +  ".txt" ; 
	m_DAQSummary.open(status.c_str());
	m_DAQSummary << "	DAQ stopped: " << GetUnixTime() <<  "\n" << "	Total Measuring time: "<< m_seconds << " s " << " = " << m_seconds/(60*60) << " h\n " << 	"	Total Number of Events Measured = " <<  m_events  << " \n";
	m_DAQSummary.flush();
	
	
	//Xurich specific
	//m_DAQSummary << "	Lifetime: " << m_seconds-(m_events*2.289/(1000)) ; 

	m_DAQStatus.close();
	m_DAQSummary.close();
	return 0;
}


int SlowcontrolManager::ShowStatus(int status ){
		if(status==1)
			m_events++;
			
		gettimeofday(&m_end, NULL);
		m_seconds  = m_end.tv_sec  - m_begin.tv_sec;
		m_useconds = m_end.tv_usec - m_begin.tv_usec;
		m_time = ((m_seconds) * 1000 + m_useconds/1000.0) + 0.5;
		
		 if(m_time>1000){
			/* print some progress indication */
			printf(KCYN);
			std::cout << "	" << (m_totalB/(1024*1024.))  << " GB "<<   (m_bytes/1048576.) << " MByte/s " << " DAQ-Rate: " << round(((m_events-m_lastevents)/m_time)*1000)<< " Hz " << " Total Events: " << m_events << std::endl;
			m_DAQStatus <<  GetUnixTime() << "      "<< round(((m_events-m_lastevents)/m_time)*1000) << "   "  <<  m_events << "    " <<  (m_totalB/(1024*1024.)) << "\n";
			m_lastevents=m_events;
			gettimeofday(&m_begin, NULL);
			printf(RESET);
			m_bytes=0;
		}
	return 0;
}


int SlowcontrolManager::ApplyXMLFile(){
	int temp;  
	char txt[100];
	const char *xstr;
	txt[0]='\0';

	printf(KYEL);
	printf("Reading User Configuration XML-File %s\n\n",m_XmlFileName);
	printf(RESET);

	printf(KGRN);
	// open the XML file -------------------------------------------------------
	XMLNode xMainNode=XMLNode::openFileHelper(m_XmlFileName,"settings");

	xstr=xMainNode.getAttribute("author");
	if (xstr) {
		strcpy(txt,xstr); 
		printf("	(Author: %s)\n",txt);	  
	} else error((char*)"XML-author"); 
	
	xstr=xMainNode.getAttribute("date");		
 	if (xstr) {
		strcpy(txt,xstr);
		printf("	(Date: %s)\n\n",txt);
	} else error((char*)"XML-date"); 	
	
	// parse global DAQ settings -----------------------------------------------
	XMLNode xNode=xMainNode.getChildNode("global");
	printf("	Global Settings:\n\n");
	
	xstr=xNode.getChildNode("user").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		printf("	User:        %s\n",txt); 
	} else error((char*)"XML-user");
	
	xstr=xNode.getChildNode("daq_version").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		printf("	DAQ Version: %s\n",txt); 
	} else error((char*)"XML-daq_version");
	

	xstr=xNode.getChildNode("source").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		printf("	Source:      %s\n",txt); 
	} else error((char*)"XML-source");

	xstr=xNode.getChildNode("notes").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		printf("	Comments:    %s\n",txt); 
	} else error((char*)"XML-notes");

	xstr=xNode.getChildNode("path").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		printf("	Path:        %s\n",txt);
		strcpy(m_OutputPath,txt);
	} else error((char*)"XML-path");

	xstr=xNode.getChildNode("nb_evts").getText();
	if (xstr) {
		strcpy(txt,xstr); 
    if (strstr(txt, "-1")!=NULL) printf("	NoF Events:  infinite\n");
       		            else printf("	NoF Events:  %s\n",txt);
		m_totalevents=atoi(txt);  
	} else error((char*)"XML-nb_evts");
	
	xstr=xNode.getChildNode("nb_evts_per_file").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		printf("	Events/File: %s\n",txt);
	} else error((char*)"XML-nb_evts_per_file");

	xstr=xNode.getChildNode("store_data").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		int WriteToFile=atoi(txt); 
		if (WriteToFile<0 || WriteToFile>7) WriteToFile=0;	
		printf("	File Format: ");
		switch (WriteToFile) {
		case 0: printf("Data NOT stored\n"); break;
		case 1: printf("ROOT format\n"); break;
		case 2: printf("ASCII format\n"); break;
		case 3: printf("Binary format\n"); break;
    }	
	} else error((char*)"XML-store_data");

	xstr=xNode.getChildNode("displaytime").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		printf("	DisplayTime: %s s\n\n",txt);
		//t.DisplayTime=atoi(txt);  
	} else error((char*)"XML-displaytime");
	
	 // parse global ADC settings -----------------------------------------------
	xNode=xMainNode.getChildNode("adc").getChildNode("global");
	printf("	ADC: Global settings:\n\n");
	
	xstr=xNode.getChildNode("ADCType").getText();
	if (xstr) {
		strcpy(txt,xstr);
		m_type=atoi(txt);
		if(m_type==0)
				printf("	ADC: v1720  \n");  
		else if(m_type==1)
				printf("	ADC: v1724    \n"); 
		else if(m_type==2)
				printf("	ADC: v1730    \n"); 
	} else error((char*)"XML-ADCType");
	 
	
	xstr=xNode.getChildNode("nb_modules").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		m_Nbmodule=atoi(txt);
		printf("	Number Modules: %s\n",txt); 
	} else error((char*)"XML-nb_modules");
	
	m_address=new string[m_Nbmodule];
	
	for(int i=0;i<m_Nbmodule;i++){
		char modules[300];
		sprintf(modules,"address_%i",i);
		xstr=xNode.getChildNode(modules).getText();
		if (xstr) {
			strcpy(txt,xstr);
			stringstream ss;
			ss << txt;
			ss >> m_address[i];
			printf("	ADC %i Address: %s\n",i,txt);
		} else error((char*)modules);
	}
	
    xstr=xNode.getChildNode("link_in_chain").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		m_numberChain=atoi(txt);
		printf("    Link in Chain: %s\n",txt); 
	} else error((char*)"link_in_chain");


	//--- Active channels of Modules-------
	for(int j=0;j<m_Nbmodule;j++){
		printf("	Module: %i\n",j);
		for(int i=0;i<8;i++){
			char channel[300];
			sprintf(channel,"ch_%i",i+j*8);
			xstr=xNode.getChildNode(channel).getText();
			if (xstr) {
				strcpy(txt,xstr);
				temp=atoi(txt);
				if(temp!=0){
					printf("	Channel %i Active. Threshold: %i\n",i,temp);
				}
		   } else error((char*)channel);
	  }
	}


	xstr=xNode.getChildNode("memoryorganisation").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		int bufSize=atoi(txt);
		float usec;
	    u_int32_t data;
		switch (bufSize) {
			case 512: data=0x0; break;
			case 256: data=0x1; break;
			case 128: data=0x2; break;
			case  64: data=0x3; break;
			case  32: data=0x4; break;
			case  16: data=0x5; break;
			case   8: data=0x6; break;
			case   4: data=0x7; break;
			case   2: data=0x8; break;
			case   1: data=0x9; break;
			case   0: data=0xA; break;  // 0.5 k
			default:  data=0x4; 
					  bufSize=32;
					  break; 
		}
		printf("	Memory Organisation: %sk Samples/Evt\n",txt);
	} else error((char*)"XML-memoryorganisation");
  

	xstr=xNode.getChildNode("custom_size").getText();
	if (xstr) {
        strcpy(txt,xstr);
        temp=(int)(atoi(txt)/10.);       // 10 samples per memory location
        printf("	Custom Size: %d samples\n",temp*10);
	} else error((char*)"XML-custom_size");
  
  

	xstr=xNode.getChildNode("posttrigger").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		printf("	Posttrigger: %s Samples\n",txt);
		temp=atoi(txt); 
	} else error((char*)"XML-posttrigger");
		
	xstr=xNode.getChildNode("baseline").getText();
	if (xstr) {
		strcpy(txt,xstr);
		printf("	Baseline:    %s ADC-Counts\n",txt);
		
	} else error((char*)"XML-baseline");
	
	xstr=xNode.getChildNode("baselineiteration").getText();
	if (xstr) {
		strcpy(txt,xstr);
		printf("	Baseline Iterations:	%s \n",txt);
		
	} else error((char*)"XML-baselineiteration");
	
	
	xstr=xNode.getChildNode("sampling_freq").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		printf("	Frequency:   %s Hz\n",txt); 
	} else error((char*)"XML-sampling_freq");
	
	xstr=xNode.getChildNode("voltage_range").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		printf("	Voltage Range: %s V\n",txt); 
	} else error((char*)"voltage_range");
	
	xstr=xNode.getChildNode("Sample_size_ADC").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		printf("	Sample Size ADC: %s bit\n",txt); 
	} else error((char*)"Sample_size_ADC");
	
	xstr=xNode.getChildNode("Sample_size_DAC").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		printf("	Sample Size DAC: %s bit\n\n",txt); 
	} else error((char*)"Sample_size_DAC");
  
	// ADC: parse ADC Trigger Settings
	xNode=xMainNode.getChildNode("adc").getChildNode("triggerSettings");
	printf("	ADC: Trigger Settings:\n\n");
	
	xstr=xNode.getChildNode("trigger").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		temp=atoi(txt);
    if (temp==0){ printf("	Trigger: External\n");
			xstr=xNode.getChildNode("TTL").getText();
			if(xstr) {
				strcpy(txt,xstr);
				temp=(int)(atoi(txt));
				if(temp==1)
					printf("	Trigger Logic: TTL\n\n");
				else
					printf("	Trigger Logic: NIM\n\n");
			} else error((char*)"XML-TTL");
	}
  	else if (temp==1){ 
		printf("	Trigger: Software\n");
		xstr=xNode.getChildNode("SoftwareRate").getText();;
		if (xstr) {
				strcpy(txt,xstr); 
				printf("	Software Rate: %s Hz\n\n",txt); 
		}
		else error((char*)"XML-SoftwareRate");
	}
    else if (temp==2) printf("	Trigger: Channel Treshold\n\n");
    else {printf("	Trigger: Channel Coincidence\n\n");}
  } else error((char*)"XML-trigger");

	
	// ADC: parse waveform display options
	xNode=xMainNode.getChildNode("graphics");
	printf("	Graphics Settings:\n\n");
	xstr=xNode.getChildNode("ydisplay").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		printf("	y-display: %s\n",txt);
	} else error((char*)"XML-ydisplay");

	xstr=xNode.getChildNode("yaxis_low").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		printf("	y-axis-low: %s\n",txt);
	} else error((char*)"XML-yaxis_low");
  
	xstr=xNode.getChildNode("yaxis_high").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		printf("	y-axis-high %s\n",txt);  
	} else error((char*)"XML-yaxis_high");

  
	printf(RESET);
	
    
	return 0;
}
  
//-------------------------------------------------------------------
// stores a copy of the XML file with the data
int SlowcontrolManager::xml_storefile()
{
  char cmnd[300];
  sprintf(cmnd,"cp %s %s.xml",m_XmlFileName,m_OutputPath);
  return 0;
}



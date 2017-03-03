#ifndef _SCOPEMANAGER_H_
#define _SCOPEMANAGER_H_


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <memory>
#include <typeinfo>
#include "xmlParser.h"
#include "common.h"

//ROOT-Stuff
#include <TH1D.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TFile.h>
#include <TLatex.h>
#include "TImage.h"


using namespace std;

/*
Class to manage the GUI of the ADC
*/


class ScopeManager: public Common
{
	public:
	   ScopeManager();
	   virtual ~ScopeManager();
	   
		//Function 
		int Init();
		
		//Set Function	
		void SetBuffer(u_int32_t *adcBuffer){buffer=adcBuffer;} 
		void SetEventLength(int length){m_length=length;}
		void SetXMLFile(char* file){m_XmlFileName=file;}
		void SetChannelTresh(int* channel){m_tresh=channel;}
		void SetModuleNumber(int module){m_nbmodule=module;}
		
		//GetFunction
		int GetModule(){return m_module;}

		//Visuliaze Event
		int ShowEvent();

		//Check keys for the window
		int graph_checkkey(char c);
		
		//Set Channel number
		void SetChannelNumber(int channeL){if(channeL<0)m_channel=0;else if(channeL>7){m_channel=channeL%8; m_module=channeL/8;} else m_channel=channeL;}


	 private:
		//Change scale of the graph
		int graph_edit(TH1D* g);
		int ApplyXMLFile();					//Read XML-Config for the Storage Manager

		TApplication *theApp;				//C++ binding for root
		TCanvas *win;						//Canvas of all
		TCanvas *single;					//Cavnas of one
		std::vector<TH1D*> g; 				//Histogramm for Waveform

		u_int32_t* buffer;					//Buffer of the data to visualize
		int m_length;						//Set length of the waveform
		int blt_bytes,pnt,Size,cnt,wavecnt, CurrentChannel;     //readout the channel
		int m_min,m_max;					//max min of event
		int m_mode;							//mode of y-axis
		char* m_XmlFileName;				//XML-File
		int m_channel;						//Channel to visuliaze
		int* m_tresh;
		int m_triggertype;					//Triggertype
		int m_module;						//Module
		int m_nbmodule;
		double m_mean;
	

};

#endif


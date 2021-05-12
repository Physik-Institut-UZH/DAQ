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
#include "CAENDigitizer.h"

//ROOT-Stuff
#include <TH1D.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TFile.h>
#include <TLatex.h>
#include "TImage.h"
#include "TDatime.h"


using namespace std;

/*
Author: Julien Wulf UZH
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
		void SetChannelThresh(int* channel){m_thresh=channel;}
    void SetModuleNumber(int module){m_nbmodule=module;}

    //Added by Neil
    void SetEnableMask(uint16_t mask){m_EnableMask = mask;}
    void Set16BitEvent(CAEN_DGTZ_UINT16_EVENT_t *evnt) {Event16 = evnt;}
    void SetEventVector(std::vector<CAEN_DGTZ_UINT16_EVENT_t> *vec){EventVector = vec;}
    void SetBufferSize(u_int32_t buff){m_BufferSize = buff;}

		
		//GetFunction
		int GetModule(){return m_module;}

		//Visuliaze Event
		int ShowEvent();

    //Plot MCA
    void ShowMCA(int count);
    //Write
    void WriteMCA(int nEvents);

		//Check keys for the window
		int graph_checkkey(char c);
		
		//Set Channel number
		void SetChannelNumber(int channeL){if(channeL<0)m_channel=0;else if(channeL>7){m_channel=channeL%8; m_module=channeL/8;} else m_channel=channeL;}
		//void SetChannelNumber(int channeL);//{if(channeL<0)m_channel=0;else if(channeL>7){m_channel=channeL%8; m_module=channeL/8;} else m_channel=channeL;}

		//Set threshold active 
		void SetThreshold(){m_triggertype=2;}


	 private:
		//Change scale of the graph
		int graph_edit(TH1D* g);
		int ApplyXMLFile();					//Read XML-Config for the Storage Manager

		TApplication *theApp;				//C++ binding for root
		TCanvas *win;						//Canvas of all
		TCanvas *single;					//Cavnas of one
		std::vector<TH1D*> g; 				//Histogramm for Waveform
    std::vector<TH1D*> gMCA;
    std::vector<std::vector<double>> vecMCA;
    std::vector<double> maxMCA, minMCA;

		u_int32_t* buffer;					//Buffer of the data to visualize
		int m_length;						//Set length of the waveform
		int blt_bytes,pnt,Size,cnt,wavecnt, CurrentChannel;     //readout the channel
		int m_min,m_max;					//max min of event
		int m_mode;							//mode of y-axis
		char* m_XmlFileName;				//XML-File
		int m_channel;						//Channel to visuliaze
		int* m_thresh;
		int m_triggertype;					//Triggertype
		int m_posttrigger;					//postTrigger
		int m_module;						//Module
		int m_nbmodule;
		double m_mean;
		int m_save; 
		int m_counter;
		int m_zleEnable;
        int m_dataReduction;    
		int m_Baseline;
    int m_nbCh;

    //added by Neil
    uint32_t m_BufferSize;
    uint16_t m_EnableMask;
    CAEN_DGTZ_UINT16_EVENT_t * Event16;
    std::vector<CAEN_DGTZ_UINT16_EVENT_t> * EventVector;
    //bool m_useMCA;// = false;
    int m_useMCA;
    int m_MCAHighBin;
    int m_MCALowBin;
    int m_NbinsMCA;
    int m_logSwitch;
    string m_path;
};

#endif


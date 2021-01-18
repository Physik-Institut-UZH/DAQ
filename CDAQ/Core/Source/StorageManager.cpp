#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <string>
#include "StorageManager.h"
#include "global.h"
#include "common.h"
#include <bitset>

//ROOT Libraries
#include "TROOT.h"
#include <TApplication.h>
#include "TSystem.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TRint.h"
#include "TApplication.h"

/*
Author: Julien Wulf UZH
*/

StorageManager::StorageManager()
{
	m_NoE,m_EventsPerFile,m_WriteToFile,m_time,m_nbCh,m_filenumber=m_module=m_ZLE=0;
	m_path="test/";
	m_moduleName="";
}

StorageManager::~StorageManager()
{
	output->cd();
	tree->Write();
	output->Close();
}

int StorageManager::Init(){

  ApplyXMLFile();

  printf(KYEL);
  printf("Creating Storage path\n\n");
  printf(RESET);
  printf(KGRN);
  std::cout << " 	Storage Path: " << m_path +  m_OutputFolder +"/"+ m_moduleName.c_str()   << std::endl ;
  printf(RESET);

  /*Create Output Folder and ROOT Folder*/
  m_command= "mkdir -p " + m_path + m_OutputFolder;
  system(m_command.c_str());
  std::cout << std::endl;

  /*Create Module Folder*/
  m_command= "mkdir -p " + m_path + m_OutputFolder +"/" + m_moduleName.c_str();
  system(m_command.c_str());
  std::cout << std::endl;

  if(m_WriteToFile==1 && m_ZLE==0){
    //Copy XML file to the output directory
    stringstream cmd;
    cmd.str("");
    cmd << "cp " << m_XmlFileName << " " << m_path + m_OutputFolder << "/" << m_OutputFolder << ".xml";
    system(cmd.str().c_str());

    InitROOT();
  }
  if(m_WriteToFile==1 && m_ZLE==1) {
    //Copy XML file to the output directory
    stringstream cmd;
    cmd.str("");
    cmd << "cp " << m_XmlFileName << " " << m_path + m_OutputFolder << "/" << m_OutputFolder << ".xml";
    system(cmd.str().c_str());

    InitROOTZLE();
  }

}



int StorageManager::InitROOT(){

  stringstream ss;
  string data;
  ss << m_filenumber;
  ss >> data;
  stringstream tt;
  tt << m_module;
  string t;
  tt >> t;
  m_command = m_path + m_OutputFolder + "/" + m_moduleName.c_str() + m_OutputFolder + "_" + "Module_" + t + "_" + data + ".root";
  m_filenumber++;
  std::cout << std::endl << m_command << std::endl << std::endl;
  output = new TFile(m_command.c_str(), "RECREATE");
  tree = new TTree("t1","");
  Int_t wfCounter = 0;
  //Create ROOT Branches	
  for(int i = 0;i < m_nbCh; i++){
    if(channelActive[i]){
      //wfVec will be the size of the total active channels, 
      //one thing to be aware of is if channel 0,1, and 3 are active,
      //the indices will not match if relooping thus another wvCounter is used in further code
      m_BufferSize = Event16->ChSize[i];
      wfVec.push_back( new int [m_BufferSize]); 
      (tree)->Branch(TString::Format("wf%i",i), wfVec[wfCounter], TString::Format("wf%i[%i]/I",i,m_BufferSize));
      wfCounter++;
    }
  }
  (tree)->Branch("Time",&m_time,"Time/D");

  TNamed * namedSampleFreq = new TNamed("SampleFreq",(to_string(m_sampleFreq)) );
  namedSampleFreq->Write();

  TNamed * namedEventSize = new TNamed("EventSize",(to_string(m_BufferSize)) );
  namedEventSize->Write();

  TNamed * namedVrange = new TNamed("EventRange",(to_string(m_vRange)) );
  namedVrange->Write();

  TNamed * namedNChannels = new TNamed("nChannelsBitMask",(to_string(m_EnableMask)));
  namedNChannels->Write();

  TNamed * namedResolution = new TNamed("adcResolution",(to_string(m_resDAC)));
  namedResolution->Write();

  return 0;

}

//TODO figure out how ZLE works...
int StorageManager::InitROOTZLE(){
  stringstream ss;
  string data;
  ss << m_filenumber;
  ss >> data;
  stringstream tt;
  tt << m_module;
  string t;
  tt >> t;
  m_command = m_path + m_OutputFolder + "/" + m_moduleName.c_str() + m_OutputFolder + "_" + "Module_" + t + "_" + data + ".root";
  m_filenumber++;
  std::cout << std::endl << m_command << std::endl << std::endl;
  output = new TFile(m_command.c_str(), "RECREATE");
  tree = new TTree("t1","");
  //Create ROOT Branches
  m_zle_wfVec.resize(m_nbCh);
  m_zle_cwVec.resize(m_nbCh);
  for(int i = 0;i < m_nbCh; i++){
    if(channelActive[i]){
      tree->Branch(TString::Format("wf%i_cw",i), "vector<int>", &m_zle_cwVec[i]);
      tree->Branch(TString::Format("wf%i",i), &m_zle_wfVec[i]);
    }
  }

  return 0;
}

int StorageManager::NewFile(){

  if(m_WriteToFile==1 && m_ZLE==0){
    this->SaveROOTContainer();
    stringstream ss;
    string data;
    ss << m_filenumber;
    ss >> data;
    stringstream tt;
    tt << m_module;
    string t;
    tt >> t;
    m_command = m_path + m_OutputFolder + "/" +  m_moduleName.c_str()  + m_OutputFolder + "_" + "Module_" + t + "_" + data + ".root";
    m_filenumber++;
    std::cout  << std::endl << m_command << std::endl << std::endl;
    output = new TFile(m_command.c_str(), "RECREATE");
    tree = new TTree("t1","");
    
    // Need a waveform counter becuase some channels will be turned off and size of wfVec might not match up with for loop indices
    Int_t wfCounter = 0; 
    for(int i = 0;i < m_nbCh; i++){
      //TODO is was intially set to channelActive[0], is this a mistake?
      if(channelActive[i]){
        (tree)->Branch(TString::Format("wf%i",i), wfVec[wfCounter], TString::Format("wf%i[%i]/I",i, m_BufferSize));
        wfCounter++;
      }
    }
    (tree)->Branch("Time",&m_time,"Time/D");
    
    TNamed * namedSampleFreq = new TNamed("SampleFreq",(to_string(m_sampleFreq)) );
    namedSampleFreq->Write();
    
    TNamed * namedEventSize = new TNamed("EventSize",(to_string(m_BufferSize)) );
    namedEventSize->Write();
    
    TNamed * namedVrange = new TNamed("EventRange",(to_string(m_vRange)) );
    namedVrange->Write();

    TNamed * namedNChannels = new TNamed("nChannelsBitMask",(to_string(m_EnableMask)));
    namedNChannels->Write();

    TNamed * namedResolution = new TNamed("adcResolution",(to_string(m_resDAC)));
    namedResolution->Write();
    
  }
  return 0;
}


int StorageManager::FillContainer(){

  if(m_WriteToFile==1 && m_ZLE==0){
    FillROOTContainer();
  }

  if(m_ZLE==1 && m_WriteToFile==1){
		FillZLEROOTContainer();
	}

}

bool StorageManager::FillROOTContainer(){
  /*
  Int_t wvCounter = 0;
  for(int i = 0; i < m_nbCh; i++){
    if(!channelActive[i]) continue;
    //Simple Live Processing (Charge,Height,Position,Baseline,RMS) can be easly implemented here (alread looping over waveform) TODO
    for(int j = 0; j < Event16->ChSize[i]; j++){
      if(channelActive[i]){
        wfVec[wvCounter][j] = Event16->DataChannel[i][j];
        int data = Event16->DataChannel[i][j];
        //cout<<"\t Reading data from channel "<<i<<", bin "<<j<<", data "<<data<<endl;
      }
    }
    wvCounter++;
  }
  */
  for(int i = 0; i < EventVector->size(); i++){
    //cout<<"First for loop over i (EventVector size) "<<i<<endl;
    Int_t wvCounter = 0;
    for(int j = 0; j < m_nbCh; j++){
    //cout<<"Second for loop over j (N channels) "<<j<<endl;
      if(!channelActive[j]) continue;
      //cout<<" Only loop over active channels "<<j;
      CAEN_DGTZ_UINT16_EVENT_t event = EventVector->at(j);
      //cout<<"...Successfully accessed EventVector"<<endl;
      //Simple Live Processing (Charge,Height,Position,Baseline,RMS) can be easly implemented here (alread looping over waveform) TODO
      for(int k = 0; k < event.ChSize[j]; k++){
        //if(k == 0) cout<<"\tLooping over event"<<endl;
        if(channelActive[j]){
          int data = event.DataChannel[j][k];
          //if(k < 10) cout<<"\t Reading data from channel "<<j<<", bin "<<k<<", data "<<data<<", waveCounter "<<wvCounter<<endl;
          wfVec[wvCounter][k] = event.DataChannel[j][k];
        }
      }
      //cout<<"...Finished reading event from CH-"<<wvCounter<<"..."<<endl;
      wvCounter++;
    }
    m_time=GetUnixTime();
    tree->Fill();
    //cout<<"...Looped over all channels, looking at next Trigger"<<endl;
  }
}

int StorageManager::SaveContainer(){
  if(m_WriteToFile==1){
    SaveROOTContainer();
  }
}

void StorageManager::SaveROOTContainer(){
  output->cd();
  tree->Write();
  output->Close();
}

int StorageManager::ApplyXMLFile(){
  int temp;  
  char txt[100];
  const char *xstr;
  txt[0]='\0';

  // open the XML file -------------------------------------------------------
	XMLNode xMainNode=XMLNode::openFileHelper(m_XmlFileName,"settings");
	
  // parse global DAQ settings -----------------------------------------------
  XMLNode xNode=xMainNode.getChildNode("global");

  xstr=xNode.getChildNode("path").getText();
  if (xstr) {
    strcpy(txt,xstr); 
		stringstream ss;
    ss << txt;
    ss >> m_path;
  } else error((char*)"XML-path");

  xstr=xNode.getChildNode("nb_evts").getText();
  if (xstr) {
		strcpy(txt,xstr); 
		m_NoE=atoi(txt); 
	} else error((char*)"XML-nb_evts");
	
	xstr=xNode.getChildNode("nb_evts_per_file").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		m_EventsPerFile=atoi(txt);
	} else error((char*)"XML-nb_evts_per_file");

	xstr=xNode.getChildNode("store_data").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		m_WriteToFile=atoi(txt); 
	} else error((char*)"XML-store_data");

  // parse global ADC settings -----------------------------------------------
	xNode=xMainNode.getChildNode("adc").getChildNode("global");
  
	xstr=xNode.getChildNode("custom_size").getText();
  if (xstr) {
    strcpy(txt,xstr);
    m_custom_size=atoi(txt);
  } else error((char*)"XML-custom_size");

  xstr=xNode.getChildNode("nb_chs").getText();
  if (xstr) {
    strcpy(txt,xstr); 
    m_nbCh=atoi(txt); 
  } else error((char*)"XML-nb_chs");

  //--- Active channels -------
  channelActive = new int[m_nbCh];

  for(int i=0;i<m_nbCh;i++){
    char channel[300];
    sprintf(channel,"ch_%i",i+m_module*m_nbCh);
    xstr=xNode.getChildNode(channel).getText();
    if (xstr) {
      strcpy(txt,xstr);
      temp = atoi(txt);
      if(temp!=0)
        channelActive[i]=1;
      else
        channelActive[i]=0;
    } else error((char*)channel);
  }
  xNode=xMainNode.getChildNode("adc").getChildNode("ZLE");
  xstr=xNode.getChildNode("ZLEActivated").getText();
  if (xstr) {
    strcpy(txt,xstr);
    temp=((int)atoi(txt));
    m_ZLE=temp;
  }
  else error((char*)"ZLE");

  return 0;
}




#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <string>
#include "ScopeManager.h"
#include "global.h"
#include "TLine.h"
#include "TH1D.h"
#include "TVirtualFFT.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TMath.h"
/*
Author: Julien Wulf UZH
*/


ScopeManager::ScopeManager()
{
	m_mode=m_channel=m_triggertype=m_module=m_nbmodule=m_mean=m_save=m_counter=m_ZLE=m_Baseline=m_nbCh=0;
    m_useMCA=m_logSwitch=0;
  g.resize(0);
}

ScopeManager::~ScopeManager()
{
}

int ScopeManager::Init(){
	//m_BufferSize=m_BufferSize+10;
  ApplyXMLFile();
  //win = new TCanvas("win","JDAQ -- DAQ for Zuerich (multi)",1700,768);

  /*Slowcontrol Folder*/
  string command= "mkdir Plots";
  system(command.c_str());
  std::cout << std::endl;


  //single = new TCanvas("single","CDAQ -- DAQ for Zuerich (single)",800,800);
  single = new TCanvas("single","CDAQ -- DAQ for Zuerich (single)",600,800); //Optimized for a 4/3 height width ratio picture
  //single = new TCanvas("single","CDAQ -- DAQ for Zuerich (single)",300,400); //Optimized for a 4/3 height width ratio picture
  gStyle->SetOptStat(0000000);
  gStyle->SetOptFit(1100);
  gStyle->SetTitleFillColor(0);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetStatColor(0);
  //win->SetFillColor(0);
  //win->SetBorderMode(0);

  single->SetFillColor(0);
  single->SetBorderMode(0);
  //	single->Divide(1,2);
  //Init Graphs
  g.resize(m_nbCh);
  gMCA.resize(m_nbCh);
  vecMCA.resize(m_nbCh);
  maxMCA.resize(m_nbCh);
  for(Int_t j=0; j<m_nbCh;j++){
    if(m_EnableMask & (1<<j)){
      g[j] = new TH1D(Form("Channel:  %i",j),Form("Channel:  %i",j),Event16->ChSize[j]-1,0,Event16->ChSize[j]-1);
      m_BufferSize = Event16->ChSize[j];
    }
    else 
      g[j] = new TH1D(Form("Channel:  %i",j),Form("Channel:  %i",j),1,0,1);
  }


  for(Int_t j=0; j<m_nbCh;j++){
    g[j]->SetLineColor(2);
    g[j]->GetXaxis()->SetTitle("Samples");
    g[j]->GetYaxis()->SetTitle("ADC-Counts");
    g[j]->GetXaxis()->SetTitleFont(42);
    g[j]->GetXaxis()->SetLabelFont(42);
    g[j]->GetXaxis()->SetTitleOffset(0.9);
    g[j]->GetYaxis()->SetTitleFont(42);
    g[j]->GetYaxis()->SetLabelFont(42);
    g[j]->GetYaxis()->SetTitleOffset(1);
    g[j]->SetLineWidth(2);
    
  }

  // Startup Window
  single->cd();

  TImage *img = TImage::Open("../Macro/Logo/splash.png");

  if (!img) {
    printf("Could not create an image... exit\n");
    return - 1;
  }

  img->SetConstRatio(0);
  img->SetImageQuality(TAttImage::kImgBest);
  img->Draw("");
  img->SetEditable(kTRUE);

  //win->Modified();
  //win->cd();
  //win->SetSelected(win);  
  //win->Update();
  //win->cd();
  //win->Divide(4,2,0,0);

  single->Modified();
  single->Update();
 
  sleep(4);


  return 0;
}


/*
//Updated by Neil to make a much more simple code (Hopefully)
int ScopeManager::ShowEvent(){

  for(int i = 0; i < m_nbCh; i++){
    for(int j = 0; j < Event16->ChSize[i]; j++){
      double binContent = Event16->DataChannel[i][j];
      g[i]->SetBinContent(j, binContent);
    } 
  }

  for(int i=0;i<m_nbCh;i++){
    graph_edit(g[i]);
  }

  TLine threshhigh = TLine(0, m_thresh[m_channel],m_BufferSize-1, m_thresh[m_channel]);
  threshhigh.SetLineWidth(4);
	threshhigh.SetLineStyle(3);
  threshhigh.SetLineColor(kOrange);
	
  //Event
  graph_edit(g[m_channel]);
  g[m_channel]->Draw();
  if(m_triggertype==2)
    g[m_channel]->SetTitle(Form("Channel:  %i , Threshold: %i",m_channel,m_thresh[m_channel]));
  else
    g[m_channel]->SetTitle(Form("Channel:  %i , Module: %i",m_channel,m_module));
  if(m_triggertype==2)
    threshhigh.Draw("same");
  single->Modified();
  single->SetSelected(single);
  single->Update();

  if(m_save==1){
    single->SaveAs(Form("Plots/Event_%i.png",m_counter));
  }
  // single->cd(2);
  //hm->Draw();
  m_counter++;
  return 0;
}
*/

//Updated by Neil to make a much more simple code (Hopefully)
int ScopeManager::ShowEvent(){
  //cout<<"ScopeManager::"<<EventVector->size()<<endl;

  //for(int h = 0; h < EventVector->size(); h++){
    CAEN_DGTZ_UINT16_EVENT_t *event = Event16;//EventVector->at(h);
    for(int i = 0; i < m_nbCh; i++){
      for(int j = 0; j < event->ChSize[i]; j++){
        double binContent = event->DataChannel[i][j];
//        if(j < 10) cout<<"\t Reading data from channel "<<i<<", Trigger num "<<h<<", bin "<<j<<", data "<<binContent<<endl;
        g[i]->SetBinContent(j, binContent);
      } 
    }

    for(int i=0;i<m_nbCh;i++){
      graph_edit(g[i]);
    }

    TLine threshhigh = TLine(0, m_thresh[m_channel],m_BufferSize-1, m_thresh[m_channel]);
    threshhigh.SetLineWidth(4);
    threshhigh.SetLineStyle(3);
    threshhigh.SetLineColor(kOrange);

    //Event
    graph_edit(g[m_channel]);
    g[m_channel]->Draw();
    if(m_triggertype==2)
      g[m_channel]->SetTitle(Form("Channel:  %i , Threshold: %i",m_channel,  m_thresh[m_channel]));
    else
      g[m_channel]->SetTitle(Form("Channel:  %i , Module: %i",m_channel,m_module));
    if(m_triggertype==2)
      threshhigh.Draw("same");
    single->Modified();
    single->SetSelected(single);
    single->Update();

    if(m_save==1){
      single->SaveAs(Form("Plots/Event_%i.png",m_counter));
    }
    // single->cd(2);
    //hm->Draw();
    m_counter++;
  //}
  return 0;
}


void ScopeManager::ShowMCA(int counter){
  

  for(int i = 0; i < m_nbCh; i++){
    double binSum = 0;
    double baseline = 0;
    for(int j = 1; j < 11; j++){
      if(Event16->ChSize[i] == 0) continue;
      baseline += Event16->DataChannel[i][j];
    }
    baseline /= 10;

    for(int j = 0; j < Event16->ChSize[i]; j++){
      double binContent = fabs(Event16->DataChannel[i][j] - baseline);
      binSum += binContent;
    } 
    vecMCA[i].push_back(binSum);
    if(binSum > maxMCA[i]) maxMCA[i] = binSum;
   
    if(counter-1 == 0)  gMCA[i] = new TH1D(Form("MCA-Channel:  %i",i),Form("MCA-Channel:  %i",i),1000,1,7e3);//100 bins over full range//seems exseive
    /*
    if(( (int)maxMCA[i]*1.0 ) > 0){
      delete gMCA[i];
      gMCA[i] = new TH1D(Form("MCA-Channel:  %i",i),Form("MCA-Channel:  %i",i),5000,1,100*16384);//100 bins over full range//seems exseive
    }
    else{
      delete gMCA[i];
      gMCA[i] = new TH1D(Form("MCA-Channel:  %i",i),Form("MCA-Channel:  %i",i),1,0,1);
    }
    */
  }
  if(counter%20 != 0) return;//don't update the MCA for every event, every 20th event seems modest

  for(int i = 0; i < vecMCA.size(); i++){
    for(int j = 0; j < vecMCA[i].size(); j++){
      gMCA[i]->Fill(vecMCA[i][j]);
    }

    //clear vector
    vecMCA[i].resize(0);
  }
  for(int i = 0; i < m_nbCh; i++) graph_edit(gMCA[i]);
  graph_edit(gMCA[m_channel]);
  gMCA[m_channel]->Draw();

  single->Modified();
  single->SetSelected(single);
  single->Update();

  if(m_save==1){
    single->SaveAs(Form("Plots/Event_%i.png",m_counter));
  }
  // single->cd(2);
  //hm->Draw();
  m_counter++;

}
void ScopeManager::WriteMCA(int nEvents){
  TDatime * datTime = new TDatime();
  TString fileName = "MCAPlot_"+to_string(nEvents)+"_"+to_string(datTime->GetDate()) + "-" + to_string(datTime->GetTime());

  cout<<"Writing MCA file "<<fileName<<".root to Plots/"<<endl;
  TFile* fOut = new TFile(m_path + fileName + TString(".root"),"RECREATE");
  fOut->cd();
  for(int i = 0; i < m_nbCh ; i++){
    gMCA[i]->Write();
    gMCA[i]->Clear();
  }
  fOut->Close();

}
//-------------------------------------------------------------------
// prepare graph for display
int ScopeManager::graph_edit( TH1D *g)
{  // y-axis
  switch (m_mode) {
    case 1:  // set manually (via xml-file) 
      g->GetYaxis()->SetRangeUser(m_min,m_max);
      break;
    case 2:  // set automatically
      m_max=g->GetMaximumBin();
      m_min=g->GetMinimumBin();
      m_max=g->GetBinContent(m_max)+50;
      m_min=g->GetBinContent(m_min)-10;
      if(m_useMCA){
        m_min = 1;
      }
      g->GetYaxis()->SetRangeUser(m_min,m_max);
      break;
    case 0:  // full mode
      g->GetYaxis()->SetRangeUser(0,16384);
      break;
    default: // do nothing; display all
      break;
  }

  return 0;
}


int ScopeManager::ApplyXMLFile(){
	int temp;  
  char txt[100];
  const char *xstr;
	txt[0]='\0';
	
	// open the XML file -------------------------------------------------------
	XMLNode xMainNode=XMLNode::openFileHelper(m_XmlFileName,"settings");

  XMLNode xNode=xMainNode.getChildNode("global");

  //Path to write MCA plot
  xstr=xNode.getChildNode("path").getText();
  if (xstr) {
    strcpy(txt,xstr); 
		stringstream ss;
    ss << txt;
    ss >> m_path;
  } else error((char*)"XML-path");


  // parse global ADC settings -----------------------------------------------
  xNode=xMainNode.getChildNode("adc").getChildNode("global");
  xstr=xNode.getChildNode("baseline").getText();
  if (xstr) {
    strcpy(txt,xstr);
    m_Baseline=(int)atoi(txt);

  } else error((char*)"XML-baseline");

  
  xNode=xMainNode.getChildNode("adc").getChildNode("global");
  xstr=xNode.getChildNode("nb_chs").getText();
  if (xstr) {
		strcpy(txt,xstr); 
		m_nbCh=(int)(atoi(txt));
	} else error((char*)"XML-nb_chs");
	
  xNode=xMainNode.getChildNode("adc").getChildNode("triggerSettings");
	xstr=xNode.getChildNode("trigger").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		m_triggertype=atoi(txt);
	} else error((char*)"XML-trigger");

  xNode=xMainNode.getChildNode("adc").getChildNode("ZLE");
  xstr=xNode.getChildNode("ZLEActivated").getText();
  if (xstr) {
    strcpy(txt,xstr);
    temp=((int)atoi(txt));
    m_ZLE=temp;
  }
  else error((char*)"ZLE");


	// ADC: parse waveform display options
	xNode=xMainNode.getChildNode("graphics");

  xstr=xNode.getChildNode("useMCA").getText();
  if(xstr){
    strcpy(txt,xstr);
    m_useMCA=(int)(atoi(txt));
    if(m_useMCA)    printf(" Using MCA plotting rather than waveform plotting \n");
  }
	xstr=xNode.getChildNode("ydisplay").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		m_mode=atoi(txt); 		
	} else error((char*)"XML-ydisplay");

	xstr=xNode.getChildNode("yaxis_low").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		m_min=atoi(txt); 
	} else error((char*)"XML-yaxis_low");
  
	xstr=xNode.getChildNode("yaxis_high").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		m_max=atoi(txt); 
	} else error((char*)"XML-yaxis_high");
		
	return 0;
}


//-------------------------------------------------------------------
// keys for Oscilloscope mode
int ScopeManager::graph_checkkey(char c){

	// increase channel
  if (c == '+' || c == '=') {
    // in channel display mode
    m_channel++;  
    if (m_channel>m_nbCh-1) {
      m_channel=0;       
      if(m_module<m_nbmodule-1)
        m_module++;
      else
        m_module=0;
    }
  }

 //Save Waveforms
 if (c == 'p' || c == 'P') {
	m_save = 1;
 }

  // decrease channel
 if (c == '-' || c == '_') {
   // in channel display mode
   m_channel--;  
   if (m_channel<0) {
     m_channel=m_nbCh-1;
     if(m_module>0)
       m_module--;
     else
       m_module=m_nbmodule-1;
   } 
 }

 // change display mode y-axis
 if (c == '1' || c == '2' || c == '3' || c == 'u' || c == 'd' || c == 'U' || c == 'D') {
    switch (c) {
      case '1': m_mode=0;  break;	
                break;
      case '2': m_mode=1;  break;	
                break;
      case '3': m_mode=2;  break;	
                break;
      case 'u': if (m_mode==1) m_min+=10;m_max-=10;	break;
      case 'U': if (m_mode==1) m_max+=10;	break;
      case 'd': if (m_mode==1) m_min-=10;m_max+=10;	break;
      case 'D': if (m_mode==1) m_max-=10;	break;
    }
 }

 if(c == 'l' || c == 'L'){
   if(m_logSwitch) single->Clear();
   single->SetLogy();
   m_logSwitch = true;
 }

	return 0;
}

/*
void ScopeManager::SetChannelNumber(int channeL)
{
  if(channeL<0)
    m_channel=0;
  else if(channeL>m_nbCh-1){
    m_channel=channeL%m_nbCh; 
    m_module=channeL/m_nbCh;
  } 
  else 
    m_channel=channeL;
}
*/



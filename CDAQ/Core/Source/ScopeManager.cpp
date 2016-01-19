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


ScopeManager::ScopeManager()
{
	m_mode=m_channel=m_triggertype=m_module=0;
}

ScopeManager::~ScopeManager()
{
}

int ScopeManager::Init(){
	//m_length=m_length+10;
	ApplyXMLFile();
	//win = new TCanvas("win","JDAQ -- DAQ for Zuerich (multi)",1024,1700);
	
	single = new TCanvas("single","JDAQ -- DAQ for Zuerich (single)",1700,768);
  	gStyle->SetOptStat(0000000);
  	gStyle->SetOptFit(1100);
  	gStyle->SetTitleFillColor(0);
  	gStyle->SetTitleBorderSize(0);
  	gStyle->SetStatColor(0);
  	
  	//win->SetFillColor(0);
  	//win->SetBorderMode(0);
	
	single->SetFillColor(0);
  	single->SetBorderMode(0);

	//Init Graphs
	 for(Int_t j=0; j<8;j++){
 		g.push_back(new TH1D(Form("Channel:  %i",j),Form("Channel:  %i",j),m_length-1,0,m_length-1));
     }

	for(Int_t j=0; j<8;j++){
	
		g[j]->SetLineColor(2);
   		g[j]->GetXaxis()->SetTitle("Samples");
   		g[j]->GetYaxis()->SetTitle("ADC-Counts");
   		g[j]->GetXaxis()->SetTitleFont(72);
       	g[j]->GetXaxis()->SetLabelFont(72);
        g[j]->GetXaxis()->SetTitleOffset(1.5);
       	g[j]->GetYaxis()->SetTitleFont(72);
	  	g[j]->GetYaxis()->SetLabelFont(72);
        g[j]->GetYaxis()->SetTitleOffset(1.5);
        g[j]->SetLineWidth(2);
	}

	// Startup Window
	single->cd();

	 TImage *img = TImage::Open("../Macro/splash.png");

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

int ScopeManager::ShowEvent(){

	
	//Start from the first word
    	pnt =0;

    	//Check first Word
    	if (buffer[0]==0xFFFFFFFF) pnt++;

    	// check header
    	if ((buffer[pnt]>>20)==0xA00 && (buffer[pnt+1]>>28)==0x0) {
       	 Size=((buffer[pnt]&0xFFFFFFF)-4);                   // size of full waveform (all channels)
       	 pnt++;
 
		//Read ChannelMask (Handbook)
        int ChannelMask=buffer[pnt] & 0xFF;                 

		pnt++;    
    
        // Get size of one waveform by dividing through the number of channels
        cnt=0;
        for (int j=0; j<8; j++) if ((ChannelMask>>j)&1) cnt++;
        Size=Size/cnt;

 	// ignore EventConter and TTT
        pnt+=2;
      	
        for (int j=0; j<8; j++) { // read all channels
	
		// read only the channels given in ChannelMask
		if ((ChannelMask>>j)&1) CurrentChannel=j;
                else{ continue;}
		
		if (CurrentChannel!=j) { pnt+=Size; continue; }
      		else pnt++;

		if (j>j) return 0;	
	      
		cnt=0;                              // counter of waveform data
		wavecnt=0;                          // counter to reconstruct times within waveform
      		while (cnt<(Size-(CORRECTION/2)))
      		{	g[j]->SetBinContent(wavecnt,(double)((buffer[pnt]&0xFFFF)));	
				g[j]->SetBinContent(wavecnt+1,(double)(((buffer[pnt]>>16)&0xFFFF)));	
          		pnt++; wavecnt+=2; cnt++;
      		} // end while(cnt...)
	}
    }
	for(int i=0;i<8;i++){
		//win->cd(1+i);
		graph_edit(g[i]);
   		//g[i]->Draw();
	}
    //win->Modified();
    //win->SetSelected(win);
    //win->Update();
    
     single->cd();
    
    //Treshhold level
    
	TLine treshhigh = TLine(0, m_tresh[m_channel],m_length-1, m_tresh[m_channel]);
	treshhigh.SetLineWidth(4);
	treshhigh.SetLineStyle(3);
	treshhigh.SetLineColor(kOrange);
	
	
	//Event
    graph_edit(g[m_channel]);
    g[m_channel]->Draw();
    if(m_triggertype==2)
		g[m_channel]->SetTitle(Form("Channel:  %i , Threshold: %i",m_channel,m_tresh[m_channel]));
	else
		g[m_channel]->SetTitle(Form("Channel:  %i , Module: %i",m_channel,m_module));
	if(m_triggertype==2)
		treshhigh.Draw("same");
    single->Modified();
    single->SetSelected(single);
    single->Update();

	return 0;

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
      m_min=(int)g->GetMinimum();
      m_max=(int)g->GetMaximum();
      m_min=m_min-5;
      m_max=m_max+5;
      g->GetYaxis()->SetRange(m_min,m_max);
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
	
	
	 // parse global ADC settings -----------------------------------------------
	XMLNode xNode=xMainNode.getChildNode("adc").getChildNode("triggerSettings");
	xstr=xNode.getChildNode("trigger").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		m_triggertype=atoi(txt);
	} else error((char*)"XML-trigger");
	
	// ADC: parse waveform display options
	xNode=xMainNode.getChildNode("graphics");

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
      if (m_channel>7) {
        m_channel=0;       
        if(m_module<m_nbmodule-1)
			m_module++;
		else
			m_module=0;
      }
   }

  // decrease channel
  if (c == '-' || c == '_') {
    // in channel display mode
      m_channel--;  
      if (m_channel<0) {
        m_channel=7;
        if(m_module>0)
			m_module--;
		else
			m_module=m_nbmodule-1;
    } 
   }
    
   // change display mode y-axis
  if (c == '1' || c == '2' || c == '0' || c == 'u' || c == 'd' || c == 'U' || c == 'D') {
    switch (c) {
      case '0': m_mode=0;  break;	
	break;
      case '1': m_mode=1;  break;	
	break;
      case '2': m_mode=2;  break;	
	break;
      case 'u': if (m_mode==1) m_min+=10;m_max-=10;	break;
      case 'U': if (m_mode==1) m_max+=10;	break;
      case 'd': if (m_mode==1) m_min-=10;m_max+=10;	break;
      case 'D': if (m_mode==1) m_max-=10;	break;
    }
  }



	return 0;
}






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

/*
Author: Julien Wulf UZH
*/


ScopeManager::ScopeManager()
{
	m_mode=m_channel=m_triggertype=m_module=m_nbmodule=m_mean=m_save=m_counter=m_ZLE=m_Baseline=0;
}

ScopeManager::~ScopeManager()
{
}

int ScopeManager::Init(){
	//m_length=m_length+10;
	ApplyXMLFile();
	//win = new TCanvas("win","JDAQ -- DAQ for Zuerich (multi)",1700,768);
	    
    	/*Slowcontrol Folder*/
    	string command= "mkdir Plots";
    	system(command.c_str());
    	std::cout << std::endl;


	single = new TCanvas("single","CDAQ -- DAQ for Zuerich (single)",1700,768);
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

int ScopeManager::ShowEvent(){

	
	//Start from the first word
    	pnt =0;

    	//Check first Word
    	if (buffer[0]==0xFFFFFFFF) pnt++;

    	// check header
    	if ((buffer[pnt]>>20)==0xA00 && (buffer[pnt+1]>>28)==0x0) {
       		 Size=((buffer[pnt]&0xFFFFFFF)-4);                   // size of full waveform (all channels)
       		 pnt++;
                 if(Size>0){

		//Read ChannelMask (Handbook)
	        int ChannelMask=buffer[pnt] & 0xFF;                 

		pnt++;    
    
	        // Get size of one waveform by dividing through the number of channels
	        cnt=0;
	        for (int j=0; j<8; j++) if ((ChannelMask>>j)&1) cnt++;
	        Size=Size/cnt;

	 	// ignore EventConter and TTT
		pnt+=2;
		m_mean=0;
		for (int j=0; j<8; j++) { // read all channels
	
			// read only the channels given in ChannelMask
			if ((ChannelMask>>j)&1) CurrentChannel=j;
		        else{ continue;}


			if(m_ZLE==0){
                	        if (CurrentChannel!=j) { pnt+=Size; continue; }

	                        if (j>j) return 0;

	                        cnt=0;                              // counter of waveform data
	                        wavecnt=0;                          // counter to reconstruct times within waveform

	      			while (cnt<(Size))
	      			{	
					double wave1=(double)((buffer[pnt]&0xFFFF));
					double wave2=(double)(((buffer[pnt]>>16)&0xFFFF));
					m_mean= m_mean+ wave1+ wave2;
					g[j]->SetBinContent(wavecnt,wave1);	
					g[j]->SetBinContent(wavecnt+1,wave2);	
		  			pnt++; wavecnt+=2; cnt++;

	      			} // end while(cnt...

				m_mean= (m_mean)/(wavecnt);
			}
			else{
		  		cnt=0;                              // counter of waveform data
                		wavecnt=0;                          // counter to reconstruct times within waveform
                		Size =  (buffer[pnt]);              //Size of the specific channel
                                if (CurrentChannel!=j) { pnt+=Size; continue; }
                                if (j>j) return 0;

                		cnt++;
                		pnt++;
                		int length;

                		 while(cnt<(Size)){
                        		//Skipped or Good Control World
                        		uint32_t control = buffer[pnt];
                        		cnt++;
                        		if(((control>>31)&1)){
        		                        length=(control&0xFFFFF);
                                		pnt++;
                        		}
                        		else {
                                		for(int i=wavecnt;i<(wavecnt+((control&0xFFFFF)*2) );i++){
							 g[j]->SetBinContent(i,m_Baseline);
						}
						wavecnt=wavecnt+ ( (control&0xFFFFF)*2);
                                		pnt++;
                                		if(cnt>=Size)
                                        		break;
                                		control = buffer[pnt];
                                		cnt++;
                                		if(((control>>31)&1)){
                                        		length=(control&0xFFFFF);
                                        		pnt++;
                               		 	}
                        		}
                        		for(int i=0;i<length;i++){
                        		        uint32_t wave1=((buffer[pnt]&0xFFFF));
	                                        uint32_t wave2=(((buffer[pnt]>>16)&0xFFFF));
                                                g[j]->SetBinContent(wavecnt,wave1);
	                                        g[j]->SetBinContent(wavecnt+1,wave2);
						wavecnt+=2; 
	                	        	pnt++;
                        	        	cnt++;
                        		}
               			 } //End while
			}
		   }
		}
	 }
	for(int i=0;i<8;i++){
	//	win->cd(1+i);
		graph_edit(g[i]);
   //		g[i]->Draw();
/*
  		if(m_triggertype==2)
			g[i]->SetTitle(Form("Channel:  %i , Threshold: %i",i,m_tresh[i]));
		else
			g[i]->SetTitle(Form("Channel:  %i , Module: %i",i,m_module));
*/
	}
//	    win->Modified();
//	    win->SetSelected(win);
 //	   win->Update();
    
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

    if(m_save==1){
 	single->SaveAs(Form("Plots/Event_%i.png",m_counter));
    }
    m_counter++;
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
      m_max=g->GetMaximumBin();
      m_min=g->GetMinimumBin();
      m_max=g->GetBinContent(m_max)+10;
      m_min=g->GetBinContent(m_min)-10;
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
	
	
	 // parse global ADC settings -----------------------------------------------
	XMLNode xNode=xMainNode.getChildNode("adc").getChildNode("global");
        xstr=xNode.getChildNode("baseline").getText();
        if (xstr) {
                strcpy(txt,xstr);
                m_Baseline=(int)atoi(txt);

        } else error((char*)"XML-baseline");

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

 //Save Waveforms
 if (c == 'p' || c == 'P') {
	m_save = 1;
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



	return 0;
}






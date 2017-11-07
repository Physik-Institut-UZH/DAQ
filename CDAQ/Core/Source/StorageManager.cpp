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
	m_NoE,m_EventsPerFile,m_WriteToFile,m_time,m_nbchs,m_filenumber=m_module=m_ZLE=0;
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
                m_command= "mkdir " + m_path + m_OutputFolder;
                system(m_command.c_str());
                std::cout << std::endl;
                
                /*Create Module Folder*/
                m_command= "mkdir " + m_path + m_OutputFolder +"/" + m_moduleName.c_str();
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

		/*Create ROOT Branches (hardcoded for 8 channels)*/
		if(channelActive[0])wf0= new int [m_length];
		if(channelActive[1])wf1= new int [m_length];
		if(channelActive[2])wf2= new int [m_length];
		if(channelActive[3])wf3= new int [m_length];
		if(channelActive[4])wf4= new int [m_length];
		if(channelActive[5])wf5= new int [m_length];
		if(channelActive[6])wf6= new int [m_length];
		if(channelActive[7])wf7= new int [m_length];

		if(channelActive[0])(tree)->Branch("wf0", wf0, TString::Format("wf0[%i]/I", m_length));
		if(channelActive[1])(tree)->Branch("wf1", wf1, TString::Format("wf1[%i]/I", m_length));
		if(channelActive[2])(tree)->Branch("wf2", wf2, TString::Format("wf2[%i]/I", m_length));
		if(channelActive[3])(tree)->Branch("wf3", wf3, TString::Format("wf3[%i]/I", m_length));
		if(channelActive[4])(tree)->Branch("wf4", wf4, TString::Format("wf4[%i]/I", m_length));
		if(channelActive[5])(tree)->Branch("wf5", wf5, TString::Format("wf5[%i]/I", m_length));
		if(channelActive[6])(tree)->Branch("wf6", wf6, TString::Format("wf6[%i]/I", m_length));
		if(channelActive[7])(tree)->Branch("wf7", wf7, TString::Format("wf7[%i]/I", m_length));
		(tree)->Branch("Time",&m_time,"Time/D");
		
		return 0;

}

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
	        /*Create ROOT Branches (hardcoded for 8 channels)*/
		if(channelActive[0]){
			tree->Branch("wf0_cw", "vector<int>", &m_zle_cw0);
                	tree->Branch("wf0", "vector<int>", &m_zle_wf0);
		}
                (tree)->Branch("Time",&m_time,"Time/D");
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
			
			if(channelActive[0])(tree)->Branch("wf0", wf0, TString::Format("wf0[%i]/I", m_length));
			if(channelActive[1])(tree)->Branch("wf1", wf1, TString::Format("wf1[%i]/I", m_length));
			if(channelActive[2])(tree)->Branch("wf2", wf2, TString::Format("wf2[%i]/I", m_length));
			if(channelActive[3])(tree)->Branch("wf3", wf3, TString::Format("wf3[%i]/I", m_length));
			if(channelActive[4])(tree)->Branch("wf4", wf4, TString::Format("wf4[%i]/I", m_length));
			if(channelActive[5])(tree)->Branch("wf5", wf5, TString::Format("wf5[%i]/I", m_length));
			if(channelActive[6])(tree)->Branch("wf6", wf6, TString::Format("wf6[%i]/I", m_length));
			if(channelActive[7])(tree)->Branch("wf7", wf7, TString::Format("wf7[%i]/I", m_length));
			(tree)->Branch("Time",&m_time,"Time/D");
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

int StorageManager::FillZLEROOTContainer(){


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
        for (int j=0; j<m_nbchs; j++) if ((ChannelMask>>j)&1) cnt++;

//        Size=Size/cnt;

        // ignore EventConter and TTT
        pnt+=2;

	for (int j=0; j<m_nbchs; j++) { // read all channels

                // read only the channels given in ChannelMask
                if ((ChannelMask>>j)&1) CurrentChannel=j;
                else continue;

                if (CurrentChannel!=j) { pnt+=Size; continue; }
               // else pnt++;

                if (j>j) return 0;

                cnt=0;                              // counter of waveform data
                wavecnt=0;                          // counter to reconstruct times within waveform
		Size =  (buffer[pnt]);		    //Size of the specific channel
	//	std::cout << Size << std::endl;
		cnt++;
		pnt++;
		uint32_t control;
		int length;
		int number_Control=0;
		while(cnt<Size){
			//Skipped or Good Control World
			control = buffer[pnt];
			cnt++;

//			if(cnt==Size){		//Last Word sometimes corrupt so we fix it by ourself
	//			std::cout << "Length Skipped:   " << ((m_length/2)-number_Control) << std::endl;
  //                              m_zle_cw0.push_back(-(((m_length/2)-number_Control)));
//				number_Control=number_Control+((m_length/2)-number_Control);
          //                      std::cout << number_Control << std::endl;
//				break;
//			}
			if(((control>>31)&1)){
				length=(control&0xFFFFF);
          //                      std::cout << "Length Good:      " << (control&0xFFFFF) << std::endl;
				number_Control=number_Control+(control&0xFFFFF);
				pnt++;
				m_zle_cw0.push_back((control&0xFFFFF));
			}
			else {
      //                  	std::cout << "Length Skipped:	" << (control&0xFFFFF) << "     " << cnt <<  std::endl;
                                number_Control=number_Control+(control&0xFFFFF);
	//			std::cout << number_Control << std::endl;
                                m_zle_cw0.push_back(-(control&0xFFFFF));
				pnt++;

				if(cnt>=Size)
					break;

				control = buffer[pnt];
                                cnt++;
				if(((control>>31)&1)){
                        		length=(control&0xFFFFF);
					m_zle_cw0.push_back((control&0xFFFFF));
    //                   			std::cout << "Length Good:	" << (control&0xFFFFF) << "	" << cnt <<  std::endl;
                                	number_Control=number_Control+(control&0xFFFFF);
  //                              	std::cout << number_Control << std::endl;
                			pnt++;
				}
				else{
//	                                std::cout << "Length Skipped:   " << (control&0xFFFFF) << std::endl;
					length=0;
//					pnt++;
				//	std::cout << "End	" << std::endl;
				}
			}
			for(int i=0;i<length;i++){
				uint32_t temp= (uint32_t)((buffer[pnt]&0xFFFF));
				m_zle_wf0.push_back(temp);
				temp= (uint32_t)(((buffer[pnt]>>16)&0xFFFF)) ;
                                m_zle_wf0.push_back(temp);
				pnt++;
				cnt++;
			}
		} //End while
//		std::cout << cnt << std::endl;
        } //end Channel
        m_time=GetUnixTime();
        tree->Fill();
	//clear
		m_zle_wf0.clear();
m_zle_cw0.clear();
    } //end Header
}



int StorageManager::FillROOTContainer(){
		float wvf[m_nbchs][m_length];
		

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
        for (int j=0; j<m_nbchs; j++) if ((ChannelMask>>j)&1) cnt++;
        Size=Size/cnt;

        // ignore EventConter and TTT
        pnt+=2;

        for (int j=0; j<m_nbchs; j++) { // read all channels

                // read only the channels given in ChannelMask
                if ((ChannelMask>>j)&1) CurrentChannel=j;
                else continue;

                if (CurrentChannel!=j) { pnt+=Size; continue; }
               // else pnt++;

                if (j>j) return 0;

                cnt=0;                              // counter of waveform data
                wavecnt=0;                          // counter to reconstruct times within waveform

		//Can be significant improved 
                while (cnt<(Size))
                {
			wvf[j][wavecnt]=(double)((buffer[pnt]&0xFFFF));
                	wvf[j][wavecnt+1]=(double)(((buffer[pnt]>>16)&0xFFFF));
                        pnt++; wavecnt+=2; cnt++;
                } // end while(cnt...)
        }
	//Simple Live Processing (Charge,Height,Position,Baseline,RMS) can be easly implemented here (alread looping over waveform) TODO
	for (int ii=0; ii<m_length; ii++) {
		if(channelActive[0])wf0[ii]=wvf[0][ii];
		if(channelActive[1])wf1[ii]=wvf[1][ii];
		if(channelActive[2])wf2[ii]=wvf[2][ii];
		if(channelActive[3])wf3[ii]=wvf[3][ii];
		if(channelActive[4])wf4[ii]=wvf[4][ii];
		if(channelActive[5])wf5[ii]=wvf[5][ii];
		if(channelActive[6])wf6[ii]=wvf[6][ii];
		if(channelActive[7])wf7[ii]=wvf[7][ii];
	}
	m_time=GetUnixTime();
	tree->Fill();
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
  
	xstr=xNode.getChildNode("nb_chs").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		m_nbchs=atoi(txt); 
	} else error((char*)"XML-nb_chs");

	//--- Active channels -------
	channelActive = new int[m_nbchs];
	
	for(int i=0;i<m_nbchs;i++){
        char channel[300];
        sprintf(channel,"ch_%i",i+m_module*m_nbchs);
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




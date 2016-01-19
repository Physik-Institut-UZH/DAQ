#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <string>
#include "IOManager.h"
#include "global.h"
#include "common.h"

//ROOT Libraries
#include "TROOT.h"
#include <TApplication.h>
#include "TSystem.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TRint.h"
#include "TApplication.h"
#include "keyb.h"

IOManager::IOManager(string path)
{
		m_path=path;
		m_flag=0;
		gettimestring(m_OutputFolder);
		printf(KGRN);
  		printf(":::: Message from IOManager (IOManager)::::\n");
       	        std::cout << "	Storage Path: " << m_path << std::endl;
       	        std::cout << "	Storage File: " << m_path + m_OutputFolder << std::endl << std::endl;; 
		printf(RESET);

		/*Create Output Folder and ROOT Folder*/
		m_command= "mkdir " + m_path; 
		system(m_command.c_str());
		m_command = m_path + m_OutputFolder + ".root";
		output = new TFile(m_command.c_str(), "RECREATE");
		tree = new TTree("T1","");
}

IOManager::IOManager(string path, int fileNumber)
{
		stringstream ss;
		ss << fileNumber;
		string OutputFolder = ss.str();
		m_path=path;
		m_flag=0;
		printf(KGRN);
  		printf(":::: Message from IOManager (IOManager)::::\n");
       	        std::cout << "	Storage Path: " << m_path << std::endl;
       	        std::cout << "	Storage File: " << m_path + OutputFolder << std::endl << std::endl;; 
		printf(RESET);

		/*Create Output Folder and ROOT Folder*/
		m_command= "mkdir " + m_path; 
		system(m_command.c_str());
		m_command = m_path + OutputFolder + ".root";
		output = new TFile(m_command.c_str(), "RECREATE");
		tree = new TTree("T1","");
}

IOManager::~IOManager()
{
	output->cd();
	tree->Write();
	output->Close();
}
void IOManager::FillContainer(vector<int> &channel, vector<double> &rates, int &event){
		m_chanels.clear();
		m_rates.clear();
		for(int i=0;i<rates.size();i++){
				m_rates.push_back(rates[i]);
		}
		int counter=0;
		printf(KCYN);
		printf(":::: Message from IOManager (FillContainer)::::\n	Event Number:%i\n\n", event);
		for(int i=0;i<channel.size();i++){
			m_chanels.push_back(channel[i]);

			if(m_chanels[i]==1){
				cout << "	Channel: " << i << "	Rate: " << m_rates[counter] << " Hz" << std::endl;
				if(m_flag==0)
					tree->Branch(Form("Ch%iRate",i),&m_rates[counter],Form("Ch%iRate/D",counter));
				counter++;
			}
		}
		if(m_flag==0){
			m_flag=1;
			tree->Branch("Time",&m_time,"Time/D");
			tree->Branch("EventNb",&m_event,"EventNb/D");
		}
		m_time=GetUnixTime();
		m_event=event;
		tree->Fill();
		printf(RESET);

}

void IOManager::SaveContainer(){
	output->cd();
	tree->Write();
        output->Close();
}







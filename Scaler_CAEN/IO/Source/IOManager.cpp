#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <string>
#include "IOManager.h"
#include "global.h"

//ROOT Libraries
#include "TROOT.h"
#include <TApplication.h>
#include "TSystem.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TRint.h"
#include "TApplication.h"


IOManager::IOManager(string path)
{
		m_path=path;
		m_flag=0;
		printf(KGRN);
  		printf(":::: Message from IOManager (IOManager)::::\n");
        std::cout << "	Storage Path: " << m_path << std::endl;
        std::cout << "	Storage File: " << m_path << std::endl << std::endl;; 
		printf(RESET);	
		
		/*Create Output Folder and ROOT Folder*/
		m_command= "mkdir " + m_path; 
		system(m_command.c_str());
		m_command = m_path + "test.root"; 
		output = new TFile(m_command.c_str(), "RECREATE");
		tree = new TTree("T1","");
}

IOManager::~IOManager()
{
	tree->Write();
	output->Close();
}
void IOManager::FillContainer(vector<int> &channel, vector<double> &rates){
		
		m_chanels.clear();
		m_rates.clear();
		for(int i=0;i<rates.size();i++){
				m_rates.push_back(rates[i]);
		}
		int counter=0;
		printf(KCYN);
		printf(":::: Message from IOManager (FillContainer)::::\n");
		for(int i=0;i<channel.size();i++){
			m_chanels.push_back(channel[i]);
			
			if(m_chanels[i]==1){
				cout << "	Counter: " << i << "	Rate: " << m_rates[counter] << " Hz" << std::endl;
				counter++;
				if(m_flag==0)
					tree->Branch(Form("Ch%iRate",i),&m_rates[i],Form("Ch%iRate/D",i));
			}
		}
		if(m_flag==0){
			m_flag=1;
			tree->Branch("Time",&m_time,"Time/D");
		}
				
		m_time=GetUnixTime();
		tree->Fill();
		printf(RESET);

}






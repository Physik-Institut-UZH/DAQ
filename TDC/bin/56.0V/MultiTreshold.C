#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>


#include "TTree.h"
#include  <TH1F.h>
#include "TFile.h"
#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TLegend.h>


using namespace std;
const int nChanels = 4;						//Number of channels to visualize in the root tree

const Style_t  MarkStyle = 31;

void MultiTreshold(){

	gStyle->SetLegendFont(350);

	vector<vector<int>> darkRate(nChanels);			//2D Vector for darkrate for several channels of several tresholds
	vector<vector<double>> mean(nChanels);
	vector<vector<double>> rms(nChanels);

	vector<double> minDrate(nChanels, 0.0);			//1D Vector for information of one treshold 
	vector<double> maxDrate(nChanels, 0.0);
	vector<double> ChiRate(nChanels);
	vector<int> thres;

	char branchName[nChanels][200];				//Information of the root file

                        TH1F *hist[nChanels];


	vector<vector<TGraphErrors*>> graphs(nChanels);		//Graphs to vizualize for every voltage
	vector<vector<string>> LegNames(nChanels);
	TMultiGraph *mg[nChanels];
	TCanvas *c1[nChanels];
	TLegend *leg[nChanels];

	int counter =0;
	int thr=0;
	//Dir name if several voltages has to be visualizes



	//File names visualize root file
	string name;
	ifstream inputFileName, FilePart;
	inputFileName.open("data.dat", ios::in);

	if (inputFileName.good()){
		inputFileName.seekg(0L, ios::beg);
		while (! inputFileName.eof()){
    		inputFileName >> name;
			if( inputFileName.eof() ) break;


			//Open File and set the adress
			TFile *f = new TFile(name.c_str());
			TTree *T1= (TTree*)f ->Get("T1");
			for(int ch=0; ch<nChanels; ch++){
				sprintf(branchName[ch], "Ch0Rate");
				T1->SetBranchAddress(branchName[ch], &ChiRate[ch]);
			}

                        //Find maxRate and minRate
                        int nev =  T1->GetEntries();
                                for(int k=0; k<nev;k++){
                                        T1->GetEntry(k);
                                        for(int ch=0; ch<nChanels; ch++){
                                                if(ChiRate[ch]>maxDrate[ch])
                                                        maxDrate[ch] = ChiRate[ch];
                                                else if(ChiRate[ch]<minDrate[ch])
                                                        minDrate[ch] = ChiRate[ch];
                                                darkRate[ch].push_back(ChiRate[ch]);
                                }
                        }


                        //Get RMS and Mean of one measurement
                        for(int ch=0; ch<nChanels; ch++){
                                hist[ch] = new TH1F(branchName[ch],branchName[ch], 5000, minDrate[ch], maxDrate[ch]);
                                for(int m = 0; m<nev; m++){
                                        hist[ch]->Fill(darkRate[ch][m]);
                                }
                                mean[ch].push_back(hist[ch]->GetMean());
                                rms[ch].push_back(hist[ch]->GetRMS());
                                hist[ch]->Draw("");
                                gPad->Modified();
                        }

                        thres.push_back(thr);
                        f->TFile::Close("R");

                        for(int ch=0; ch< nChanels; ch++)
                                darkRate[ch].clear();
                        thr++;
                        f->Close();
		} //End while
	}	  //End data file
	else{
//		printf(KRED);
//		std::cout  << "Cannot open: " << folder << endl;
//		printf(RESET);
//		return 0;
	}



	for(int ch=0; ch<nChanels; ch++){
		//for each voltage graph DC vs threshold:
		cout << thres.size() << std::endl;
		TGraphErrors* gr = new TGraphErrors(thres.size());
		graphs[ch].push_back(gr);
		for(int k=0; k<thres.size(); k++){
			graphs[ch][counter]->SetPoint(k,thres[k],mean[ch][k]);
			graphs[ch][counter]->SetPointError(k,0, rms[ch][k]);
		}
		graphs[ch][counter]->SetMarkerColor(1+counter);

		graphs[ch][counter]->SetMarkerStyle(31+counter);
		graphs[ch][counter]->SetLineColor(1+counter);
		graphs[ch][counter]->SetLineWidth(3);
		graphs[ch][counter]->SetLineStyle(1);
                graphs[ch][counter]->GetYaxis()->SetTitle("Rate [Hz]");
                graphs[ch][counter]->GetYaxis()->CenterTitle(1);

                graphs[ch][counter]->GetYaxis()->SetTitleOffset(1.1);
                graphs[ch][counter]->GetYaxis()->SetTitleSize(0.045);
                graphs[ch][counter]->GetYaxis()->SetTitleFont(72);
                graphs[ch][counter]->GetYaxis()->SetLabelSize(0.045);
                graphs[ch][counter]->GetYaxis()->SetLabelOffset(0.01);
                graphs[ch][counter]->GetYaxis()->SetLabelFont(72);

                graphs[ch][counter]->GetXaxis()->SetTitle("Treshold [mV]");
                graphs[ch][counter]->GetXaxis()->SetTitleSize(0.045);
                graphs[ch][counter]->GetXaxis()->SetTitleOffset(1.2);
                graphs[ch][counter]->GetXaxis()->SetTitleFont(72);
                graphs[ch][counter]->GetXaxis()->CenterTitle();
                graphs[ch][counter]->GetXaxis()->SetLabelFont(72);
                graphs[ch][counter]->GetXaxis()->SetLabelSize(0.045);
                graphs[ch][counter]->GetXaxis()->SetLabelOffset(0.01);

                graphs[ch][counter]->SetLineColor(kBlack);
                graphs[ch][counter]->SetLineWidth(2);
		std::stringstream ss;
		ss << "c" << ch;
		c1[ch]=  new TCanvas(ss.str().c_str(),"DarkCount over different tresholds",200,10,700,500);
		std::stringstream sss;
		sss << "Channel: " << ch << " DC over Threshold";
		graphs[ch][counter]->SetTitle("");
		graphs[ch][counter]->Draw("APL");
	}

        //Clear Evertything
        for(int ch=0; ch< nChanels; ch++){
		darkRate[ch].clear();
                maxDrate.clear();
                minDrate.clear();
                mean[ch].clear();
                rms[ch].clear();
         }

/*

	// draw all graphs as multigraph in one Canvas
	mg[0] = new TMultiGraph();
	for(int g = 0; g < nChanels; g++){
		mg[0]->Add(graphs[g][0]);
	}
	c1[0]=  new TCanvas("c2","A Simple Graph Example",200,10,700,500);
	c1[0]->SetLogy();
	mg[0]->Draw("APL");
	mg[0]->GetXaxis()->SetTitleFont(72);
	mg[0]->GetXaxis()->SetLabelFont(72);
	mg[0]->GetYaxis()->SetTitleFont(72);
	mg[0]->GetYaxis()->SetLabelFont(72);
*/
}







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
const int nChanels = 1;						//Number of channels to visualize in the root tree

const Style_t  MarkStyle = 20;
int color=0;

void MultiVoltage(){

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
	TMultiGraph *mg[nChanels];
	TCanvas *c1[nChanels];
	TLegend *leg[nChanels];
	vector<string> LegNames;
	int counter =0;
	//Dir name if several voltages has to be visualizes
	string command = string("ls -v -d *V> data.dat");
	//system(command.c_str());
	ifstream inputFolder;
	
	//File names visualize root file
	string folder;
	string name;
   	inputFolder.open("data.dat", ios::in);
	inputFolder.seekg(0L, ios::beg);
	if (inputFolder.good())
   	{
		while (inputFolder.good())
    	{	
			inputFolder >> folder;
			LegNames.push_back(folder);
			std::cout << folder << std::endl;
			if( inputFolder.eof()) break;

			command = string("ls -v " + folder + "/*.root" + " >" + folder + "/data.dat");
			system(command.c_str());
			command= folder +"/data.dat";

			ifstream inputFileName;
			inputFileName.open(command.c_str(), ios::in);
			int thr=0;
			if (inputFileName.good()){
				inputFileName.seekg(0L, ios::beg);
				while (! inputFileName.eof()){
					inputFileName >> name;
					if( inputFileName.eof() ) break;

					//Open File and set the adress
					TFile *f = new TFile(name.c_str());
					TTree *T1= (TTree*)f ->Get("T1");
					for(int ch=0; ch<nChanels; ch++){
						sprintf(branchName[ch], "Ch3Rate");
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
				std::cout  << "Cannot open: " << folder << endl;
			}


			for(int ch=0; ch<nChanels; ch++){
				//for each voltage graph DC vs threshold:
				//cout << thres.size() << std::endl;
				TGraphErrors* gr = new TGraphErrors(thres.size());
				graphs[ch].push_back(gr);
				for(int k=0; k<thres.size(); k++){
					graphs[ch][counter]->SetPoint(k,thres[k],mean[ch][k]);
					graphs[ch][counter]->SetPointError(k,0, rms[ch][k]);
				}
				graphs[ch][counter]->SetMarkerColor(1+color);
				graphs[ch][counter]->SetMarkerStyle(MarkStyle+counter);
				graphs[ch][counter]->SetLineColor(1+color);
				color++;
                                if(color==9){
                                	color=0;
				}
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

				//graphs[ch][counter]->SetLineColor(kBlack);
				graphs[ch][counter]->SetLineWidth(2);

				//c1[ch]=  new TCanvas(ss.str().c_str(),"DarkCount over different tresholds",200,10,700,500);
				std::stringstream sss;
				sss << "Channel: " << ch << " DC over Threshold";
				graphs[ch][counter]->SetTitle(sss.str().c_str());
				//graphs[ch][counter]->Draw("APL");
			}

				//Clear Evertything
				for(int ch=0; ch< nChanels; ch++){
					darkRate[ch].clear();
						maxDrate.clear();
						minDrate.clear();
						mean[ch].clear();
						rms[ch].clear();
                                                thres.clear();
				}
				counter++;
		}
	}
	else{
		//	printf(KRED);
			std::cout  << "Cannot open: " << endl;
		//	printf(RESET);
		//	return 0;
	}
	
	// draw all graphs as multigraph in one Canvas
	for(int g=0;g<nChanels;g++){
		mg[g] = new TMultiGraph();
		leg[g] = new TLegend(0.91, 0.1, 0.99, 0.9);
		for(int i=0;i<counter;i++){				//Minus the last 15 measurements
			mg[g]->Add(graphs[g][i]);
			leg[g]->AddEntry(graphs[g][i], LegNames[i].c_str(), "p");	  // one mg consist ngr graps for different volt

		}	
		std::stringstream ss;
		ss << "c" << g;
		c1[g]=  new TCanvas(ss.str().c_str(),"A Simple Graph Example",200,10, 1920, 1080);
		c1[g]->SetLogy();
		mg[g]->Draw("APL");
		mg[g]->SetTitle("Ch 0 Darkcount over Threshold at T=299K GXe");
		mg[g]->GetXaxis()->SetTitleFont(72);
		mg[g]->GetXaxis()->SetLabelFont(72);
		mg[g]->GetYaxis()->SetTitleFont(72);
		mg[g]->GetYaxis()->SetLabelFont(72);
        mg[g]->GetYaxis()->SetTitle("Rate [Hz]");
        mg[g]->GetYaxis()->CenterTitle(1);
        mg[g]->GetYaxis()->SetTitleOffset(1.1);
        mg[g]->GetYaxis()->SetTitleSize(0.045);
       	mg[g]->GetYaxis()->SetTitleFont(72);
        mg[g]->GetYaxis()->SetLabelSize(0.045);
        mg[g]->GetYaxis()->SetLabelOffset(0.01);
        mg[g]->GetYaxis()->SetLabelFont(72); 
        mg[g]->GetXaxis()->SetTitle("Threshold [mV]");
       	mg[g]->GetXaxis()->SetTitleSize(0.045);
        mg[g]->GetXaxis()->SetTitleOffset(1.2);
        mg[g]->GetXaxis()->SetTitleFont(72);
        mg[g]->GetXaxis()->CenterTitle();
       	mg[g]->GetXaxis()->SetLabelFont(72);
       	mg[g]->GetXaxis()->SetLabelSize(0.045);
	    mg[g]->GetXaxis()->SetLabelOffset(0.01);
	    leg[g]->Draw();		
		gPad->Modified();
   }
   
   
   	ifstream PEthreshhold;
	
	int number = 16;
	int channel =0;
	//File names visualize root file
	vector<vector<double>> data(22);
	double tmp;
	int countertwo=0;
//   	ifstream PEthreshhold;
   	PEthreshhold.open("PETH_CH3", ios::in);
   	
	if (PEthreshhold.good())
    {
     	PEthreshhold.seekg(0L, ios::beg);
        while (! PEthreshhold.eof())
        {
			for(int i=0;i<3;i++){
				PEthreshhold >> tmp;
				data[countertwo].push_back(tmp);
			}
			for(int i=0;i<3;i++)
				cout << "	" << data[countertwo][i]; 
				cout << std::endl;
				if( PEthreshhold.eof() ) break;
            	countertwo++;
        }
     }
     
     
    //Plot DC over Voltage at 0.5PE
    double factor =0.1220703125;
    TCanvas *c3 = new TCanvas("c3", "", 750, 550);
	gStyle->SetOptStat(kFALSE);

	c3->SetTickx();
	c3->SetTicky();
	//c->SetGridy();
	c3->cd();
	TH2F *b = new TH2F("b", "", 400, 65.0, 68.0, 700, 800000,3000000);
	
	b->SetStats(0);
	b->SetTitle("");
	b->GetXaxis()->SetTitle("Bias Voltage [V]");
	b->GetXaxis()->SetTitleFont(72);
	b->GetXaxis()->SetLabelFont(72);
	b->GetXaxis()->SetTitleSize(0.04);
	b->GetXaxis()->SetLabelSize(0.04);
	b->GetXaxis()->CenterTitle();
	
	b->GetYaxis()->SetTitle("DC [Hz]");
	b->GetYaxis()->SetTitleFont(72);
	b->GetYaxis()->SetLabelFont(72);
	b->GetYaxis()->CenterTitle();
	b->GetYaxis()->SetTitleSize(0.04);
	b->GetYaxis()->SetLabelSize(0.04);
	b->GetYaxis()->SetTitleOffset(1.2);

	TGaxis *Y = (TGaxis*)b->GetYaxis();
	Y->SetMaxDigits(3);
	b->Draw();
	
	TCanvas *c4 = new TCanvas("c4", "", 750, 550);
	c4->SetTickx();
	c4->SetTicky();
	//c->SetGridy();
	c4->cd();
	TH2F *f1 = new TH2F("f", "", 400, 65.0, 68.0, 700, 0, 100.0);
	
	f1->SetStats(0);
	f1->SetTitle("");
	f1->GetYaxis()->SetTitle("Crosstalk Probability [%]");
	f1->GetXaxis()->SetTitleFont(72);
	f1->GetXaxis()->SetLabelFont(72);
	f1->GetXaxis()->SetTitleSize(0.04);
	f1->GetXaxis()->SetLabelSize(0.04);
	f1->GetXaxis()->CenterTitle();
	
	f1->GetXaxis()->SetTitle("Bias Voltage [V]");
	f1->GetYaxis()->SetTitleFont(72);
	f1->GetYaxis()->SetLabelFont(72);
	f1->GetYaxis()->CenterTitle();
	f1->GetYaxis()->SetTitleSize(0.04);
	f1->GetYaxis()->SetLabelSize(0.04);
	f1->GetYaxis()->SetTitleOffset(1.2);

	TGaxis *Y1 = (TGaxis*)f1->GetYaxis();
	Y1->SetMaxDigits(3);
	f1->Draw();
	
	TGraphErrors* plots = new TGraphErrors(number);
	TGraphErrors* ct = new TGraphErrors(number);
	std::fstream fs;
  	fs.open ("ch3.txt",std::fstream::out);
	for(int i=0;i<number;i++){
		double dc05 = data[i][1]*factor*0.5;
		double dc15 = (data[i][2]*factor+data[i][1]*factor)/2;
		cout << "Height 0.5PE:	" << dc05;
		std::cout << std::endl;
		cout << "Height 1.5PE:	" << dc15;
		std::cout << std::endl;
		int point; 
		point = dc05;
		int poin;
		poin = dc15;
		std::cout << "Error:	" << graphs[channel][i]->GetErrorY(point) << std::endl;
                std::cout << std::endl;
		std::cout << "DC 0.5PE:	" << graphs[channel][i]->Eval(dc05);
		std::cout << std::endl;
		std::cout << "DC 1.5PE:	" << graphs[channel][i]->Eval(dc15);
		std::cout << std::endl;
		double dc005 = graphs[channel][i]->Eval(dc05);
		double dc015 = graphs[channel][i]->Eval(dc15);
		std::cout << "CT:	" << (dc015/dc005)*100 ;
		std::cout << std::endl;
		plots->SetLineColor(1);
		plots->SetLineWidth(2);

		ct->SetLineColor(1);
		ct->SetLineWidth(2);
		plots->SetPoint(i,data[i][0],graphs[channel][i]->Eval(dc05));
		plots->SetPointError(i,0.01,0);	

		ct->SetPoint(i,data[i][0],(dc015/dc005)*100 );
		ct->SetPointError(i,0.01,0);
	        fs << data[i][0] << "	" << graphs[channel][i]->Eval(dc05) << "	" << (dc015/dc005)*100  << "	" << graphs[channel][i]->GetErrorY(point) << "	" << graphs[channel][i]->GetErrorY(poin) <<  "\n";
		}
	c3->cd();
	plots->Draw("P");

	c4->cd();
	ct->Draw("P");
	fs.close();

	
}







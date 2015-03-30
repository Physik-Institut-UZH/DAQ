#ifndef PHOTONCALIBRATION_H
#define PHOTONCALIBRATION_H

#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h> 
#include <string.h>
#include <fstream> 


/*Author: Julien Wulf UZH
  	Data Processor for calibration of  every kind of Photondetectors. 
	
*/

using namespace std;
class PhotonCalibration
{
  public:                            
	PhotonCalibration();   
        void WriteStateFile();  	
	void ComputeSpectrum();
	void PlotRandomTrace();
	void PlotSpectrum();
	void CreateROOTFile();
  private:
        void ComputeAreaOfPulse();	
	int ht, lt; // High treshhold and low treshhold
	int length, height; //
	fstream data;
	string Message;

};

#endif
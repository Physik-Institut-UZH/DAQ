// Julien W. Rizalina M.
// Program to set the threshold for the v895 Leding Edge Discriminator

//C++ Libraries
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include "discriminator.h"

#include <cmath>
#include <map>
#include <math.h>
#include <vector>
#include <sys/time.h>

//ROOT Libraries
#include "TROOT.h"
#include <TApplication.h>
#include "TSystem.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TRint.h"
#include "TApplication.h"


//const int measurement = 1000;


using namespace std;

int main(int argc, char *argv[]){ // argv[1] - name of the main cfg;
	discriminator fDisc(argv[1]); // read cfg files and thr Files in constructur
	int fCrateHandle=fDisc.GetfCrateHandle();
	int Nthr = fDisc.GetNthresholds();

	u_int32_t chHex =0xFFFFFF; // wtf is it? Should be read from cfg?
	u_int32_t fVMEAddressSca=fDisc.GetVMEAddressSca();

	//Enable All Channels
	if(CAENVME_WriteCycle(fCrateHandle,fVMEAddressSca+0x1100, &chHex,cvA32_U_DATA,cvD32)!=cvSuccess) //0-31
		cout << "VME write error" << endl;
	else
		cout << "VME write successful" << endl;
	//Save Directory
	
	for(int i=0; i< Nthr; i++){
		fDisc.SetThresholds2(i);
		fDisk.DarkCount2Root(i);
	}

	fDisc.Close();
	std::cout << endl << endl;

	return 0;
}

   
   
  

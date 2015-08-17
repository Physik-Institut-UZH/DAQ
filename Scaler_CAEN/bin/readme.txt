Readme to scaler2:

The program performs DC mesuremts using the scaler and store the obtained data for each single threshold in a root file 

1. create a file with number of scaler chanel and thresholds are needed to be used for taking DC data form teh chanel:

e.g

=====scalerThresholds.txt=======

0 1 2 3 4 5 6
^ ^ ^ ^ ^ ^ ^ 
| | | | | | | 
n_chanel
  | | | | | | 
   thresholds for this chanel
   
   
   
 ================================== 	
   
   
 2. create the initial config file (keeping the name of variables)
  
 e.g
 ====scaler.ini======================
 
SCALER_ADDRESS EEEE0000
VMELink 0

   
 3. create the main config file (keeping the name of variables)
 
 e.g
 ====ScalerCfg======================
 
ConfigIni = "scaler.ini"  					// adress to initial cfg file
ThresholdsFile="scalerThresholds.txt"		// adress to file with the list thresholds have to be used for DC

fVMEAddressSca = "0xDDDD0000"				//physical adress of the scaler
VoltageSetForChanels = 50 50 1500			//operating voltage values are used for DC 
AdressesForChanels = 00 02 04				//physical adresses of the scaler are used are used for DC 
DCevents = 1000 							//reauared number of event for DC mesurements
OutputRootDir = "data/LArSetup/Rizalina/Lar_PMT/RoomTempr/DarkCount/"   //the output dir for the program where root files for each singles will be stored
 


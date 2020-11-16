//C++ Libraries
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <cmath>
#include <map>
#include <math.h>
#include <vector>
#include <sys/time.h>
#include <string>
#include <Riostream.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <string>
#include "ADCManager1730_16Ch.h"
#include "ADCManager.h"
#include "global.h"

//Root Libaries
#include <TH1D.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TFile.h>
#include <TLatex.h>
#include "TTree.h"
#include "TFile.h"
#include "TROOT.h"

/*
Author:Neil McFadden 22.09.2020
Rewrote code from J. Wulf using CAEN digitzer libraries rather than directly writting into ADC memomry because
J.Wulf's code could not be converted from 8 channels to 16 channels. Maybe N.McFadden was too dumb...
*/

//Note the dummy ADCManager class is intialized here
ADCManager1730_16Ch::ADCManager1730_16Ch():ADCManager(0)
{
	m_CrateHandle= m_ADCaddr=m_EnableMask=0;
	m_EnableVMEIrq=m_Align64=m_EnableBerr=m_EnableOLIrq=m_EnableInt=m_EvAlign=m_Frequency=m_Baseline=m_resDAC=m_Voltage=m_nbCh=m_triggertyp=m_SoftwareRate=m_module=0;
  Event16=NULL; /* generic event struct with 16 bit data (10, 12, 14 and 16 bit digitizers */
  m_handle = -1;
	for(int i=0;i<16;i++){
		m_DACTarget[i]=0;
 		m_DACLevel[i]=0;
		m_DACFinished[i]=0;
	}
}

ADCManager1730_16Ch::~ADCManager1730_16Ch()
{
}

int ADCManager1730_16Ch::Init(){

	
	//printf(KYEL);
	//printf("\nReset the ADC Module %i . . .\n\n",m_module);
	//printf(RESET);

  //Reset the board first
  //m_hex=0x1;
  //adc_writereg(SoftwareResetReg,m_hex);
  //sleep(3);

  //Set Register Setting from default file (only to give the user some freedom)
  //RegisterWriting(m_RegisterFileName);

  //TODO turn these into global variables, check if VMEBaseAddress can be anything is connected via optical link
  /*
  m_Ret = CAEN_DGTZ_OpenDigitizer(CAEN_DGTZ_OpticalLink, 0 Link Num, (Link number in daisy Chain), 0 Conet Node, optical link number of PCI board , 0 VMEBaseAddress (only for USB), &m_handle);
  if(abs((int)m_Ret) == 25) cout<<"Digitizer already open"<<endl;
  else if (m_Ret) {
    cout<<m_handle<<endl;
    cout<<"Opening Digitizer, "<<errors[abs((int)m_Ret)]<<" (code "<<m_Ret<<")"<<endl;
    return 1;
  }
  */

  //Set Register/other Settings from xml-file
	if(ApplyXMLFile()) return 1;

	if(OpenDigitizer()) return 1;

  if(CalculateBaseline()) return 1;

	//Read Settings
	//RegisterReading();

	printf(KYEL);
	printf("Calibrate the ADC . . .\n");
	printf(RESET);

  //calibration of the Channels with the current Setting
  m_hex=0x1;
  adc_writereg(ChannelCalibrationReg,m_hex);
  sleep(0.1);	

	return 0;
}


bool ADCManager1730_16Ch::OpenDigitizer(){
	
  CAEN_DGTZ_ErrorCode m_Ret = CAEN_DGTZ_Success;
  
  /* *************************************************************************************** */
  /* Open the digitizer and read the board information                                       */
  /* *************************************************************************************** */

  //Digitizer is opened in VMEManager
  //m_Ret = CAEN_DGTZ_OpenDigitizer(CAEN_DGTZ_OpticalLink, 0 /*Link Num, (Link number in daisy Chain)*/, 0/*Conet Node, optical link number of PCI board */, 0/*VMEBaseAddress (only for USB)*/, &m_handle);
  /*
  if(abs((int)m_Ret) == 25) cout<<"Digitizer already open"<<endl;
  else if (m_Ret) {
    cout<<m_handle<<endl;
    cout<<"Opening Digitizer, "<<errors[abs((int)m_Ret)]<<" (code "<<m_Ret<<")"<<endl;
    return 1;
  }
  */

  m_Ret = CAEN_DGTZ_GetInfo(m_handle, &BoardInfo);
  if (m_Ret) {
    cout<<"GetInfo "<<errors[abs((int)m_Ret)]<<" (code "<<m_Ret<<")"<<endl;
    return 1;
      
  }
  printf("Connected to CAEN Digitizer Model %s\n", BoardInfo.ModelName);
  printf("ROC FPGA Release is %s\n", BoardInfo.ROC_FirmwareRel);
  printf("AMC FPGA Release is %s\n", BoardInfo.AMC_FirmwareRel);

  //Get the length of buffer, Set in XML file
  m_Ret = CAEN_DGTZ_GetRecordLength(m_handle,&m_BufferSize);
  if(m_Ret) {
    cout<<"Get Record Length "<<errors[abs((int)m_Ret)]<<" (code "<<m_Ret<<")"<<endl;
    return 1;
  }

  //
  //**************//
  //Programming ADC//
  //**************//
  //

  m_Ret = CAEN_DGTZ_Reset(m_handle);
  if (m_Ret != 0) {
    printf("Error: Unable to reset digitizer.\nPlease reset digitizer manually then restart the program\n");
    cout<<errors[abs((int)m_Ret)]<<" (code "<<m_Ret<<")"<<endl;
    return 1;
  }

  //For debugging only, if TestPattern = 1 and 
  int TestPattern = 0;
  if (TestPattern) m_Ret = CAEN_DGTZ_WriteRegister(m_handle, CAEN_DGTZ_BROAD_CH_CONFIGBIT_SET_ADD, 1<<3);
  
  m_Ret = CAEN_DGTZ_SetRecordLength(m_handle, m_RecordLength);
  if(m_Ret) cout<<"Failed to set record Length"<<endl;
  else cout<<"Set Record length to "<<m_RecordLength<<endl;

  //The size is expressed in percentage of the record length.
  //0% means that the trigger is at the end of the window, 
  //while 100% means that it is at the beginning.
  m_Ret = CAEN_DGTZ_SetPostTriggerSize(m_handle, m_PostTrigger);
  if(m_Ret) cout<<"Post Trigger failed to be set"<<endl;
  else cout<<"Post Trigger set to "<<m_PostTrigger<<" percent"<<endl;
  
  m_Ret = CAEN_DGTZ_SetIOLevel(m_handle, m_TriggerLogicType);
  if(m_Ret) cout<<"Failed to set the logic level (NIM/TTL)"<<endl;
 
  m_Ret = CAEN_DGTZ_SetMaxNumEventsBLT(m_handle, m_NumEvents);
  m_Ret = CAEN_DGTZ_SetAcquisitionMode(m_handle, CAEN_DGTZ_SW_CONTROLLED);

  //TODO set this via XML
  //m_ExtTriggerMode = CAEN_DGTZ_TRGMODE_ACQ_ONLY;
  //m_ExtTriggerMode = CAEN_DGTZ_TRGMODE_DISABLED;
  //This function decides whether the external trigger should only be used to generate the acquisition trigger, only to generate the trigger output, or both.
  m_Ret = CAEN_DGTZ_SetExtTriggerInputMode(m_handle, m_ExtTriggerMode);
  m_Ret = CAEN_DGTZ_SetSWTriggerMode(m_handle, m_SWTriggerMode);

  //EnableMask is a binary number that is used to turn on/off channels, i.e. if you want
  //channel 5 and 14 enables m_EnableMask would be 01000000000100000 but in decimal (32800) rather than binary
  m_Ret = CAEN_DGTZ_SetChannelEnableMask(m_handle, m_EnableMask);
  
  //TODO
  ///*
  for (int i = 0; i < m_nbCh; i++) {
    if (m_EnableMask & (1<<i)) {
      m_Ret = CAEN_DGTZ_SetChannelTriggerThreshold(m_handle, i, m_channelThresh[i]);
      m_Ret = CAEN_DGTZ_SetTriggerPolarity(m_handle, i, m_trigPolarity[i]); //.TriggerEdge
      //m_Ret = CAEN_DGTZ_SetChannelSelfTrigger(m_handle,CAEN_DGTZ_TRGMODE_ACQ_ONLY,(1<<i));
    }
  }
  //*/


  
  // Read again the board infos, just in case some of them were changed by the programming
  // (like, for example, the TSample and the number of channels if DES mode is changed)
  m_Ret = CAEN_DGTZ_GetInfo(m_handle, &BoardInfo);
  if (m_Ret) {
    cout<<"Could not get info again after programming board"<<endl;
    cout<<errors[abs((int)m_Ret)]<<" (code "<<m_Ret<<")"<<endl;
    return 1;
  }

  // Allocate memory for the event data and readout buffer
  m_Ret = CAEN_DGTZ_AllocateEvent(m_handle, (void**)&Event16);
	    
  if (m_Ret != CAEN_DGTZ_Success) {
    cout<<"Failed to allocate memory"<<endl;
    return 1;
  }
  
  // WARNING: This malloc must be done after the digitizer programming
  m_Ret = CAEN_DGTZ_MallocReadoutBuffer(m_handle, &m_buffer,&m_AllocatedSize); 
  if (m_Ret) {
    cout<<"Failed to Malloc the Readout Buffer"<<endl;
    return 1;
  }


  return 0;	  
}

//setup for an 'OR' trigger
CAEN_DGTZ_ErrorCode ADCManager1730_16Ch::OrProgrammer(){
  int m_ret=0;
  
  /*
  int triggerLogicBaseAddress=0x1084;
  
  // channel pair settings for x730 boards
  for (int i = 0; i < 8; i += 2) {
    if (EnableMask & (0x3 << i)) {
      CAEN_DGTZ_TriggerMode_t mode = ChannelTriggerMode[i];
      uint32_t pair_chmask = 0;
	
      // Build mode and relevant channelmask. The behaviour is that,
      // if the triggermode of one channel of the pair is DISABLED,
      // this channel doesn't take part to the trigger generation.
      // Otherwise, if both are different from DISABLED, the one of
      // the even channel is used.


      if (ChannelTriggerMode[i] != CAEN_DGTZ_TRGMODE_DISABLED && ChannelTriggerMode[i + 1] != CAEN_DGTZ_TRGMODE_DISABLED) {
        //channel i and i+1 are both enabled
        if(verbose) cout<<"Channel "<<i<<" and "<<i+1<<" enableddd\n";
        m_ret |= adc_writereg(triggerLogicBaseAddress+256*i, 0x3, 0xFFFFFFFF);
        pair_chmask = (0x3 << i);

      } else if(ChannelTriggerMode[i] != CAEN_DGTZ_TRGMODE_DISABLED && ChannelTriggerMode[i + 1] == CAEN_DGTZ_TRGMODE_DISABLED) {
        //channel i enabled, channel i+1 disabled
        if(verbose) cout<<"Channel "<<i<<" enabled and "<<i+1<<" disabled\n";
        pair_chmask = (0x1 << i); 
        m_ret |= adc_writereg(triggerLogicBaseAddress+256*i, 0x1, 0xFFFFFFFF);

      }	else if(ChannelTriggerMode[i] == CAEN_DGTZ_TRGMODE_DISABLED&&ChannelTriggerMode[i+1] != CAEN_DGTZ_TRGMODE_DISABLED){
        //channel i disabled, channel i+1 enabled
        if(verbose) cout<<"Channel "<<i<<" disabled and "<<i+1<<" enabled\n";
        m_ret |= adc_writereg(triggerLogicBaseAddress+256*i, 0x2, 0xFFFFFFFF);
        mode = ChannelTriggerMode[i + 1];
        pair_chmask = (0x2 << i);
      }else{
        if cout<<"Channel "<<i<<" and "<<i+1<<" disabled\n";
      }




      pair_chmask &= EnableMask;
      m_ret |= CAEN_DGTZ_SetChannelSelfTrigger(m_handle, mode, pair_chmask);

    }
  }
  */
  return (CAEN_DGTZ_ErrorCode)m_ret;

}

int ADCManager1730_16Ch::CalculateBaseline(){
  //return CAEN_DGTZ_Success;
  
  float cal = 1;
  float offset = 0;
  int i = 0, k = 0, p = 0, acq = 0, ch=0;
  CAEN_DGTZ_ErrorCode m_ret;
  CAEN_DGTZ_AcqMode_t mem_mode;
  uint32_t  AllocatedSize;

  uint32_t BufferSize;
	
  CAEN_DGTZ_EventInfo_t       EventInfo;
  char *buffer = NULL;
  char *EventPtr = NULL;
  CAEN_DGTZ_UINT16_EVENT_t    *Event16 = NULL;
  int cal_ok[m_nbCh];

  m_ret = CAEN_DGTZ_GetAcquisitionMode(m_handle, &mem_mode);//chosen value stored
  if (m_ret)
    printf("Error trying to read acq mode!!\n");
  m_ret = CAEN_DGTZ_SetAcquisitionMode(m_handle, CAEN_DGTZ_SW_CONTROLLED);
  if (m_ret)
  cout<<errors[abs((int)m_ret)]<<" (code "<<m_ret<<")"<<endl;
  m_ret = CAEN_DGTZ_SetExtTriggerInputMode(m_handle, CAEN_DGTZ_TRGMODE_DISABLED);
  if (m_ret)
    printf("Error trying to set ext trigger!!\n");
  m_ret = CAEN_DGTZ_SetSWTriggerMode(m_handle, CAEN_DGTZ_TRGMODE_ACQ_ONLY);
  if (m_ret)
    printf("Someshit is bad, maybe try it agian but with less pizazz");
  m_ret = CAEN_DGTZ_SetPostTriggerSize(m_handle, 0);
  if (m_ret)
    printf("Error trying to set post trigger!!\n");
  ///malloc
  m_ret = CAEN_DGTZ_MallocReadoutBuffer(m_handle, &buffer, &AllocatedSize);
  if (m_ret) {
    cout<<errors[abs((int)m_ret)]<<" (code "<<m_ret<<")"<<endl;
    return m_ret;
  }

  m_ret = CAEN_DGTZ_AllocateEvent(m_handle, (void**)&Event16);		
  if (m_ret != CAEN_DGTZ_Success) {
    cout<<errors[abs((int)m_ret)]<<" (code "<<m_ret<<")"<<endl;
  }

  m_ret = CAEN_DGTZ_SWStartAcquisition(m_handle);
  if (m_ret){
    printf("Warning: error starting acq\n");
    cout<<errors[abs((int)m_ret)]<<" (code "<<m_ret<<")"<<endl;
    return m_ret;
  }

  #define NPOINTS 2
  #define NACQS   50
  float avg_value[NPOINTS] = { 0 };
	
  uint32_t dc[NPOINTS] = {25,75}; //test values, baseline at 25% and 75% 

  m_DCoffset = new uint32_t[m_nbCh];
  printf("Calculating baseline...\n");
  for (ch = 0; ch < (int32_t)BoardInfo.Channels; ch++)    {
    if (m_EnableMask & (1 << ch)){
      m_ret = CAEN_DGTZ_SetChannelSelfTrigger(m_handle,CAEN_DGTZ_TRGMODE_DISABLED, (1 << ch));			
      if (m_ret)
        printf("Warning: error disabling ch %d self trigger\n", ch);


      for (p = 0; p < NPOINTS; p++){
        //Make a guess for the DC offset
        m_ret = CAEN_DGTZ_SetChannelDCOffset(m_handle, (uint32_t)ch, (uint32_t)((float)(fabs((int)dc[p] - 100))*(655.35)));
        if (m_ret)
          printf("Warning: error setting ch %d test offset\n", ch);

        //usleep(200000);
        usleep(50000);

        int value[NACQS] = { 0 };
        for (acq = 0; acq < NACQS; acq++){
          CAEN_DGTZ_SendSWtrigger(m_handle);
          m_ret = CAEN_DGTZ_ReadData(m_handle, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT, buffer, &BufferSize);
          if (m_ret) {
            cout<<errors[abs((int)m_ret)]<<" (code "<<m_ret<<")"<<endl;
            exit(0);
            }

          m_ret = CAEN_DGTZ_GetEventInfo(m_handle, buffer, BufferSize, 0, &EventInfo, &EventPtr);
          if (m_ret) {
            cout<<errors[abs((int)m_ret)]<<" (code "<<m_ret<<")"<<endl;
            return m_ret;
          }
          // decode the event //
          m_ret = CAEN_DGTZ_DecodeEvent(m_handle, EventPtr, (void**)&Event16);

          if (m_ret) {
            cout<<errors[abs((int)m_ret)]<<" (code "<<m_ret<<")"<<endl;
            return m_ret;
          }


          for (i = 1; i < 7; i++){ //mean over 6 samples
            value[acq] += (int)(Event16->DataChannel[ch][i]);
          }
          value[acq] = (value[acq] / 6);
        }//for acq

        //check for clean baselines
        //I (N.M.) found this baseline estimation method from someone on github, the basic idea is to 
        //estimate the average baseline from 50 waveforms by looking at the first 6 samples of the 
        //waveform and use that to set your DC offset. This is performed twice for two different baseline guess
        //Some nuisances...
        //Intially the base line is set to be 25% and 75%  and the code sees how well that baseline
        //is maintained after accounting for noise. This estimate is the value Cal
        //The baseline needs to be inserted as a value from 0x0000 to 0xFFFF (i.e. 0 to 65535). If the DAC is set to
        //0x0000, then no DC offset is added, and the range of the input signal goes from -Vpp to 0. Conversely, when the DAC is
        //set to 0xFFFF, the DC offset is Vpp and the range goes from 0 to +Vpp. 
        //When calculating the average baseline, ADC counts are used, this have to be converted into the accetable range to progem the DAQ
        //I have added a XML command to leave some fraction of baseline below the signal incase unexpected signals arrive
        int max = 0;
        int mpp = 0;
        int size = (int)pow(2, (double)BoardInfo.ADC_NBits);//returns 2^14 = 16384
        int *freq = (int*)calloc(size, sizeof(int));

        for (k = 0; k < NACQS; k++){
          if (value[k] > 0 && value[k] < size){
            freq[value[k]]++;
            if (freq[value[k]] > max) {
              max = freq[value[k]]; 
              mpp = value[k]; 
            }
          }
        }

        free(freq);
        int ok = 0;
        for (k = 0; k < NACQS; k++) {
          if (value[k] == mpp || value[k] == (mpp + 1) || value[k] == (mpp - 1)){
            avg_value[p] = avg_value[p] + (float)value[k]; ok++;
          }
        }
        //Convert ADC counts into a percentage
        avg_value[p] = (avg_value[p] / (float)ok)*100. / (float)size; 

      }//close for p
      cal = ((float)(avg_value[1] - avg_value[0]) / (float)(dc[1] - dc[0]));
      offset = (float)(dc[1] * avg_value[0] - dc[0] * avg_value[1]) / (float)(dc[1] - dc[0]);
      //printf("Cal %f   offset %f\n", cal, offset);


      if (m_PulsePolarity[ch] == CAEN_DGTZ_PulsePolarityPositive){
        //DCoffset[ch] = (uint32_t)((float)(fabs(( ((float)dc_file[ch] - offset )/ cal ) - 100.))*(655.35));
        m_DCoffset[ch] = (uint32_t)((float)(fabs(( ( - offset )/ cal ) - 100.))*(655.35) - 65535*m_Baseline);
        if (m_DCoffset[ch] > 65535) m_DCoffset[ch] = 65535;
        if (m_DCoffset[ch] < 0) m_DCoffset[ch] = 0;		      
      }
      else if (m_PulsePolarity[ch] == CAEN_DGTZ_PulsePolarityNegative){
        //m_DCoffset[ch] = (uint32_t)((float)(fabs(( (fabs(dc_file[ch] - 100.) - offset) / cal ) - 100.))*(655.35));
        m_DCoffset[ch] = (uint32_t)((float)(fabs(( ( 100. - offset) / cal ) - 100.))*(655.35) + 65535*m_Baseline);
        if (m_DCoffset[ch] < 0) m_DCoffset[ch] = 0;
        if (m_DCoffset[ch] > 65535) m_DCoffset[ch] = 65535;		      
      }

      //cout<<"Calibrated DC offset (between 0 and 65535)"<<m_DCoffset[ch]<<", frac "<<m_DCoffset[ch]/65535.<<endl;

      m_ret = CAEN_DGTZ_SetChannelDCOffset(m_handle, (uint32_t)ch, m_DCoffset[ch]);
      if (m_ret)
        printf("Warning: error setting ch %d offset\n", ch);
      usleep(200000);
    }//if ch enabled

  }//loop ch

  //printf("DAC Calibration ready\n");

  CAEN_DGTZ_SWStopAcquisition(m_handle);  

  ///free events e buffer
  CAEN_DGTZ_FreeReadoutBuffer(&buffer);
  CAEN_DGTZ_FreeEvent(m_handle, (void**)&Event16);

  //TODO
  SetCorrectThreshold();

  //Restore previous settings
  CAEN_DGTZ_SetPostTriggerSize(m_handle, m_PostTrigger);
  CAEN_DGTZ_SetAcquisitionMode(m_handle, mem_mode);
  CAEN_DGTZ_SetExtTriggerInputMode(m_handle, m_ExtTriggerMode);
  CAEN_DGTZ_GetRecordLength(m_handle,&m_BufferSize);
  if (m_ret)
    printf("Warning: error setting recorded parameters\n");

  //TODO
  //CalibComplete=true;
  
  return CAEN_DGTZ_Success;
}

CAEN_DGTZ_ErrorCode ADCManager1730_16Ch::SetCorrectThreshold(){
//Threshold needs to be adjusted for DC offset  
  CAEN_DGTZ_UINT16_EVENT_t    *Event16;
  int i = 0,ch=0;
  CAEN_DGTZ_ErrorCode m_ret;
  uint32_t  AllocatedSize;
  
  uint32_t BufferSize;
  CAEN_DGTZ_EventInfo_t       EventInfo;
  char *buffer = NULL;
  char *EventPtr = NULL;
  
  ///malloc
  m_ret = CAEN_DGTZ_MallocReadoutBuffer(m_handle, &buffer, &AllocatedSize);
  if (m_ret) {
    return m_ret;
  }
  
  m_ret = CAEN_DGTZ_AllocateEvent(m_handle, (void**)&Event16);
  if (m_ret != CAEN_DGTZ_Success) {
    return m_ret;
  }
  
  uint32_t mask;
  CAEN_DGTZ_GetChannelEnableMask(m_handle, &mask);

  CAEN_DGTZ_SWStartAcquisition(m_handle);
  //For the 1730, channels are paired such that the trigger conditions are the same for channel N and channel N+1
  for (ch = 0; ch < (int32_t)BoardInfo.Channels; ch+=2){
    if (mask & (1 << ch)){
      int baseline = 0;
      CAEN_DGTZ_SendSWtrigger(m_handle);

      m_ret = CAEN_DGTZ_ReadData(m_handle, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT, buffer, &BufferSize);
      if (m_ret) {
        cout<<errors[abs((int)m_ret)]<<" (code "<<m_ret<<")"<<endl;
        exit(0);
      }

      m_ret = CAEN_DGTZ_GetEventInfo(m_handle, buffer, BufferSize, 0, &EventInfo, &EventPtr);
      if (m_ret) {
        cout<<errors[abs((int)m_ret)]<<" (code "<<m_ret<<")"<<endl;
        return m_ret;
      }
      // decode the event //
      m_ret = CAEN_DGTZ_DecodeEvent(m_handle, EventPtr, (void**)&Event16);

      if (m_ret) {
        cout<<errors[abs((int)m_ret)]<<" (code "<<m_ret<<")"<<endl;
        return m_ret;
      }


      for (i = 1; i < 11; i++){ //mean over 10 samples
        baseline += (int)(Event16->DataChannel[ch][i]);
      }
      baseline = (baseline / 10);

      if (m_PulsePolarity[ch] == CAEN_DGTZ_PulsePolarityPositive){
        m_channelThresh[ch] += (uint32_t)baseline;
      }
      else 	if (m_PulsePolarity[ch] == CAEN_DGTZ_PulsePolarityNegative){
        //cout<<"Baseline calc "<<baseline<<", thesh "<<m_channelThresh[ch]<<endl;
        m_channelThresh[ch] = (uint32_t)baseline - m_channelThresh[ch];
      }

      if (m_channelThresh[ch] < 0) m_channelThresh[ch] = 0;
      int size = (int)pow(2, (double)BoardInfo.ADC_NBits);
      if (m_channelThresh[ch] > (uint32_t)size) m_channelThresh[ch] = size;
      //cout<<"Calibrated threshold: "<<m_channelThresh[ch]<<endl;


      m_ret = CAEN_DGTZ_SetChannelTriggerThreshold(m_handle, ch, m_channelThresh[ch]);
      if (m_ret)
        printf("Warning: error setting ch %d corrected threshold\n", ch);

      //Do channel trigger is external is disabled
      if(m_SelfTriggerMode == CAEN_DGTZ_TRGMODE_ACQ_ONLY )
        CAEN_DGTZ_SetChannelSelfTrigger(m_handle, CAEN_DGTZ_TRGMODE_ACQ_ONLY, (1 << ch));
      else
        CAEN_DGTZ_SetChannelSelfTrigger(m_handle, CAEN_DGTZ_TRGMODE_DISABLED, (1 << ch));

    }
  }
  CAEN_DGTZ_SWStopAcquisition(m_handle);

  CAEN_DGTZ_FreeReadoutBuffer(&buffer);
  CAEN_DGTZ_FreeEvent(m_handle, (void**)&Event16);
  
  return CAEN_DGTZ_Success;
}

int ADCManager1730_16Ch::CheckEventBuffer(){
  //Do something
  startAcq();
  CAEN_DGTZ_ErrorCode m_ret = CAEN_DGTZ_Success;

  //Seems like if you put this in the header file, the code crashes...IDK why -N.M.
  CAEN_DGTZ_EventInfo_t       EventInfo;
  double startTimedebug = std::time(nullptr);
  bool Quit = false;
  int eventCounter = 0;
  while(!Quit){
    if(std::time(nullptr) - startTimedebug > 2) Quit = true;
    //TODO need to add in adcManager->GetTransferedBytes() <= 0, i.e. a non zero amount of data was transfered
    //
    if(m_SWTriggerMode == CAEN_DGTZ_TRGMODE_ACQ_ONLY){
      //usleep 
      usleep(GetSoftwareRate()); 
      CAEN_DGTZ_SendSWtrigger(m_handle);
    }
    /// Read data from the board ///
    m_BufferSize = 0;//Zero out BufferSize before reading data
    m_Ret = CAEN_DGTZ_ReadData(m_handle, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT, m_buffer, &m_BufferSize);
    if (m_Ret) {
      cout<<errors[abs((int)m_Ret)]<<" (code "<<m_Ret<<")"<<endl;
      exit(0);
    }
    uint32_t nEventsPerTrigger = 0;
    if (m_BufferSize != 0) {
      m_Ret = CAEN_DGTZ_GetNumEvents(m_handle,m_buffer, m_BufferSize, &nEventsPerTrigger);
      if (m_Ret) {
        cout<<errors[abs((int)m_Ret)]<<" (code "<<m_Ret<<")"<<endl;
        exit(0);
      }
    }
    else {
      uint32_t lstatus;
      m_Ret = CAEN_DGTZ_ReadRegister(m_handle, CAEN_DGTZ_ACQ_STATUS_ADD, &lstatus);
      if (m_Ret) {
        printf("Warning: Failure reading reg:%x (%d)\n", CAEN_DGTZ_ACQ_STATUS_ADD, m_Ret);
        cout<<errors[abs((int)m_Ret)]<<" (code "<<m_Ret<<")"<<endl;
      }
      else {
        if (lstatus & (0x1 << 19)) {
          exit(0);
        }
      }
    }
    //TODO add info about data, currently reported somewhere else
    
    // Analyze data //
    for(int i = 0; i < (int)nEventsPerTrigger; i++) {

      //// Get one event from the readout buffer //
      m_ret = CAEN_DGTZ_GetEventInfo(m_handle, m_buffer, m_BufferSize, i, &
          EventInfo, &m_EventPtr);
      if (m_ret) {
        cout<<errors[abs((int)m_ret)]<<" (code "<<m_ret<<")"<<endl;
        continue;
      }
      //TODO cast Event16 into a vector of Event16's
      m_ret = CAEN_DGTZ_DecodeEvent(m_handle, m_EventPtr, (void**)&Event16);
      if (m_ret) {
        cout<<errors[abs((int)m_ret)]<<" (code "<<m_ret<<")"<<endl;
        Quit = true;
      }
    }
    eventCounter+=nEventsPerTrigger;
    if(eventCounter >=m_NumEvents) Quit = true;
  }

  //TODO not sure if this is necessary
  m_ret = CAEN_DGTZ_SWStopAcquisition( m_handle);
  if (m_ret) {
    cout<<errors[abs((int)m_ret)]<<" (code "<<m_ret<<")"<<endl;
  }
  return 0;
}

bool ADCManager1730_16Ch::startAcq(){
      
  //if(!CalibComplete) Calibrate_DC_Offset();

  /* 
  Description:  This function starts the acquisition in a board using a software command. When the acquisition starts, the relevant
  RUN LED on the front panel lights up. It is worth noticing that in case of multiple board systems, the software start
  doesn’t allow the digitizer to start synchronously. For this purpose, it is necessary to use to start the acquisition using a
  physical signal, such as the S-IN or GPI as well as the TRG-IN-TRG-OUT Daisy chain. Please refer to Digitizer manual for
  more details on this issue.
  */
  //printf("Digitizer: Acquisition started\n");
  CAEN_DGTZ_SWStartAcquisition(m_handle);
  RunStartTime = util::markTime();
  //cout.precision(15);
  //cout<<"Digitizer: Start time: "<<RunStartTime<<endl;
  //fman.setRunStartTime(RunStartTime);
  
  //AcqRun = 1;
}

int ADCManager1730_16Ch::ApplyXMLFile(){
	
	int temp;  
	char txt[100];
	const char *xstr;
	txt[0]='\0';
	
	// open the XML file -------------------------------------------------------
	XMLNode xMainNode=XMLNode::openFileHelper(m_XmlFileName,"settings");
	// parse global DAQ settings -----------------------------------------------
	XMLNode xNode=xMainNode.getChildNode("global");
	// parse global ADC settings -----------------------------------------------
	xNode=xMainNode.getChildNode("adc").getChildNode("global");
	
	xstr=xNode.getChildNode("nb_chs").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		m_nbCh=(int)(atoi(txt));
	} else error((char*)"XML-nb_chs");
	
  xstr=xNode.getChildNode("eventsTransferred").getText();
  if (xstr){
    strcpy(txt,xstr); 
		m_NumEvents=(int)(atoi(txt));
  }else{
    cout<<"Setting numbers of events transferred per read to 1. Note that the greater the number of events transferred, the greater the efficiency of the readout, since the protocol overhead is smaller."<<endl;
    m_NumEvents = 1;  
  }
	m_hex=0x0;
	m_channelThresh = new int[m_nbCh];
  m_trigPolarity = new CAEN_DGTZ_TriggerPolarity_t[m_nbCh];
  m_PulsePolarity = new CAEN_DGTZ_PulsePolarity_t[m_nbCh];
  for(int i=0;i<m_nbCh;i++){
    char channel[300];
    sprintf(channel,"ch_%i",i+m_module*m_nbCh);
    xstr=xNode.getChildNode(channel).getText();
    if (xstr) {
      strcpy(txt,xstr);
      temp=atoi(txt);
      m_channelThresh[i]=temp;
      if(temp!=0){
        m_hex=m_hex+pow(2,i);
        //adc_writereg(ThresholdRegN+(i*0x0100),temp);

        m_EnableMask += (1<<i);
      }
    } else error((char*)channel);
    
    char polarity[300];
    sprintf(polarity,"polarity_%i",i+m_module*m_nbCh);
    xstr=xNode.getChildNode(polarity).getText();
    strcpy(txt,xstr);
    temp=atoi(txt);
    if(temp == 1){
      m_PulsePolarity[i] = CAEN_DGTZ_PulsePolarityPositive;
      m_trigPolarity[i]  = CAEN_DGTZ_TriggerOnRisingEdge;
    }
    else if(temp == -1){
      m_PulsePolarity[i] = CAEN_DGTZ_PulsePolarityNegative;
      m_trigPolarity[i]  = CAEN_DGTZ_TriggerOnFallingEdge;
    }
    else 
      cout<<"Error::wrong signal polarity of "<<temp<<endl;
  }
  cout<<"EnableMask is "<<m_EnableMask<<endl;

	

  /*
  //TODO what does this do? Seems like it is not needed here but when removed from the xml, things crash
  //seems like I do not need it
	xstr=xNode.getChildNode("memoryorganisation").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		switch (atoi(txt)) {
			case 512: m_hex=0x0; break;
			case 256: m_hex=0x1; break;
			case 128: m_hex=0x2; break;
			case  64: m_hex=0x3; break;
			case  32: m_hex=0x4; break;
			case  16: m_hex=0x5; break;
			case   8: m_hex=0x6; break;
			case   4: m_hex=0x7; break;
			case   2: m_hex=0x8; break;
			case   1: m_hex=0x9; break;
			case   0: m_hex=0xA; break;  // 0.5 k
			default:  m_hex=0x4; 
					  break; 
		}
	  adc_writereg(BlockOrganizationReg,m_hex);
	} else error((char*)"XML-memoryorganisation");
  */
	
  xstr=xNode.getChildNode("custom_size").getText();
	if (xstr) {
        strcpy(txt,xstr);
	      m_RecordLength = (int)(atoi(txt)); 
	} else error((char*)"XML-custom_size");

	xstr=xNode.getChildNode("posttrigger").getText();
	if (xstr) {
		strcpy(txt,xstr); 
    m_PostTrigger=((int)atoi(txt));
		temp=((int)atoi(txt))/8;
		m_posttrigger=temp;
	} else error((char*)"XML-posttrigger");
		

	xstr=xNode.getChildNode("baseline").getText();
  if (xstr) {
    strcpy(txt,xstr);
    m_Baseline=(int)atoi(txt);
    m_Baseline /= 100;
    if(m_Baseline > 1 || !m_Baseline){
      cout<<"Bad baseline value "<< 100*m_Baseline<<"...Setting baseline such that 95% of waveform is above baseline (assuming uni-polar signals)"<<endl;
      m_Baseline = .95;
    }
    else{
      if(100*m_Baseline < 50) cout<<"Warning::Baseline might be set to low, try to always set it above 50% (more than 50% of the expected waveform is above the baseline"<<endl;
      cout<<"Baseline value set to "<<100*m_Baseline<<"% of waveform is above baseline (assuming uni-polar signals)"<<endl;
    }

  } else error((char*)"XML-baseline");

/*
	xstr=xNode.getChildNode("baselineiteration").getText();
	if (xstr) {
		strcpy(txt,xstr);
		m_iteration=(int)atoi(txt); 
	} else error((char*)"XML-baselineiteration");
	
*/
	xstr=xNode.getChildNode("sampling_freq").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		m_Frequency=(int)atoi(txt); 
	} else error((char*)"XML-sampling_freq");


	//TODO, does not work
  /*
	xstr=xNode.getChildNode("voltage_range").getText();
	if (xstr) {
		strcpy(txt,xstr);
    m_Voltage = stod(std::string(txt) );
    if(m_Voltage == 2){
      //Input Range 0-2V!
      for(int i=0;i<m_nbCh;i++)adc_writereg(GainRegN+(i*0x0100),0);
    }
    else if(m_Voltage == 0.5){
      //Input Range 0-0.5V! //TODO double check that this works
      for(int i=0;i<m_nbCh;i++)adc_writereg(GainRegN+(i*0x0100),1);
    }
    else error((char*)"Wrong value for voltage range");
	} else error((char*)"voltage_range");
  */
	
	xstr=xNode.getChildNode("sample_size_ADC").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		m_resADC=(int)atoi(txt); 
	} else error((char*)"sample_size_ADC");
	
	xstr=xNode.getChildNode("sample_size_DAC").getText();
	if (xstr) {
		strcpy(txt,xstr); 
		m_resDAC=(int)atoi(txt); 
	} else error((char*)"sample_size_DAC");
  
	//TODO rewrite in terms of Digitzer functions! -N.M.
  //ADC: parse ADC Trigger Settings
	xNode=xMainNode.getChildNode("adc").getChildNode("triggerSettings");


  /*
	xstr=xNode.getChildNode("trigger").getText();
  if (xstr) {
    strcpy(txt,xstr); 
    temp=atoi(txt);
    m_triggertyp=temp;
    if (temp==0) {
      //External + Software Trigger always activated 
      m_hex=0xC0000000;
      adc_writereg(FrontPanelTriggerOutReg,m_hex);
      adc_writereg(TriggerSourceMaskReg,m_hex);
    }
    else if (temp==1){ 
      //External + Software Trigger always activated 
      m_hex=0xC0000000;
      adc_writereg(FrontPanelTriggerOutReg,m_hex);	
      adc_writereg(TriggerSourceMaskReg,m_hex);

      xstr=xNode.getChildNode("SoftwareRate").getText();;
      if (xstr) {
        strcpy(txt,xstr); 
        m_SoftwareRate=atoi(txt);
      }
      else error((char*)"SoftwareRate");
    }
    //Channel Trigger
    else if(temp=2){
      m_hex=0;
      uint32_t m_dat=0;

      for(int i=0;i<4;i++){
        char logic[300];
        sprintf(logic,"logic%i",i+4*m_module);
        xstr=xNode.getChildNode(logic).getText();

        if (xstr) {
          strcpy(txt,xstr); 
          if(atoi(txt)>=0){
            m_hex=m_hex+pow(2,i);
            m_dat=m_dat+pow(2,i);
            m_dat=m_dat+pow(2,i+1);
            if(atoi(txt)==1){
              adc_writereg(LogicRegN+(i*0x200),5);
            }
            else if(atoi(txt)==0){
              adc_writereg(LogicRegN+(i*0x200),7);
            }
            else if(atoi(txt)==2){
              adc_writereg(LogicRegN+(i*0x200),6);
            }
            else if(atoi(txt)==3){
              adc_writereg(LogicRegN+(i*0x200),4);
            }	
          }
        }
        else error((char*)logic);
      }

      //Software + Channel Trigger by default 
      m_hex=m_hex+pow(2,31);
      m_dat=m_dat+pow(2,31);
      adc_writereg(FrontPanelTriggerOutReg,m_dat);
      adc_writereg(TriggerSourceMaskReg,m_hex);
    }
    //Coincidence Trigger
    else{
      m_hex=0;
      uint32_t m_dat=0;

      for(int i=0;i<4;i++){
        char logic[300];
        sprintf(logic,"logic%i",i+4*m_module);
        xstr=xNode.getChildNode(logic).getText();

        if (xstr) {
          strcpy(txt,xstr); 
          if(atoi(txt)>=0){
            m_hex=m_hex+pow(2,i);
            m_dat=m_dat+pow(2,i);
            m_dat=m_dat+pow(2,i+1);
          }
          //Software + Channel Trigger by default 
          m_hex=m_hex+pow(2,31);

          m_hex=m_hex+pow(2,24);							//Means more than 1 channel above treshold

          adc_writereg(FrontPanelTriggerOutReg,m_dat);
          adc_writereg(TriggerSourceMaskReg,m_hex);
        }
        else error((char*)logic);
      }
    }
  }
  else error((char*)"XML-trigger");
  */
  m_ExtTriggerMode = CAEN_DGTZ_TRGMODE_DISABLED;
  m_SWTriggerMode  = CAEN_DGTZ_TRGMODE_DISABLED;
  m_SelfTriggerMode = CAEN_DGTZ_TRGMODE_DISABLED;
  xstr=xNode.getChildNode("trigger").getText();
  if (xstr) {
    strcpy(txt,xstr); 
    temp=atoi(txt);
    if (temp==0) {
      m_ExtTriggerMode = CAEN_DGTZ_TRGMODE_ACQ_ONLY; 
    }
    else if (temp == 1){
      m_SWTriggerMode = CAEN_DGTZ_TRGMODE_ACQ_ONLY;
    }
    else if (temp == 2){
      m_SelfTriggerMode =  CAEN_DGTZ_TRGMODE_ACQ_ONLY;
    }
  }
  
  xstr=xNode.getChildNode("SoftwareRate").getText();;
  if (xstr) {
    strcpy(txt,xstr); 
    m_SoftwareRate=atoi(txt);
  }
  else error((char*)"SoftwareRate");   
    
  xstr=xNode.getChildNode("TTL").getText();
  if (xstr) {
    strcpy(txt,xstr);
    temp=(int)(atoi(txt));
    if(temp==1){
      //adc_writereg(FrontPanelIODataReg,m_hex);
      m_TriggerLogicType = CAEN_DGTZ_IOLevel_TTL;
		}
		else{
      //adc_writereg(FrontPanelIODataReg,m_hex);
      m_TriggerLogicType = CAEN_DGTZ_IOLevel_NIM;
		}
	} else error((char*)"ADC-Manager-XML-TTL");

  

	return 0;
}




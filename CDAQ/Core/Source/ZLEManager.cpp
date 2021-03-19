#include <ZLEManager.h>

/*
** 
** Attempt to add ZLE for 1730 16 Channel
**Code adapted from DPP‐ZLEplus demo software www.caen.it/download/?filter=DPP-ZLEplus
**
** */

ZLEManager::ZLEManager(){

}

int ZLEManager::XX2530_ZLE_SetPreTrigger(int handle, uint32_t samples, int channel) {
	int ret;
  uint32_t regvalue;
	CAEN_DGTZ_ReadRegister(handle, 0x1038 | (channel << 8), &regvalue);
	if (samples < 256) {
		ret = CAEN_DGTZ_WriteRegister(handle, 0x1038 | (channel << 8), samples);
		//if (DEBUG) printf("pretrigger sample number of channel %d: %d\n", channel,samples);
		printf("pretrigger sample number of channel %d: %d\n", channel,samples);
	} else {
		printf("invalid value for pretrigger sample number of channel %d\n", channel);
	}
	cout<<"\t value before "<<regvalue;
  CAEN_DGTZ_ReadRegister(handle, 0x1038 | (channel << 8), &regvalue);
  cout<<", value after "<<regvalue<<endl;
	return ret;
}

int ZLEManager::XX2530_ZLE_SetBLineMode(int handle,uint32_t mode,int channel) {
	int ret;
	uint32_t regvalue;
	ret = CAEN_DGTZ_ReadRegister(handle, 0x1034 | (channel << 8), &regvalue);
	if (mode) ret |= CAEN_DGTZ_WriteRegister(handle, 0x1034 | (channel << 8), regvalue | (uint32_t)(1 << 24));
	else ret |= CAEN_DGTZ_WriteRegister(handle, 0x1034 | (channel << 8), (regvalue & (~(uint32_t)(1 << 24))));
  printf("baseline mode set to %d for channel %d: \n", mode,channel);
  cout<<"\t value before "<<regvalue;
  CAEN_DGTZ_ReadRegister(handle, 0x1034 | (channel << 8), &regvalue);
  cout<<", value after "<<regvalue<<endl;
	
  return ret;
}

int ZLEManager::XX2530_ZLE_SetBLineDefValue(int handle, uint32_t bl, int channel) {
	int ret;
	uint32_t regvalue;
	if (bl < 16384) {
		ret = CAEN_DGTZ_ReadRegister(handle, 0x1034 | (channel << 8), &regvalue);
		regvalue = (regvalue & (uint32_t)(~(0x00003fff))) | (uint32_t)(bl & 0x3fff); // replace only the two bits affecting the selected couple's logic.
		ret |= CAEN_DGTZ_WriteRegister(handle, 0x1034 | (channel << 8), regvalue);
	} else {
		printf("invalid value for default baseline of channel %d\n", channel);
	}
 	cout<<"\t value before "<<regvalue;
  CAEN_DGTZ_ReadRegister(handle, 0x1034 | (channel << 8), &regvalue);
  cout<<", value after "<<regvalue<<endl;
	return ret;
}

int ZLEManager::XX2530_ZLE_SetBLineNoise(int handle, uint32_t noise, int channel) {
	int ret;
	uint32_t regvalue;
	if (noise < 256) {
		ret = CAEN_DGTZ_ReadRegister(handle, 0x1034 | (channel << 8), &regvalue);
		regvalue = (regvalue & (uint32_t)(~(0x00ff0000))) | (uint32_t)(noise << 16); // replace only the two bits affecting the selected couple's logic.
		ret |= CAEN_DGTZ_WriteRegister(handle, 0x1034 | (channel << 8), regvalue);
    printf("baseline noise set to %d for channel %d: \n", noise,channel);
	} else {
		printf("invalid value for baseline noise of channel %d\n", channel);
	}

  cout<<"\t value before "<<regvalue;
  ret = CAEN_DGTZ_ReadRegister(handle, 0x1034 | (channel << 8), &regvalue);
  cout<<", value after "<<regvalue<<endl;

	return ret;
}

int ZLEManager::XX2530_ZLE_SetPreSamples(int handle,uint32_t samples, int channel) {
	int ret;
  uint32_t regvalue;
	ret = CAEN_DGTZ_ReadRegister(handle, 0x1054 | (channel << 8), &regvalue);
	if (samples < 256) {
		ret = CAEN_DGTZ_WriteRegister(handle, 0x1054 | (channel << 8),samples);
		//if (DEBUG) printf("pre-signal sample number of channel %d: %d\n", channel,samples);
		printf("ZLE_SetPreSamples::pre-signal sample number of channel %d: %d\n", channel,samples);
	} else {
		printf("invalid value for pre-signal sample number of channel %d\n",channel);
	}

  cout<<"\t value before "<<regvalue;
  ret = CAEN_DGTZ_ReadRegister(handle, 0x1054 | (channel << 8), &regvalue);
  cout<<", value after "<<regvalue<<endl;

	return ret;
}

int ZLEManager::XX2530_ZLE_SetPostSamples(int handle, uint32_t samples, int channel) {
	int ret;

  uint32_t regvalue;
	ret = CAEN_DGTZ_ReadRegister(handle, 0x1058 | (channel << 8), &regvalue);
	if (samples < 256) {
		ret = CAEN_DGTZ_WriteRegister(handle, 0x1058 | (channel << 8),samples);
		//if (DEBUG) printf("post-signal sample number of channel %d: %d\n", channel,samples);
		printf("ZLE_SetPostSamples::post-signal sample number of channel %d: %d\n", channel,samples);
	} else {
		printf("invalid value for post-signal sample number of channel %d\n",channel);
	}

  cout<<"\t value before "<<regvalue;
  ret = CAEN_DGTZ_ReadRegister(handle, 0x1058 | (channel << 8), &regvalue);
  cout<<", value after "<<regvalue<<endl;
	
  return ret;
}

int ZLEManager::XX2530_ZLE_SetDataThreshold(int handle, uint16_t threshold, int channel) {
	int ret;
  
  uint32_t regvalue;
	ret = CAEN_DGTZ_ReadRegister(handle, 0x105C | (channel << 8), &regvalue);

  if (threshold < 16384) {
		ret = CAEN_DGTZ_WriteRegister(handle, 0x105C | (channel << 8), (uint32_t)(threshold & 0x3FFF));
		//if (DEBUG) printf("data threshold of channel %d: %d\n", channel,threshold);
		printf("ZLE_SetDataThreshold::data threshold of channel %d: %d\n", channel,threshold);
	} else {
		printf("invalid value for data threshold of channel %d\n", channel);
	}

  cout<<"\t value before "<<regvalue;
  ret = CAEN_DGTZ_ReadRegister(handle, 0x105C | (channel << 8), &regvalue);
  cout<<", value after "<<regvalue<<endl;
	
  return ret;
}

int ZLEManager::XX2530_ZLE_SetTriggerThreshold(int handle, uint16_t threshold, int channel) {
	int ret;

  uint32_t regvalue;
  ret = CAEN_DGTZ_ReadRegister(handle, 0x1060 | (channel << 8), &regvalue);

	if (threshold < 16384) {
		ret = CAEN_DGTZ_WriteRegister(handle, 0x1060 | (channel << 8), (uint32_t)(threshold & 0x3FFF));
		//if (DEBUG) printf("trigger threshold of channel %d: %d\n",channel, threshold);
		printf("trigger threshold of channel %d: %d\n",channel, threshold);
	} else {
		printf("invalid value for trigger threshold of channel %d\n", channel);
	}

  cout<<"\t value before "<<regvalue;
  ret = CAEN_DGTZ_ReadRegister(handle, 0x1060 | (channel << 8), &regvalue);
  cout<<", value after "<<regvalue<<endl;

	return ret;
}

int ZLEManager::XX2530_ZLE_SetPulsePolarity(int handle, uint32_t polarity, int channel) {
	int ret;
	uint32_t regvalue;
	ret = CAEN_DGTZ_ReadRegister(handle, 0x1064 | (channel << 8), &regvalue);
	(polarity) ? (regvalue = regvalue | 0x00000100) : (regvalue = regvalue & ~(0x00000100));
	ret = CAEN_DGTZ_WriteRegister(handle, 0x1064 | (channel << 8), regvalue);
	return ret;
}

int ZLEManager::XX2530_ZLE_NoThresholdEnable(int handle, int channel) {
	int ret;
	uint32_t regvalue;
	ret = CAEN_DGTZ_ReadRegister(handle, 0x1064 | (channel << 8), &regvalue);
	ret = CAEN_DGTZ_WriteRegister(handle, 0x1064 | (channel << 8), regvalue | (uint32_t)(0x80));
	return ret;
}

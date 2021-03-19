#ifndef _ZLEManager_H_
#define _ZLEManager_H_

#include <CAENDigitizer.h>
#include <CAENDigitizerType.h>
#include <time.h>
#include <sys/timeb.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <math.h>

using namespace std;

class ZLEManager
{

  public:
    ZLEManager();
    ~ZLEManager();

    int XX2530_ZLE_SetPreTrigger(int handle, uint32_t samples, int channel);
    int XX2530_ZLE_SetBLineMode(int handle,uint32_t mode,int channel);
    int XX2530_ZLE_SetBLineDefValue(int handle, uint32_t bl, int channel);
    int XX2530_ZLE_SetBLineNoise(int handle, uint32_t noise, int channel);
    int XX2530_ZLE_SetPreSamples(int handle,uint32_t samples, int channel);
    int XX2530_ZLE_SetPostSamples(int handle, uint32_t samples, int channel);
    int XX2530_ZLE_SetDataThreshold(int handle, uint16_t threshold, int channel);
    int XX2530_ZLE_SetTriggerThreshold(int handle, uint16_t threshold, int channel);
    int XX2530_ZLE_SetPulsePolarity(int handle, uint32_t polarity, int channel);
    int XX2530_ZLE_NoThresholdEnable(int handle, int channel);
  private:
};
#endif

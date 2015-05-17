#ifndef _SCALER_H_
#define _SCALER_H_

#include <string.h>
#include <time.h>
#include "CAENVMElib.h"

using namespace std;

class scaler
{
 public:
   scaler();
   virtual ~scaler();
   
   //All functions 0=success negative number=fail
   int       ReadChannel(int channel);
   int       Reset();
   int       ReadConfigFile(string filename); 
   int       Test();
   int	     ReadFW();
   string    GetTimeString();
   int       Close();
 private:
   u_int32_t StringToInt(const string &str);
   u_int32_t StringToHex(const string &str);
   u_int32_t fVMEAddressController;
   u_int32_t fVMEAddressScaler;
   int fCrateHandle;
   u_int32_t fChannelMask;
   u_int32_t fLink;
};

#endif

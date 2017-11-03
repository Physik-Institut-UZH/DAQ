#ifndef COMMON_H
#define COMMON_H

#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h> 
#include <string.h>
#include <fstream> 
#include "global.h"
#include <boost/filesystem.hpp>


/*Author: Julien Wulf UZH
  General functions and stuff for every Module. The function name describes it very good.
*/

using namespace std;
class Common
{
  public:
	  Common();   
	  virtual ~Common();
	  int32_t get_time();
	  int GetUnixTime();
	  void gettimestring(char* buffer);
	  int   error(char *node);
	  static boost::filesystem::path getdotdaqdir();
};

#endif

#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h> 
#include <string.h>
#include <fstream> 
#include "CAENVMElib.h"
#include "common.h"
#include <sys/time.h>
#include <cstdlib>

/*
Author: Julien Wulf UZH
*/

Common::Common()
{

}

Common::~Common()
{
}

void Common::gettimestring(char* buffer)
{
  time_t curtime;
  struct tm *loctime;
  curtime = time (NULL);                                // Get the current time.
  loctime = localtime (&curtime);                        // Convert it to local time representation.
  strftime(buffer, 100, "%y%m%d_%H%M", loctime);       // formatted print for filenames

}

// returns time in milliseconds
int32_t Common::get_time()
{
    int32_t time_ms;

    struct timeval t1;
    struct timezone tz;

    gettimeofday(&t1, &tz);
    time_ms = (t1.tv_sec) * 1000 + t1.tv_usec / 1000;
    return time_ms;
}

// returns time since start of UNIX
int Common::GetUnixTime(){

    time_t current_time;
    current_time = time(NULL);
    return current_time;
}

int Common::error(char *node)
{
  printf(KRED);
  printf("Error: (%s)\n",node);
  printf(RESET);
  return 0;
}

// Return the location of the .daq dir (for now just ~/.daq
std::string Common::getdotdaqdir()
{
  std::string p(std::getenv("HOME"));
  p.append("/.daq");
  return p;
}


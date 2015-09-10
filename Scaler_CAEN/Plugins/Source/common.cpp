#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h> 
#include <string.h>
#include <fstream> 
#include "CAENVMElib.h"
#include "common.h"
#include <sys/time.h>

Common::Common()
{

}

Common::~Common()
{
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



#include <iostream>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctime>
#include <cmath>
#include <time.h>
#include <stdlib.h>
#include <stdio.h> 
#include <sys/stat.h>
#include "common.h"
#include "CAENVMElib.h"	// CAEN VME library

using namespace std;

Common::Common(){
}

string Common::GetTime(){

	time_t current_time;
	current_time = time(NULL);
	buffer = ctime(&current_time);
        t = std::string(buffer);
	return t;
}

int Common::GetUnixTime(){

	time_t current_time;
	current_time = time(NULL);
	return current_time;
}

string Common::GetDayYear(){
	
	time_t rawtime;
	struct tm * timeinfo;
	char buffer [80];
	time (&rawtime);
	timeinfo = localtime (&rawtime);
	strftime(buffer,80,"%H",timeinfo);
	t = std::string(buffer);
	return t;
}

string Common::GetDay(){
	
	time_t rawtime;
	struct tm * timeinfo;
	char buffer [80];
	time (&rawtime);
	timeinfo = localtime (&rawtime);
	strftime(buffer,80,"%F",timeinfo);
	t = std::string(buffer);
	return t;
}




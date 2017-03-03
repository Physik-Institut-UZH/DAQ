#!/usr/bin/python
# -*- coding: utf-8 -*-
#############################################################################
# FILENAME
# DATUM
# Written by Julien W.
# Descrpition
#############################################################################

import os
import time
import datetime
import serial
import sys
from optparse import OptionParser

#############################################################################
# Initialization 
#############################################################################

def isopen(s):
    print "Port open" if pulser.isOpen() else "Port not open"



# Constants
DAQdir  = "/home/sandbox/DAQ_dev/DAQ/CDAQ/bin/"
binary  = "/home/sandbox/DAQ_dev/DAQ/CDAQ/bin/SingleDAQ"
xmlfile = "/home/sandbox/DAQ_dev/DAQ/CDAQ/bin/SingleADC1730_Chris.xml"
baselinefile = "/home/sandbox/DAQ_dev/DAQ/CDAQ/bin/SingleADC1730_Chris.xml"
mapfile = "map"

serialport = "/dev/ttyACM0"        # Pulser


#############################################################################
# Read in map File
#############################################################################
FileNames=[]
LEDSetting = []

print "Read MapFile ... "

with open(mapfile) as thefile:
         for line in thefile:
            content = line.split()
            FileNames.append(content[0])
            LEDSetting.append(content[1])

#############################################################################
# Open serial ports
#############################################################################

print "Establish connection to the Pulser"
pulser = serial.Serial(serialport, baudrate=9600) 


#############################################################################
# Starting measurement
#############################################################################
#cmd3 = binary + " -b -x " + baselinefile

try:
   for i,j in zip(FileNames,LEDSetting):
	pulser.write("CHN 1" + '\r\n')
	time.sleep(5)		
	pulser.write("AMPL " + j + '\r\n')
        time.sleep(5)

        cmd2 = binary + " -x " + xmlfile + " -f " + i
        os.system(cmd2)


except KeyboardInterrupt:
    print "Got Ctrl-c"
    # Close port
    if pulser.isOpen():
        print "closing port"
        pulser.close()
    print "exiting ..."
    sys.exit(0)


#############################################################################
# Exiting
#############################################################################

isopen(pulser)
print "close port and exit ..."
pulser.close()
isopen(pulser)

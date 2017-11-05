#!/usr/bin/python
# -*- coding: utf-8 -*-
#############################################################################
# FILENAME
# DATUM
# Written by Sandro D'Amato with the friendly support of Gaudenz Kessler
# Descrpition
#############################################################################

import os
import time
import serial
import sys

#############################################################################
# Initialization 
#############################################################################

def isopen(s):
    print "Port open" if ser.isOpen() else "Port not open"



# Constants

serialport = "/dev/ttyACM1"



#############################################################################
# Open serial point
#############################################################################
#ser = serial.Serial(args.serialport, baudrate=9600) # 2.7
print "opening serial port"
ser = serial.Serial(serialport, baudrate=9600) # uncool


#############################################################################
# Starting measurement
#############################################################################
time.sleep(5)
ser.write("CHN 1" + '\r\n')
time.sleep(5)
ser.write("AMPL 2.650" + '\r\n')
time.sleep(5)
ser.write("OUTPUT ON" + '\r\n')
time.sleep(5)


isopen(ser)
print "close port and exit ..."
ser.close()
isopen(ser)
sys.exit(0)

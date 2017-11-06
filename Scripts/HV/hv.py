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

serialport = "/dev/ttyACM0"



#############################################################################
# Open serial point
#############################################################################
#ser = serial.Serial(args.serialport, baudrate=9600) # 2.7
print "opening serial port"
ser = serial.Serial(serialport, baudrate=9600)


#############################################################################
# Starting measurement
#############################################################################

try:



    #with open(args.tfile, "w") as tfile: # For python 2.7
    while True:

        s = raw_input("type Command: ")

        #print "Sending:  >>",
	isopen(ser)
        ser.write(s + '\r')
	out = ''

	# let's wait one second before reading output (let's give device time to answer)
	time.sleep(1)
	while ser.inWaiting() > 0:
		out += ser.read(1)

	if out != '':
		print ">>" + out


except KeyboardInterrupt:
    print "Got Ctrl-c"
    if ser.isOpen():
        print "closing port"
        ser.close()
    print "exiting ..."
    sys.exit(0)


#############################################################################
# Exiting
#############################################################################

isopen(ser)
print "close port and exit ..."
ser.close()
isopen(ser)

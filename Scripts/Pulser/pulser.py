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
ser = serial.Serial(serialport, baudrate=4800) # uncool


#############################################################################
# Starting measurement
#############################################################################
#time.sleep(5)

try:



    #with open(args.tfile, "w") as tfile: # For python 2.7
    while True:
        s = raw_input("type Command: ")
	print(s)
        # send position information to SandBox controller
        #print "Sending:  >>",
	isopen(ser)
        ser.write(s + '\r\n')
	out = ''
	# let's wait one second before reading output (let's give device time to answer)
	time.sleep(1)
	while ser.inWaiting() > 0:
		out += ser.read(1)
		if out != '':
			print ">>" + out



except KeyboardInterrupt:
    print "Got Ctrl-c"
    # Reset and go to zero position
    #s = "0000000000"
    #for c in s:
    #    ser.write(c)
    #time.sleep(1)
    # Close port
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

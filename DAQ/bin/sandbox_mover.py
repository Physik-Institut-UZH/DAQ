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
xmax = 13000
ymax = 14500

mm_steps = 64.12311

mapfile = "map"
serialport = "/dev/ttyACM0"


# Datafile for temperature
tfilename = "./temperature.dat"

#############################################################################
# Open serial point
#############################################################################
#ser = serial.Serial(args.serialport, baudrate=9600) # 2.7
print "opening serial port"
ser = serial.Serial(serialport, baudrate=9600) # uncool

# Read the first 4 values from the USB port
print "read out some lines"
for i in range(4):
    print ser.readline(),

#############################################################################
# Starting measurement
#############################################################################
#time.sleep(5)

try:
    # Drive to zero position
    print "Go to zero position"
    s = "0000000000"
    for c in s:
        ser.write(c)
    #time.sleep(60)


    #with open(args.tfile, "w") as tfile: # For python 2.7
    while True:
        s = raw_input("type position (10 digs): ")
        if len(s) != 10 or int(s[0:5]) > xmax or int(s[5:]) > ymax:
            print "wrong formal"
            continue
        # send position information to SandBox controller
        #print "Sending:  >>",
        for c in s:
            #print c,
            ser.write(c)
        #print "<<"
        isopen(ser)

        time.sleep(3)


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

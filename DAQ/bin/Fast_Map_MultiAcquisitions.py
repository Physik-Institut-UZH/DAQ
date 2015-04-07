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
import datetime
import serial
import sys
from optparse import OptionParser

#############################################################################
# Initialization 
#############################################################################

def isopen(s):
    print "Port open" if ser.isOpen() else "Port not open"


# Defining paths
DAQdir  = "/home/sandbox/DAQ_2.0/DAQ/bin/"
binary  = "/home/sandbox/DAQ_2.0/DAQ/bin/Sandaq"
xmlfile = "/home/sandbox/DAQ_2.0/DAQ/bin/sandbox.xml"
baselinefile = "/home/sandbox/DAQ_2.0/DAQ/bin/set_baseline.xml"

# Constants
xmax = 13000
ymax = 14500

datafolder = 0
mapfile = "map"
serialport = "/dev/ttyACM1"


# Datafile for temperature
tfilename = "./temperature.dat"


#############################################################################
#  Parser
#############################################################################
### For version 2.7
#parser = argparse.ArgumentParser()
#parser.add_argument('-m', '--mapfile', dest='mapfile', type=str,
#                    default=mapfile, help="path of the mapfile")
#parser.add_argument('-t', '--tfile', dest='tfile', type=str,
#                    default=tfilename,
#                    help="path of the output file for the temperature")
#parser.add_argument('-x', '--execute', dest='xmlfile', type=str,
#                    default=xmlfile,
#                    help="XML file for the DAQ")
#parser.add_argument('-b', '--binary', dest='binaryfile', type=str,
#                    default=binary,
#                    help="binary file for the DAQ")
#parser.add_argument('-p', '--port', dest='serialport', type=str,
#                    default=serialport,
#                    help="serial port for SandBox controller")
#args = parser.parse_args()
#
## checking if files exist
## xml file
#if os.path.isfile(args.xmlfile):
#    xmlfile = args.xmlfile
#else:
#    print "Given XMLFile does not exist!"
#    sys.exit(0)
#
## mapfile
#if os.path.isfile(args.mapfile):
#    mapfile = args.mapfile
#else:
#    print "Given mapfile does not exist!"
#    sys.exit(0)
#
## binary file
#if os.path.isfile(args.binaryfile):
#    binary = args.binaryfile
#else:
#    print "Given binary file does not exist!"
#    sys.exit(0)



# for the python version 2.6.2
# Which came out in the year 2010 which is fucking long time ago!!
parser = OptionParser()
parser.add_option('-m', '--mapfile', dest='mapfile', type=str,
                    default=mapfile, help="path of the mapfile")
parser.add_option('-t', '--tfile', dest='tfile', type=str,
                    default=tfilename,
                    help="path of the output file for the temperature")
parser.add_option('-x', '--execute', dest='xmlfile', type=str,
                    default=xmlfile,
                    help="XML file for the DAQ")
parser.add_option('-b', '--binary', dest='binaryfile', type=str,
                    default=binary,
                    help="binary file for the DAQ")
parser.add_option('-p', '--port', dest='serialport', type=str,
                    default=serialport,
                    help="serial port for SandBox controller")
options, args = parser.parse_args()

# checking if files exist
# xml file
if os.path.isfile(options.xmlfile):
    xmlfile = options.xmlfile
else:
    print "Given XMLFile does not exist!"
    sys.exit(0)

# mapfile
if os.path.isfile(options.mapfile):
    mapfile = options.mapfile
else:
    print "Given mapfile does not exist!"
    sys.exit(0)

# binary file
if os.path.isfile(options.binaryfile):
    binary = options.binaryfile
else:
    print "Given binary file does not exist!"
    sys.exit(0)

## Basic setting
print "parsing xml file"
import xml.etree.ElementTree as ET

tree = ET.parse(xmlfile)
root = tree.getroot()

if(root[0][2].tag != "path"):   # Not so cute
    print "Can't find path in the xml file"
    print "Please locate data yourself"
    sys.exit(0)

datapath = root[0][2].text[:-1]

if not os.path.isdir(datapath):
    print "Have to create", datapath
    os.system("mkdir " + datapath)


#############################################################################
# Open serial point
#############################################################################
#ser = serial.Serial(args.serialport, baudrate=9600) # 2.7
ser = serial.Serial(options.serialport, baudrate=9600) # uncool

# Read the first 4 values from the USB port
for i in range(4):
    print ser.readline(),

#############################################################################
# Read in measurement points
#############################################################################

# empty list of setpoints
setpoints = []

IsMapsFileCorrect = 1

print "Checking map file ... "
with open(mapfile) as thefile:
    for i, row in enumerate(thefile):
        position = row.rstrip().lstrip() # Remove whitespaces
        # does the string have the right length (exactly 10 characters)
        if len(position) != 10:
            print "ERROR: position command wrong format" 
            print "String needs to have exactly 10 characters"
            print "{0:d}: {1:s}".format(i+1, position)
            IsMapsFileCorrect = 0
        # does the string have only numbers
        try:
            int(position)
        except:
            print "ERROR: position command wrong format" 
            print "Input not a number"
            print "{0:d}: {1:s}".format(i+1, position)
            IsMapsFileCorrect = 0
        # Check for maximum values
        if int(position[0:5]) > xmax or int(position[5:]) > ymax:
            print "ERROR: position out of range" 
            print "{0:d}: {1:s}".format(i+1, position)
            IsMapsFileCorrect = 0

        setpoints.append(position)

if not IsMapsFileCorrect:
    print "Map file not correct, please check!!!"
    print "Exiting ..."
    sys.exit(0)

#############################################################################
# Starting measurement
#############################################################################
time.sleep(5)

try:
    # Drive to zero position
    print "Go to zero position"
    s = "0000000000"
    for c in s:
        ser.write(c)
    time.sleep(100)
    #Set Baseline
    cmd3 = binary + " -b -x " + baselinefile
    print cmd3

    #with open(args.tfile, "w") as tfile: # For python 2.7
    with open(options.tfile, "w") as tfile:
        tfile.write("#position,date,temperatur(degree celsius),datafoldername\n")
        for s in setpoints:
            print s
            # send position information to SandBox controller
            #print "Sending:  >>",
            for c in s:
                #print c,
                ser.write(c)
            #print "<<"
            isopen(ser)

            time.sleep(4)
	
            # Take data
            cmd1 = binary + " -b -x " + baselinefile
            cmd2 = binary + " -x " + xmlfile + " -s " + str(datafolder)
            # print "\n"+cmd1     #set baseline everytime
            print cmd2
            #  os.system(cmd1)    #set baseline every fucking time
            # Write temperature into file
            temperature = ser.readline().rstrip()
            logfileline = s + ',' + \
                datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S") + \
                ',' + str(temperature) + ',' +str(datafolder)+ '\n'
            tfile.write(logfileline)
            os.system(cmd2)
	    print datafolder
	    datafolder = datafolder+1



except KeyboardInterrupt:
    print "Got Ctrl-c"
    # Reset and go to zero position
    s = "0000000000"
    for c in s:
        ser.write(c)
    time.sleep(2)
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



#print "cpy data on farm"
#os.system("rm miniDAX*.old")
#os.system("cp " + mapfile + ' ' + datapath)
#os.system("cp " + options.tfile + ' '+ datapath)
#os.system("rsync -ae ssh " + datapath + " xurich@farm-ui:data/SandBox/")

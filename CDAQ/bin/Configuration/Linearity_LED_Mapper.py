
###############################################################################
###############################################################################
#
# MAPPER_1.0
#
# Description:
# produces and prints mapfile for the Linearity Meassurement
#
# written by Julien Wulf April 2015
#
###############################################################################
###############################################################################

import numpy as np
import pylab as p
import sys

if len(sys.argv) > 1:
    outputfilename = argv[1]
else:
    outputfilename = "map"


# Constants
a = 0.005	     # distance between two LED settings
LEDStart=2.000	     # First Point of the LED
numberPoints=100;
number = True


FileNames =   []
LEDSetting =  []

# Generating map points
#numberPoints=(LEDEnd-LEDStart)/a
for i in range(numberPoints+2):
        FileNames.append(`LEDStart` + "V")
        LEDSetting.append(`LEDStart`)
        LEDStart =  	round(LEDStart + a,3)

with open (outputfilename, "w") as mapfileoutput:
     for i,j in zip(FileNames,LEDSetting):
         mapfileoutput.write(i +" "+ j +'\n')







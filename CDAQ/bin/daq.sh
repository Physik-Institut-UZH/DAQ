#!/bin/bash
for k in `seq 1 100000`;
do
	cd /home/sandbox/DAQ_dev/DAQ/CDAQ/bin/
	echo "taking data set ${k}"
	./SingleDAQ -x SingleADC1730_Noise.xml -f NoiseRun_$k
	sleep 2h
done


#!/bin/bash
run="Run"
spechChar="_"
spechChar1="/"
#alias sandaq='cd /data/Software/DAQ/CDAQ/bin/'

for i in {1..300}
do
#	print sandaq_dir
#	sandaq_dir
	cd /data/Software/DAQ/CDAQ/bin/
	echo "taking data set ${i}"
	./SingleDAQ -x SingleADC1720.xml -f RJ$i
	#sleep 10m
done


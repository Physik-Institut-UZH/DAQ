#!/bin/bash
run="Run"
spechChar="_"
spechChar1="/"

for i in {0..100000}
do
	cd /home/sandbox/DAQ_dev/DAQ/Scaler/bin/
	echo "taking data set ${i}"
	./Scaler -x Settings.xml
done


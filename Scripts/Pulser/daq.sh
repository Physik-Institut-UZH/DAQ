for i in $(seq $new 1000000)
do
	cd /home/sandbox/DAQ_dev/DAQ/Pulser/
	python pulser_on_PMT_Gain.py
	cd /home/sandbox/DAQ_dev/DAQ/CDAQ/bin/
	echo "taking data set Gain PMT ${i}"
	./SingleDAQ -x PMTMeasurementGain.xml -f Gain_PMT$i
    	sleep 5s
	cd /home/sandbox/DAQ_dev/DAQ/Pulser/
	python pulser_on_PMT_AP.py
	cd /home/sandbox/DAQ_dev/DAQ/CDAQ/bin/
	echo "taking data set AP PMT ${i}"
	./SingleDAQ -x PMTMeasurementGain.xml -f AP_PMT$i
	cd /home/sandbox/DAQ_dev/DAQ/Pulser/
	python pulser_off.py
	sleep 6h
	echo "taking DC"
done



#!/bin/bash



for i in {0..15}
do
    snmpset -v 2c -m +WIENER-CRATE-MIB -c guru 130.242.128.166 outputSwitch.u$i i 0
done


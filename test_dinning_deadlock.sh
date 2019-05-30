#!/bin/sh
while true 
do
#LD_PRELOAD=./build/ddtector.so ./deadlock/dinning_deadlock
LD_PRELOAD=./build/dmonitor.so ./deadlock/dinning_deadlock
echo
echo --------------------------------------------
echo
sleep 0.2
done

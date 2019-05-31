#!/bin/sh
while true 
do
LD_PRELOAD=./build/ddtector.so ./deadlock/build/abba
done

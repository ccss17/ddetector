all: 
	gcc -o abba abba.c -pthread
	gcc -shared -fPIC -o ddetector.so ddtector.c -ldl
	gcc -o dinning_deadlock dinning_deadlock.c -pthread


clean:
	rm -rf abba dinning_deadlock ddetector.so

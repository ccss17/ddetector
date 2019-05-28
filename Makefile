all: ddtector example

ddtector: ddtector.c
	gcc -shared -fPIC -o ddtector.so ddtector.c -ldl

example: abba.c dining_deadlock.c
	gcc -o abba abba.c
	gcc -o dining_deadlock dining_deadlock.c

clean:
		rm -rf ddtector example

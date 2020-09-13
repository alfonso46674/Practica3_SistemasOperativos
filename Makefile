all: serial posix

serial: serial.c
	gcc -o serial serial.c -lm

posix: posix.c
	gcc -o posix posix.c -lm -pthread

clean:
	rm serial posix

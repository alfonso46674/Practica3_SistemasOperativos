all: serial posix clone parallel

serial: serial.c
	gcc -o serial serial.c -lm

posix: posix.c
	gcc -o posix posix.c -lm -pthread

clone: clone.c
	gcc -o clone clone.c -lm 

parallel: parallel.c
	gcc -o parallel parallel.c -lm

clean:
	rm serial posix clone parallel

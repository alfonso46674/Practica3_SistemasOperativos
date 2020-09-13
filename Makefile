all: serial

init: init.c
	gcc -o serial serial.c

clean:
	rm serial

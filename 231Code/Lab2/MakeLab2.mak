CC=gcc
CFLAGS=-Wall

all: ECE231_Lab_2.exe

ECE231_Lab_2.exe: ECE231_Lab_2.c
	$(CC) $(CFLAGS) -o ECE231_Lab_2.exe ECE231_Lab_2.c 

.PHONY: clean
clean:
	del ECE231_Lab_2.exe

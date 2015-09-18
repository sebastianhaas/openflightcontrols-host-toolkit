CC=gcc
INC=-I/usr/local/include/hidapi/

.PHONY: all

all: first

first: first.o ncursesdisplay.o
	$(CC) -Wall $(INC) -o first first.o ncursesdisplay.o -lhidapi-libusb -lpiclib -lncurses

first.o: first.c
	$(CC) -Wall $(INC) $< -o $@ -c

ncursesdisplay.o: ncursesdisplay.c
	$(CC) -Wall $(INC) $< -o $@ -c

.PHONY: clean

clean:
	rm -f *.o
	rm -f *.out
	rm -f first

CC=gcc
INC=-I/usr/local/include/hidapi/

first: first.c
	$(CC) -Wall $(INC) $< -o $@ -lhidapi-libusb

.PHONY: clean

clean:
	rm -f first

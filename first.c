#ifdef WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <hidapi.h>
#include "first.h"

#define MAX_STR 255

int main(int argc, char* argv[])
{
	int res;
	unsigned char buf[64] = {0};
	wchar_t wstr[MAX_STR];
	hid_device *handle;
	int i;

	// Initialize the hidapi library
	printf("Starting to initialize HID library...\n");
	res = hid_init();
	printf("Initialized HID library.\n");

	// Open the device using the VID, PID,
	// and optionally the Serial number.
	printf("Trying to open device...\n");
	handle = hid_open(0x04d8, 0x003f, NULL);
	if(handle != NULL) {
		printf("Opened device successfully.\n");
	} else {
		printf("Could not open device.\n");
		exit(EXIT_FAILURE);
	}
	// Read the Manufacturer String
	printf("Trying to read manufacturer string...\n");
	res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	wprintf(L"The manufacturer string is: %s\n", wstr);

	// Read the Product String
	res = hid_get_product_string(handle, wstr, MAX_STR);
	wprintf(L"Product String: %s\n", wstr);

	// Read the Serial Number String
	res = hid_get_serial_number_string(handle, wstr, MAX_STR);
	wprintf(L"Serial Number String: (%d) %s\n", wstr[0], wstr);

	// Read Indexed String 1
	res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
	wprintf(L"Indexed String 1: %s\n", wstr);

	// Toggle LED (cmd 0x80). The first byte is the report number (0x0).
	buf[0] = 0x0;
	buf[1] = 0x80;
	res = hid_write(handle, buf, 64);

	// Request state (cmd 0x81). The first byte is the report number (0x0).
	buf[0] = 0x0;
	buf[1] = 0x81;
	res = hid_write(handle, buf, 64);

	// Read requested state
	res = hid_read(handle, buf, 64);

	// Print out the returned buffer.
	for (i = 0; i < 2; i++)
		printf("buf[%d]: %d\n", i, buf[i]);
	printf("Button is %spressed.\n", buf[1] ? "not " : "");

	// Request state (cmd 0x37). The first byte is the report number (0x0).
	buf[0] = 0x0;
	buf[1] = 0x37;
	res = hid_write(handle, buf, 64);
	// Read in the requested state
	res = hid_read(handle, buf, 64);

	printf("The decimal result is %d\n", merge_uint8(buf[1], buf[2]));

	// Print out the returned buffer.
	for (i = 0; i < 3; i++)
		printf("buf[%d]: %d\n", i, buf[i]);

	// Finalize the hidapi library
	res = hid_exit();

	return 0;
}

uint16_t merge_uint8(uint8_t lowerBits, uint8_t higherBits)
{
	uint16_t result = ((uint16_t)higherBits << 8) | lowerBits;
	return result;
}


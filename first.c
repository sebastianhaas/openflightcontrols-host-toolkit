#include "ncursesdisplay.h"
#include "first.h"

unsigned char buf[64] = {0};
hid_device *handle;

// Device state
uint16_t pot1;

void toggle_led1()
{
	static unsigned char CMD = 0x80;
	int response;

	// Toggle LED (cmd 0x80). The first byte is the report number (0x0).
	buf[0] = 0x0;
	buf[1] = CMD;
	response = hid_write(handle, buf, 64);
	if(response < 0)
	{
		printf("Error sending command %x", CMD);
		return;
	}
}

bool get_push1_state()
{
	static unsigned char CMD = 0x81;
	int response;

	// Request state (cmd 0x81). The first byte is the report number (0x0).
	buf[0] = 0x0;
	buf[1] = CMD;
	response = hid_write(handle, buf, 64);
	if(response < 0)
	{
		printf("Error sending command %x", CMD);
		return false;
	}

	// Read requested state
	response = hid_read(handle, buf, 64);
	if(response < 0)
	{
		printf("Error reading response for command %x", CMD);
		return false;
	}

	// Print out the returned buffer.
	// for (i = 0; i < 2; i++)
	// 	printf("buf[%d]: %d\n", i, buf[i]);
	return !buf[1];
}

uint16_t get_pot1_val()
{
	static unsigned char CMD = 0x37;
	int response;

	// Request state (cmd 0x37). The first byte is the report number (0x0).
	buf[0] = 0x0;
	buf[1] = CMD;
	response = hid_write(handle, buf, 64);
	if(response < 0)
	{
		printf("Error sending command %x", CMD);
		return -1;
	}

	// Read in the requested state
	response = hid_read(handle, buf, 64);
	if(response < 0)
	{
		printf("Error reading response for command %x", CMD);
		return -1;
	}

	return merge_uint8(buf[1], buf[2]);

	// Print out the returned buffer.
	// for (i = 0; i < 3; i++)
	// 	printf("buf[%d]: %d\n", i, buf[i]);
}

void read_device_info()
{
	int res;
	wchar_t wstr[MAX_STR];

	// Read the Manufacturer String
	printf("Trying to read manufacturer string...\n");
	res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	wprintf(L"The manufacturer string is: %ls\n", wstr);

	// Read the Product String
	res = hid_get_product_string(handle, wstr, MAX_STR);
	wprintf(L"Product String: %ls\n", wstr);

	// Read the Serial Number String
	res = hid_get_serial_number_string(handle, wstr, MAX_STR);
	wprintf(L"Serial Number String: (%d) %ls\n", wstr[0], wstr);

	// Read Indexed String 1
	res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
	wprintf(L"Indexed String 1: %ls\n", wstr);
}

int main(int argc, char* argv[])
{
	int res;
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
	read_device_info();

	toggle_led1();

	printf("Button is %spressed.\n", get_push1_state() ? "" : "not ");

	printf("The decimal result is %d\n", get_pot1_val());

	// Register signal handler
	if (signal(SIGINT, sig_handler) == SIG_ERR) {
        printf("\nCan't catch SIGINT\n");
        exit(EXIT_FAILURE);
	}

	// Enter ncurses mode
	initialize_display();

	// Main loop
	while(1) {
		int a;
		a = (int)((100 / 1023.0) * get_pot1_val()); 
		print_percent_bar(2, a);
		refresh();
		sleep(0.1);
	}

	// Leave ncurses mode
	end_display();

	// Unregister signal handler
	signal(SIGINT, SIG_DFL);

	// Finalize the hidapi library
	res = hid_exit();

	return 0;
}

void sig_handler(int signo)
{
	if (signo == SIGINT)
	{
		end_display();
		exit(EXIT_SUCCESS);
	}
}

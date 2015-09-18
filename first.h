#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <hidapi.h>
#include <piclib.h>

#define MAX_STR 255

void sig_handler(int signo);
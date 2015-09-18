#include <stdlib.h>
#include "ncursesdisplay.h"

int min_x;
int max_x;
int min_y;
int max_y;

void initialize_display()
{
	// Start curses mode
	initscr();

	// Get screen/window size
	getbegyx(stdscr, min_y, min_x);
	getmaxyx(stdscr, max_y, max_x);
}

void end_display()
{
	// End curses mode
	endwin();
}

void print_percent_bar(int y, int percent)
{
	static const int LABEL_WIDTH = 5;
	int i;
	int percentDrawIndex;
 

	percentDrawIndex = (int)(((max_x - LABEL_WIDTH) / 100.0) * percent); 
	for(i = min_x; i < percentDrawIndex; i++)
	{
		mvprintw(y, i, "#");
	}

	mvprintw(y, max_x - LABEL_WIDTH, " %3d%%", percent);
}
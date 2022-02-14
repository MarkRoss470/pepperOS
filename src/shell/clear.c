#include "clear.h"

#include "../screen.h"
#include "../graphics.h"

int clear(int argc, char *argv[])
{
	screen_clear(backgroundColour);
	line = -1;
	column = 0;
	return(0);
}

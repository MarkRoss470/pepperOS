#include "colour.h"

#include "../graphics.h"

int colourtest(int argc, char *argv[])
{
	puts("\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	for(uint8_t x = 0; x < 16; x++)
	{
		for(uint8_t y = 0; y < 16; y++)
		{
			draw_rect(x * 10, y * 10, 10, 10, (x << 4) | y);
		}
	}
	return(0);
}

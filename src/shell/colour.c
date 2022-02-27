#include "colour.h"

#include "../graphics.h"
#include "../screen.h"

int colourtest(int argc, char *argv[])
{
	puts("\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	for(uint8_t x = 0; x < 255; x++)
	{
		for(uint8_t y = 0; y < 255; y++)
		{
			//draw_rect(x * 10, y * 10, 10, 10, (x << 4) | y);
			set_pixel(x, y, (x << 8) | y);
		}
	}
	
	//draw_mario(0);
	
	screen_swap();
	return(0);
}

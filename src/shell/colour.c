#include <shell/colour.h>

#include <graphics.h>
#include <screen.h>

int colourtest(int argc, char *argv[])
{
	for(uint16_t x = 0; x < VESA_chosen_mode_buffer.width; x++)
	{
		for(uint16_t y = 0; y < VESA_chosen_mode_buffer.height; y++)
		{
			set_pixel(x, y, (x << 8) | y);
		}
	}
	screen_swap();
	return(0);
}

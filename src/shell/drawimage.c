#include <shell/drawimage.h>

#include <screen.h>
#include <graphics.h>
#include <stdint.h>

extern uint32_t _binary_src_image_bin_start[];
#define IMAGE_SIZE 10
int drawimage(int argc, char *argv[])
{
	uint32_t width = _binary_src_image_bin_start[0];
	uint32_t height = _binary_src_image_bin_start[1];
	
	uint32_t i = 2;
	
	for(uint32_t y = 0; y < height; y++)
	{
		for(uint32_t x = 0; x < width; x++)
		{
			draw_rect(x * IMAGE_SIZE, y * IMAGE_SIZE, IMAGE_SIZE, IMAGE_SIZE, _binary_src_image_bin_start[i]);
			i++;
		}
	}
	return(0);

}

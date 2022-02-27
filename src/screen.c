#include "screen.h"


u32 *BUFFER;

// double buffers
u32 _sbuffer[SCREEN_SIZE];
u32 _sback = 0;

const int num_colours = NUM_COLOURS;
const char colour_names[NUM_COLOURS][20] = {
	"black",
	"white",
	
	"red",
	"green",
	"blue",
	
	"yellow",
	"cyan",
	"pink",
	"purple",
	"orange",
	
	"light_gray",
	"middle_gray",
	"dark_gray"
};
const uint32_t colour_values[NUM_COLOURS] = {
	COLOUR_BLACK,
	COLOUR_WHITE,

	COLOUR_RED,
	COLOUR_GREEN,
	COLOUR_BLUE,
	
	COLOUR_YELLOW,
	COLOUR_CYAN,
	COLOUR_PINK,
	COLOUR_PURPLE,
	COLOUR_ORANGE,
	
	COLOUR_LIGHT_GRAY,
	COLOUR_MIDDLE_GRAY,
	COLOUR_DARK_GRAY,
};


#define SWAP() (_sback = 1 - _sback)

void screen_swap() {

    	for(int y = 0; y < VESA_chosen_mode_buffer.height; y++)
	{
		for(int x = 0; x < VESA_chosen_mode_buffer.width; x++)
		{
			BUFFER[(y * VESA_chosen_mode_buffer.pitch / 4) + x] = _sbuffer[(y * VESA_chosen_mode_buffer.pitch / 4) + x];
		}
	}

}

void screen_clear(u32 colour) {
	for(int y = 0; y < VESA_chosen_mode_buffer.height; y++)
	{
		for(int x = 0; x < VESA_chosen_mode_buffer.width; x++)
		{
			_sbuffer[(y * VESA_chosen_mode_buffer.pitch / 4) + x] = colour;
		}
	}
}

void screen_buffer_clear(u32 colour) {
	for(int y = 0; y < VESA_chosen_mode_buffer.height; y++)
	{
		for(int x = 0; x < VESA_chosen_mode_buffer.width; x++)
		{
			BUFFER[(y * VESA_chosen_mode_buffer.pitch / 4) + x] = colour;
		}
	}
}

void screen_init() {
    BUFFER = (u32*) VESA_chosen_mode_buffer.framebuffer;
}

void scroll_screen(int pixels, uint32_t fillcolour)
{
	int offset = (pixels * VESA_chosen_mode_buffer.pitch / 4);
	for(int i = 0; i < (SCREEN_SIZE - offset); i++)
	{
		_sbuffer[i] = _sbuffer[i + offset];
	}
	for(int i = SCREEN_SIZE - offset + 1; i < SCREEN_SIZE; i++)
	{
		_sbuffer[i] = fillcolour;
	}
}



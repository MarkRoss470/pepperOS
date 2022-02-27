#ifndef SCREEN_H
#define SCREEN_H

#include "util/types.h"
#include "util/mem.h"
#include "util/IO.h"

#include <stdint.h>

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 1200
#define SCREEN_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT)

extern u32* BUFFER;
extern u32 _sbuffer[SCREEN_SIZE];

void screen_swap();
void screen_clear(u32 color);
void screen_buffer_clear(u32 color);
void screen_init();
void scroll_screen(int pixels, uint32_t fillcolour);


enum
{
	COLOUR_BLACK = 0x000000,
	COLOUR_WHITE = 0xffffff,
	
	COLOUR_RED = 0xff0000,
	COLOUR_GREEN = 0x00ff00,
	COLOUR_BLUE = 0x0000ff,
	
	COLOUR_YELLOW = 0xffff00,
	COLOUR_CYAN = 0x00ffff,
	COLOUR_PINK = 0xff00ff,
	COLOUR_PURPLE = 0x800080,
	COLOUR_ORANGE = 0xffa500,
	
	COLOUR_LIGHT_GRAY = 0xc0c0c0,
	COLOUR_MIDDLE_GRAY = 0x808080,
	COLOUR_DARK_GRAY = 0x404040,
	
};

#define NUM_COLOURS 13
extern const int num_colours;
extern const char colour_names[NUM_COLOURS][20];
extern const uint32_t colour_values[NUM_COLOURS];

struct vbe_mode_info {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
 
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;
 
	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));
extern struct vbe_mode_info VESA_chosen_mode_buffer;
extern struct vbe_mode_info VESA_mode_buffer;

inline void set_pixel(uint16_t x, uint16_t y, uint32_t colour)
{
	if(x > VESA_chosen_mode_buffer.width || y > VESA_chosen_mode_buffer.height)return;
	_sbuffer[(VESA_chosen_mode_buffer.pitch / 4) * y + x] = colour;
}


inline uint32_t get_pixel(uint16_t x, uint16_t y)
{
	if(x > VESA_chosen_mode_buffer.width || y > VESA_chosen_mode_buffer.height)return(0);
	return(_sbuffer[(VESA_chosen_mode_buffer.pitch / 4) * y + x]);
}

/*
#define TEST_WIDTH 200
#define TEST_HEIGHT 1000

inline void set_pixel(uint16_t x, uint16_t y, uint32_t colour)
{
	if(x > TEST_WIDTH || y > TEST_HEIGHT)return;
	CURRENT[(TEST_WIDTH * y) + x] = colour;
	return;
}
*/

#endif

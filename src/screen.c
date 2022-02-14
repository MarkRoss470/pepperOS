#include "screen.h"


static u8 *BUFFER = (u8 *) 0xA0000;

// double buffers
u8 _sbuffers[2][SCREEN_SIZE];
u8 _sback = 0;

const int num_colours = NUM_COLOURS;
const char colour_names[NUM_COLOURS][10] = {
	"black",
	"white",
	
	"red",
	"green",
	"blue"
};
const uint8_t colour_values[NUM_COLOURS] = {
	COLOUR_BLACK,
	COLOUR_WHITE,
	
	COLOUR_RED,
	COLOUR_GREEN,
	COLOUR_BLUE
};

#define CURRENT (_sbuffers[_sback])
#define OTHER (_sbuffers[1-_sback])
#define SWAP() (_sback = 1 - _sback)

// VGA control port addresses
#define PALETTE_MASK 0x3C6
#define PALETTE_READ 0x3C7
#define PALETTE_WRITE 0x3C8
#define PALETTE_DATA 0x3C9

void screen_swap() {
    memcpy(BUFFER, &CURRENT, SCREEN_SIZE);
    memcpy(&OTHER, &CURRENT, SCREEN_SIZE);
    SWAP();
}

void screen_clear(u8 color) {
    memset(&CURRENT, color, SCREEN_SIZE);
}

void screen_init() {
    // configure palette with 8-bit RRRGGGBB color
    outb(PALETTE_MASK, 0xFF);
    outb(PALETTE_WRITE, 0);
    for (u8 i = 0; i < 255; i++) {
        outb(PALETTE_DATA, (((i >> 5) & 0x7) * (256 / 8)) / 4);
        outb(PALETTE_DATA, (((i >> 2) & 0x7) * (256 / 8)) / 4);
        outb(PALETTE_DATA, (((i >> 0) & 0x3) * (256 / 4)) / 4);
    }

    // set color 255 = white
    outb(PALETTE_DATA, 0x3F);
    outb(PALETTE_DATA, 0x3F);
    outb(PALETTE_DATA, 0x3F);
}

int set_pixel(uint16_t x, uint16_t y, uint8_t colour)
{
	if(x > 320 || y > 240)return(1);
	CURRENT[(320 * y) + x] = colour;
	return(0);
}

uint8_t get_pixel(uint16_t x, uint16_t y)
{
	if(x > 320 || y > 240)return(0);
	return(CURRENT[(320 * y) + x]);
}

uint8_t get_colour_from_values(int r, int g, int b)
{
	return(COLOR(r, g, b));
}

void scroll_screen(int pixels, uint8_t fillcolour)
{
	int offset = (pixels * SCREEN_WIDTH);
	for(int i = 0; i < (SCREEN_SIZE - offset); i++)
	{
		CURRENT[i] = CURRENT[i + offset];
	}
	for(int i = SCREEN_SIZE - offset + 1; i < SCREEN_SIZE; i++)
	{
		CURRENT[i] = fillcolour;
	}
}



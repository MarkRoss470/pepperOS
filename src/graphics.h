#pragma once

#include <stdint.h>
#include <stdarg.h>


int draw_rect(uint16_t xPos, uint16_t yPos, uint16_t width, uint16_t height, uint32_t colour);
int setpixel(uint16_t xPos, uint16_t yPos, uint32_t colour);
int draw_text_highlight(const char* string, uint16_t xPos, uint16_t yPos, uint32_t textColour, uint32_t backgroundColor, int cursor_pos);
int draw_text(const char* string, uint16_t xPos, uint16_t yPos, uint32_t textColour, uint32_t backgroundColor);
void draw_mario();

extern int line;
extern int column;

extern uint32_t textColour;
extern uint32_t backgroundColour;

void putchar(char c);
void puts(char* text);
int printf(char* text, ...);

void draw_line(uint16_t startX, uint16_t startY, uint16_t endX, uint16_t endY, uint32_t colour);


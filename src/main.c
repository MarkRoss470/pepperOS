#include "util/strings.h"
#include "util/types.h"

#include "screen.h"
#include "idt.h"
#include "isr.h"
#include "irq.h"
#include "timer.h"
#include "font.h"
#include "system.h"
#include "keyboard.h"
#include "fpu.h"

#include <stdint.h>
#include "graphics.h"

#include "shell/shell.h"

bool is_multiple(u64 divided, u64 divisor)
{
	if(divisor > divided)return(false);
	if(divisor == divided)return(true);
	if(divisor == 0)return(false);
	u64 temp = divisor;
	while(temp < divided)
	{
		temp += divisor;
	}
	return(temp == divided);
}

#define INPUT_BUFFER_SIZE 1024
char input_text[INPUT_BUFFER_SIZE];
int input_length = 0;
int cursor_pos = 0;

void handle_keyboard()
{
	if(keybuffer.start != keybuffer.end)
	{
		char newchar = keybuffer.buffer[keybuffer.start];
		if(newchar == 0x00)
		{
			keybuffer.start += 1;
			//left arrow
			if(keybuffer.buffer[keybuffer.start] == 0x01)
			{
				if(cursor_pos != 0)
				{
					cursor_pos -= 1;
				}
			}
			//right arrow
			else if(keybuffer.buffer[keybuffer.start] == 0x02)
			{
				if(cursor_pos != input_length)
				{
					cursor_pos += 1;
				}
			}
			else{}
			
		}
		else if(newchar == '\b')
		{
			if(cursor_pos != 0){
				input_length -= 1;
				cursor_pos -= 1;
			}
			int i = cursor_pos;
			do
			{
				input_text[i] = input_text[i+1];
				i++;
			}
			while(input_text[i] != '\0');
		}
		else if(newchar == '\n')
		{
			puts(input_text);
			putchar('\n');
			keybuffer.start++;
			run_command(input_text);
			puts("\n>");
			input_length = 0;
			cursor_pos = 0;
			input_text[0] = '\0';
			
			screen_swap();
			return;
		}
		//tab does nothing
		else if(newchar == '\t'){}
		else
		{
			if(input_length != INPUT_BUFFER_SIZE){
				for(int i = input_length + 1; i > cursor_pos; i--)
				{
					input_text[i] = input_text[i-1];
				}
				input_text[cursor_pos] = newchar;
				input_text[input_length + 1] = '\0';
				input_length += 1;
				cursor_pos += 1;
			}
		}
		
		keybuffer.start++;
		
		char input_with_blank[INPUT_BUFFER_SIZE];
		strlcpy(input_with_blank, input_text, input_length+1);
		input_with_blank[input_length] = ' ';
		input_with_blank[input_length+1] = '\0';		
		draw_text_highlight(input_with_blank, column * 10, line * 10, textColour, backgroundColour, cursor_pos-1);
		screen_swap();
	}
}

void _main(u32 magic) {
	idt_init();
	isr_init();
	irq_init();
	keyboard_init();
	timer_init();
	screen_init();
	
	putchar('>');
	screen_swap();
	for(;;)
	{
		handle_keyboard();
	}
}

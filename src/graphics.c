#include <graphics.h>
#include <font_mine.h>
#include <screen.h>
#include <util/strings.h>
#include <util/math.h>

uint32_t textColour = 0xffffff;
uint32_t backgroundColour = 0x000000;

//draws a rectange with the upper left corner at a specified position, and with a specified width, height, and colour.
//returns zero on success, one otherwise
int draw_rect(uint16_t xPos, uint16_t yPos, uint16_t width, uint16_t height, uint32_t colour)
{
	if((xPos+width) > VESA_chosen_mode_buffer.width || (yPos+height) > VESA_chosen_mode_buffer.height)return(1);
	for(int x = xPos; x < (xPos + width); x++)
	{
		for(int y = yPos; y < (yPos + height); y++)
		{
			set_pixel(x, y, colour);
		}
	}
	return(0);
}

int draw_text_highlight(const char* string, uint16_t xPos, uint16_t yPos, uint32_t textColour, uint32_t backgroundColour, int cursor_pos)
{
	int i = 0;
	uint16_t charstartx = xPos;
	uint16_t charstarty = yPos;
	while(string[i] != '\0')
	{
		if(string[i] == '\n')
		{
			charstartx = 0;
			charstarty += 10;
			i++;
			continue;
		}
		const uint8_t *character;
		character = font_char_mine(string[i]);
		for(int x = 0; x < 8; x++)
		{
			for(int y = 0; y < 8; y++)
			{
				if((i == cursor_pos) != ((character[y] & (1 << (x))) > 0))
				{
					set_pixel((uint16_t)x + charstartx,(uint16_t) y + charstarty, textColour);
				}
				else
				{
					set_pixel((uint16_t)x + charstartx,(uint16_t) y + charstarty, backgroundColour);
				}
			}
		}
		i++;
		charstartx += 10;
		if(charstartx > VESA_chosen_mode_buffer.width)
		{
			charstartx = 0;
			charstarty += 10;
		}
	}
	return(i);
}

int draw_text(const char* string, uint16_t xPos, uint16_t yPos, uint32_t textColour, uint32_t backgroundColor)
{
	return(draw_text_highlight(string, xPos, yPos, textColour, backgroundColor, -1));
}

void draw_mario(int pose)
{
	//0 = empty / white
	//1 = red
	//2 = brown
	//3 = yellow / skin tone

	static const uint8_t mario[2][16][16] = {{
	{0,0,0,1,1,1,1,1,0,0,0,0},
	{0,0,1,1,1,1,1,1,1,1,1,0},
	{0,0,2,2,2,3,3,2,3,0,0,0},
	{0,2,3,2,3,3,3,2,3,3,3,0},
	{0,2,3,2,2,3,3,3,2,3,3,3},
	{0,2,2,3,3,3,3,2,2,2,2,0},
	{0,0,0,3,3,3,3,3,3,3,0,0},
	{0,0,2,2,1,2,2,2,0,0,0,0},
	{0,2,2,2,1,2,2,1,2,2,2,0},
	{2,2,2,2,1,1,1,1,2,2,2,2},
	{3,3,2,1,3,1,1,3,1,2,3,3},
	{3,3,3,1,1,1,1,1,1,3,3,3},
	{3,3,1,1,1,1,1,1,1,1,3,3},
	{0,0,1,1,1,0,0,1,1,1,0,0},
	{0,2,2,2,0,0,0,0,2,2,2,0},
	{0,2,2,2,0,0,0,0,2,2,2,0}
	},
	{
	{0,0,0,0,0,0,1,1,1,1,1,0,0,3,3,3},
	{0,0,0,0,0,1,1,1,1,1,1,1,1,1,3,3},
	{0,0,0,0,0,2,2,2,3,3,2,3,0,2,2,2},
	{0,0,0,0,2,3,2,3,3,3,2,3,3,3,2,2},
	{0,0,0,0,2,3,2,2,3,3,3,2,3,3,3,2},
	{0,0,0,0,0,2,3,3,3,3,2,2,2,2,2,0},
	{0,0,0,0,0,0,3,3,3,3,3,3,3,2,0,0},
	{0,0,2,2,2,2,2,1,2,2,2,1,2,0,0,0},
	{0,2,2,2,2,2,2,2,1,2,2,2,1,0,0,0},
	{3,3,2,2,2,2,2,2,1,1,1,1,1,0,0,2},
	{3,3,3,0,0,1,2,1,1,1,3,1,3,1,2,2},
	{0,3,0,2,0,1,1,1,1,1,1,1,1,1,2,2},
	{0,0,2,2,2,1,1,1,1,1,1,1,1,1,2,2},
	{0,2,2,2,1,1,1,1,1,1,1,0,0,0,0,0},
	{0,2,0,0,1,1,1,1,0,0,0,0,0,0,0,0},
	}};
	
	
	for(int x = 0; x < 16; x++)
	{
		for(int y = 0; y < 16; y++)
		{
			int pixelcolour = mario[pose][y][x];
			//int pixelcolour = (x%3)+(y%3);
			uint32_t VGAcolour = 0;
			if (pixelcolour == 0)
			{
				//white
				VGAcolour = 0xff;
			}
			else if (pixelcolour == 1)
			{
				//red
				VGAcolour = 0xff0000;
			}
			else if (pixelcolour == 2)
			{
				//brown
				VGAcolour = 0xbbbb00;
			}
			else if (pixelcolour == 3)
			{
				//skin tone
				VGAcolour = 0xffdd00;
			}
			draw_rect(x * 10, y * 10, 10, 10, VGAcolour);
		}
	}
	
	draw_text("Mario", 0, 160, 0xffffff, 0x000000);
}

int line = 0;
int column = 0;

void putchar(char c)
{
	if(c == '\n')
	{
		column = 0;
		line++;
		return;
	}

	const uint8_t *font_char = font_char_mine(c);
	int pixelX = (column * 10);
	int pixelY = (line * 10);
	for(int x = 0; x < 8; x++)
	{
		for(int y = 0; y < 8; y++)
		{
			if((font_char[y] & (1 << (x))) > 0)
			{
				set_pixel((uint16_t) x + pixelX,(uint16_t) y + pixelY, textColour);
			}
			else
			{
				set_pixel((uint16_t) x + pixelX,(uint16_t) y + pixelY, backgroundColour);
			}
		}
	}
	column++;

	if(column > VESA_chosen_mode_buffer.width/10)
	{
		column = 0;
		line++;
	}
	if(line > VESA_chosen_mode_buffer.height/10 - 4)
	{
		scroll_screen(10, 0);
		line--;
	}
}

void puts(char* text)
{
	int i = 0;
	while(text[i] != '\0')
	{
		char character = text[i];
		
		putchar(character);
		
		i++;
	}
}

struct printf_flags
{
	bool justify;
	bool force_sign;
	bool pad_sign;
	bool print_prefix;
	bool pad_zeroes;
	
	unsigned int min_chars;
	unsigned int precision;
};

#define found_flag(FLAG, VALUE) i++;flags. FLAG = VALUE ;flag_found = true
#define check_flag(CHAR, FLAG, VALUE) if(format[i] == CHAR ){found_flag( FLAG , VALUE );}

int printf(char* format, ...)
{
	va_list valist;
	va_start(valist, format);
	
	int num_chars = 0;
	int i = -1;
	int j = 0;
	while(i == -1 || format[i] != '\0')
	{
		i++;
		if(format[i] != '%')continue;
		format[i] = '\0';
		puts(format + j);
		num_chars += strlen(format + j);
		format[i] = '%';
		i++;
		struct printf_flags flags = {.justify=false, .force_sign=false, .pad_sign=false, .print_prefix=false, .pad_zeroes=false,
		.min_chars=0, .precision=6};
		bool flag_found;
		do
		{
			flag_found = false;
			
			check_flag('-', justify, true);
			check_flag('+', force_sign, true);
			check_flag(' ', pad_sign, true);
			check_flag('#', print_prefix, true);
			check_flag('0', pad_zeroes, true);
			
			
		}while(flag_found);
		while((format[i] >= '1' && format[i] <= '9') || (format[i] == '0' && flag_found))
		{
			flags.min_chars *= 10;
			flags.min_chars += format[i] - '0';
			flag_found = true;
			i++;
		}
		if(format[i] == '*')
		{
			i++;
			flag_found = true;
			flags.min_chars = va_arg(valist, int);
		}
		
		if(format[i] == '.')
			{
				bool first_char = true;
				flag_found = true;
				flags.precision = 0;
				i++;
				if(format[i] == '*')
				{
					found_flag(min_chars, va_arg(valist, int));
				}
				else{
					while((format[i] >= '1' && format[i] <= '9') || (format[i] == '0' && !first_char))
					{
						flags.precision *= 10;
						flags.precision += format[i] - '0';
						i++;
					}
				}
			}
		
		switch(format[i])
		{
			int length;
			char buffer[50];
			case(0):
				i++;
				break;
			case('%'):
				putchar('%');
				num_chars++;
				break;
			case('s'):
				;
				char* string = va_arg(valist, char*);
				length = strlen(string);
				if(flags.justify)puts(string);
				for(int k = length; k < (int)flags.min_chars; k++)
				{
					putchar(' ');
					num_chars++;
				}
				if(!flags.justify)puts(string);
				num_chars += strlen(string);
				break;
			case('d'):case('i'):
				;
				{
				int number = va_arg(valist, int);
				
				bool extra_sign = false;
				if(number < 0)
				{
					if(flags.pad_zeroes)
					{
						putchar('-');
						num_chars++;
					}
					else
					{
						buffer[0] = '-';
						extra_sign = true;
					}
				}
				else if(flags.force_sign)
				{
					buffer[0] = '+';
					extra_sign = true;
				}
				else if(flags.pad_sign)
				{
					buffer[0] = ' ';
					extra_sign = true;
				}
				itoa(abs(number), buffer + extra_sign, 50 - extra_sign);
				
				length = strlen(buffer);
				
				if(!flags.justify)
				{
					for(int k = length; k < (int)flags.min_chars; k++)
					{
						puts(flags.pad_zeroes ? "0" : " ");
						num_chars++;
					}
				}
				
				puts(buffer);
				num_chars += length;
				
				if(flags.justify)
				{
					for(int k = length; k < (int)flags.min_chars; k++)
					{
						putchar(' ');
						num_chars++;
					}
				}
				}	
				break;
			case('u'):
				;
				{
				unsigned int number = va_arg(valist, unsigned int);
				
				uitoa(number, buffer, 50);
				
				length = strlen(buffer);
				
				if(!flags.justify)
				{
					for(int k = length; k < (int)flags.min_chars; k++)
					{
						puts(flags.pad_zeroes ? "0" : " ");
						num_chars++;
					}
				}
				
				puts(buffer);
				num_chars += length;
				
				if(flags.justify)
				{
					for(int k = length; k < (int)flags.min_chars; k++)
					{
						putchar(' ');
						num_chars++;
					}
				}
				}
				break;
			case('c'):
				putchar(va_arg(valist, int));
				num_chars++;
				break;
			case('n'):
				*va_arg(valist, int*) = num_chars;
				break;
			case('x'):
				;
				{
				unsigned int number = va_arg(valist, unsigned int);
				length = 0;
				bool extra_0x = false;
				if(flags.print_prefix)
				{
					if(flags.pad_zeroes)
					{
						puts("0x");
						length += 2;
					}
					else
					{
						buffer[0] = '0';
						buffer[1] = 'x';
						extra_0x = true;
					}
				}
				itoh(number, buffer + extra_0x * 2, 50 - extra_0x * 2);
				
				length += strlen(buffer);
				
				if(!flags.justify)
				{
					for(int k = length; k < (int)flags.min_chars; k++)
					{
						puts(flags.pad_zeroes ? "0" : " ");
						num_chars++;
					}
				}
				
				puts(buffer);
				num_chars += length;
				}
				break;
			case('X'):
				;
				{
				unsigned int number = va_arg(valist, unsigned int);
				length = 0;
				bool extra_0x = false;
				if(flags.print_prefix)
				{
					if(flags.pad_zeroes)
					{
						puts("0X");
						length += 2;
					}
					else
					{
						buffer[0] = '0';
						buffer[1] = 'X';
						extra_0x = true;
					}
				}
				ITOH(number, buffer + extra_0x * 2, 50 - extra_0x * 2);
				
				length += strlen(buffer);
				
				if(!flags.justify)
				{
					for(int k = length; k < (int)flags.min_chars; k++)
					{
						puts(flags.pad_zeroes ? "0" : " ");
						num_chars++;
					}
				}
				
				puts(buffer);
				num_chars += length;
				}
				break;
			
		}
		j = i + 1;
	}
	puts(format + j);
	num_chars += strlen(format + j);
	va_end(valist);
	return num_chars;
}

void draw_line(uint16_t startX, uint16_t startY, uint16_t endX, uint16_t endY, uint32_t colour)
{
	float gradient = ((float)(endY - startY)) / ((float)(endX - startX));
	for(uint16_t x = startX; x <= endX; x++)
	{
		uint16_t stripeEndY = startY + gradient * (x+1);
		for(uint16_t y = startY + gradient * x; y < stripeEndY + 1; y++)
		{
			set_pixel(x, y, colour);
		}
	}
}










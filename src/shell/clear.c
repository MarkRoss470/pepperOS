#include <shell/clear.h>

#include <screen.h>
#include <graphics.h>
#include <keyboard.h>
#include <util/strings.h>

int clear(int argc, char *argv[])
{
	screen_clear(backgroundColour);
	line = -1;
	column = 0;
	return(0);
}

int calibrate_screen(int argc, char *argv[])
{
	uint16_t stored_pitch = VESA_chosen_mode_buffer.pitch;
	uint16_t stored_width = VESA_chosen_mode_buffer.width;
	//VESA_chosen_mode_buffer.pitch = 200;
	bool exit = false;
	
        //return 0;
        bool drawn = true;
        while(true)
        {
        	VESA_chosen_mode_buffer.width = VESA_chosen_mode_buffer.pitch / 4;
		if(keybuffer.start != keybuffer.end)
		{
			switch(keybuffer.buffer[keybuffer.start])
			{
				case('d'):
					VESA_chosen_mode_buffer.pitch += 4;
					break;
				case('a'):
					VESA_chosen_mode_buffer.pitch -= 4;
					break;
				
				case('e'):
					VESA_chosen_mode_buffer.pitch += 40;
					break;
				case('q'):
					VESA_chosen_mode_buffer.pitch -= 40;
					break;
				
				case('c'):
					VESA_chosen_mode_buffer.pitch += 400;
					break;
				case('z'):
					VESA_chosen_mode_buffer.pitch -= 400;
					break;
				case('m'):
					VESA_chosen_mode_buffer.pitch += 4000;
					break;
				case('b'):
					VESA_chosen_mode_buffer.pitch -= 4000;
					break;
				case('r'):
					return(0);
				default:
					draw_text("U", 60, 0, 0xff00ff, 0x0000ff);
			}
			keybuffer.start++;
			drawn = true;
		}
		else if(drawn)
		{
			drawn = false;
			//uint16_t tempPitch = VESA_chosen_mode_buffer.pitch;
			//uint16_t tempWidth = VESA_chosen_mode_buffer.width;
			//VESA_chosen_mode_buffer.pitch = stored_pitch;
			//VESA_chosen_mode_buffer.width = stored_width;
			
			screen_clear(0xffff33);
			
			//VESA_chosen_mode_buffer.pitch = tempPitch;
			//VESA_chosen_mode_buffer.width = tempWidth;
			
			char numbers[30];
			itoa(keybuffer.end, numbers, 30);
			draw_text(numbers, 0, 0, 0xff00ff, 0xff0000);
			itoa(keybuffer.start, numbers, 30);
			
			draw_text(numbers, 30, 10, 0xff00ff, 0xff0000);
			itoa(VESA_chosen_mode_buffer.pitch, numbers, 30);
			draw_text(numbers, 0, 20, 0xffff00, 0xff00ff);
			itoa(stored_pitch, numbers, 30);
			draw_text(numbers, 0, 30, 0xffff00, 0xff00ff);
			
			screen_swap();
		}
	}
	return(0);
}

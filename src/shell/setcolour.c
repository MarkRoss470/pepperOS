#include "setcolour.h"

#include "../graphics.h"
#include "../screen.h"
#include "../util/strings.h"

int setcolour(int argc, char *argv[])
{
	//exit if at least one argument is not provided
	char* args_message = "setcolour requires at least one argument: text colour. Background colour is optional.";
	if(argc < 2 || argc > 3)
	{
		puts(args_message);
		return(1);
	}
	bool foundcolour = false;
	
	uint8_t newtextcolour = textColour;
	uint8_t newbackgroundcolour = backgroundColour;
	
	for(int i = 0; i < num_colours; i++)
	{
		if(strcmp(argv[1], (char*) colour_names[i]))
		{
			newtextcolour = colour_values[i];
			foundcolour = true;
			break;		
		}
	}
	if(!foundcolour)
	{
		puts(argv[1]);
		puts(": colour not found");
		return(1);
	}
	
	if(argc == 3){
		foundcolour = false;
		for(int i = 0; i < num_colours; i++)
		{
			if(strcmp(argv[2], (char*) colour_names[i]))
			{
				newbackgroundcolour = colour_values[i];
				foundcolour = true;
				break;		
			}
		}
		if(!foundcolour)
		{
			puts(argv[2]);
			puts(": colour not found");
			return(1);
		}
	}
	for(uint16_t x = 0; x < 320; x++)
	{
		for(uint16_t y = 0; y < 240; y++)
		{
			set_pixel(x, y, (get_pixel(x, y) == textColour) ? newtextcolour : newbackgroundcolour);
		}
	}
	textColour = newtextcolour;
	backgroundColour = newbackgroundcolour;
	return(0);
}

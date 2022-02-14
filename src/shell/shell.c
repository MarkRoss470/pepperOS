#include "shell.h"
#include "../util/strings.h"
#include "../util/mem.h"
#include "../graphics.h"

int help(int argc, char *argv[]);

static const char shell_builtins_names[][30] = {
	"help",
	"echo",
	"setcolour",
	"clear",
	"cpuinfo",
	"lspci",
	"mines",
	"colourtest",
	"printftest",
	"lsacpi",
	"enableacpi",
	"poweroff",
	"lsapic"
};

static int (*shell_builtins_functions[]) (int argc, char *argv[]) = {
	help,
	echo,
	setcolour,
	clear,
	cpuinfo,
	lspci,
	mines,
	colourtest,
	printftest,
	lsacpi,
	enableacpi,
	poweroff,
	lsapic
};

int help(int argc, char *argv[])
{
	for(int i = 0; i < (int)(sizeof(shell_builtins_names) /sizeof(shell_builtins_names[0])); i++)
	{
		puts((char*)shell_builtins_names[i]);
		if(i+1 != (int)(sizeof(shell_builtins_names) / sizeof(shell_builtins_names[0])))putchar('\n');

	}
	return(0);	
}

//TODO: replace with dynamic memory allocation when implemented: should be able to handle arbitrary length strings
//1030 is a few more than the char buffer of current sole caller, the keyboard handler
#define commandbuffer_SIZE 1030
static char commandbuffer[commandbuffer_SIZE];
//static array for argv - 515 entries is the largest argv can be with current 1030 char limit on input
char *argv[515];
int run_command(char* command)
{
	memset(commandbuffer, 0, commandbuffer_SIZE);
	int i = 0;
	bool firstCharReached = false;
	while(1)
	{
		if(command[i] == ' ')
		{
			//ignore leading spaces
			if(!firstCharReached)
			{
				continue;
			}
			//otherwise, a space means command is over
			else
			{
				break;
			}
		}
		//break loop if null char is reached
		else if(command[i] == '\0')
		{
			break;
		}
		else
		{
			firstCharReached = true;
		}
		//inc loop counter
		i++;
	}
	
	//if a string with no non-space chars was passed, do nothing and return
	if(!firstCharReached)
	{
		line -= 1;
		return(0);
	}
	
	//prepare argc and argv
	int argc = 0;
	//copy command input to command buffer
	int whole_command_length = strlen(command);
	strlcpy((char*)commandbuffer, command, whole_command_length+1);
	//loop through command buffer, setting spaces to null chars and recording their addresses in argv
	//NOTE: argv[0] is the program's invocation name by convention, and this system meets that need by it's nature - no special case needed
	
	//variable to store the previous argument (to prevent the arguments from being shifted in argv)
	char* prev_arg = (char*)&commandbuffer;
	//boolean textsincespace stores whether there has been a non space character sinse the last argument ended - to prevent 0-length arguments from being created when there are multiple spaces between arguments
	bool textsincespace = false;
	for(int i = 0; i < whole_command_length; i++)
	{
		//find spaces
		if(commandbuffer[i] == ' ')
		{
			if(!textsincespace)
			{
				continue;
			}
			
			//set argv[prev_arg] to a pointer to the previous string
			argv[argc] = prev_arg;
			
			//increment argc
			argc++;
			
			//set prev_arg to the new pointer
			prev_arg = (char*)&commandbuffer + i + 1;
			commandbuffer[i] = '\0';
			textsincespace = false;
		}
		else
		{
			//non space characters
			textsincespace = true;
		}
	}
	//add the final argument to argv (but only if it contains text)
	if(textsincespace){
		argc++;
		argv[argc-1] = prev_arg;
	}
	
	for(int i = 0; i < (int)(sizeof(shell_builtins_names) / sizeof(shell_builtins_names[0])); i++)
	{
		if(strcmp(commandbuffer, (char*)shell_builtins_names[i]))
		{
			return(shell_builtins_functions[i](argc, argv));
		}
	}
	//the command name in commandbuffer is now null-terminated (from setting up argv)
	puts(commandbuffer);
	puts(": Command not found");
	return(1);
}

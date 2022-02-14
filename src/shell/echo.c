#include "echo.h"

#include "../graphics.h"
#include "../util/strings.h"

int echo(int argc, char *argv[])
{
	for(int i = 1; i < argc; i++)
	{
		printf(argv[i]);
		if(i != argc)putchar(' ');
	}
	return(0);
}

int printftest(int argc, char *argv[])
{
	if(argc > 1 && (strcmp(argv[1], "s") || strcmp(argv[1], "all"))){
		printf("%%s: <%s>\n", "no pad");
		printf("%%20s: <%20s>\n", "pad left 20");
		printf("%%-20s: <%-20s>\n", "pad right 20");
		printf("%%*s: <%*s>\n", 20, "pad left arg 20");
		printf("%%-*s: <%-*s>\n", 20, "pad right arg 20");
	}
	
	if(argc > 1 && (strcmp(argv[1], "d") || strcmp(argv[1], "all")))
	{
		printf("%%d: <%d>  %%10d: <%10d>\n", -100, -100);
		printf("%%010d: <%010d>\n", -100);
		printf("%%+d: <%+d>,%% d: <% d>\n", 100, 100);
		printf("%%u <%u>\n", -100);
	}
	if(argc > 1 && (strcmp(argv[1], "c") || strcmp(argv[1], "all")))printf("%%c: <%c>\n", 'c');
	
	if(argc > 1 && (strcmp(argv[1], "x") || strcmp(argv[1], "all")))
	{
		printf("%%x: <%x>\n", 0xdeadbeef);
		printf("%%10x: <%10x>\n", 0xdeadbeef);
		printf("%%-10x: <%-10x>\n", 0xdeadbeef);
		printf("%%010x: <%010x>\n", 0xdeadbeef);
		printf("%%#015x: <%#015x>", 0xdeadbeef);
	}
	
	if(argc > 1 && (strcmp(argv[1], "X") || strcmp(argv[1], "all")))
	{
		printf("%%X: <%X>\n", 0xdeadbeef);
		printf("%%10X: <%10X>\n", 0xdeadbeef);
		printf("%%-10X: <%-10X>\n", 0xdeadbeef);
		printf("%%010X: <%010X>\n", 0xdeadbeef);
		printf("%%#015X: <%#015X>", 0xdeadbeef);
	}
	return(0);
}







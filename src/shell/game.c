#include "game.h"

#include "../keyboard.h"
#include "../timer.h"
#include "../util/math.h"
#include "../screen.h"
#include "../graphics.h"
#include "clear.h"
#include "../system.h"
#include "../util/strings.h"

#define SCALE_FACTOR 1

int BOARD_WIDTH = 0;
int BOARD_HEIGHT = 0;
#define CELL_HEIGHT 10
#define BORDER_WIDTH 1

int NUM_MINES = 0;
#define UNKNOWN_COLOUR COLOR(2, 2, 1)
#define UNCOVERED_COLOUR COLOR(0, 7, 0)
#define MINE_COLOUR COLOR(7, 0, 0)
#define BORDER_COLOUR COLOR(0, 0, 0)
#define SELECTED_BORDER_COLOUR COLOR(7, 0, 0)


struct tile
{
	bool isMine;
	bool isFlagged;
	bool isUncovered;
	uint8_t surrounding;
};

int selectedX;
int selectedY;

struct tile board[30][16];
int found;
int flagged;

static const uint8_t numbers[8][8] = {
	{ 0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00},   // U+0031 (1)
	{ 0x1E, 0x33, 0x30, 0x1C, 0x06, 0x33, 0x3F, 0x00},   // U+0032 (2)
	{ 0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E, 0x00},   // U+0033 (3)
	{ 0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78, 0x00},   // U+0034 (4)
	{ 0x3F, 0x03, 0x1F, 0x30, 0x30, 0x33, 0x1E, 0x00},   // U+0035 (5)
	{ 0x1C, 0x06, 0x03, 0x1F, 0x33, 0x33, 0x1E, 0x00},   // U+0036 (6)
	{ 0x3F, 0x33, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x00},   // U+0037 (7)
	{ 0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E, 0x00},   // U+0038 (8)
};

static const uint8_t flag[8] = {16, 24, 28, 30, 16, 16, 16, 255};

void uncover_recursive(int x, int y)
{
	if(board[x][y].isUncovered)return;
	board[x][y].isFlagged = false;
	board[x][y].isUncovered = true;
	found++;
	
	if(board[x][y].surrounding > 0)return;
	
	
	
	if(x != 0 && y != 0)uncover_recursive(x-1, y-1);
	if(x != 0)uncover_recursive(x-1, y);
	if(x != 0 && y != BOARD_HEIGHT - 1)uncover_recursive(x-1, y+1);
	if(y != BOARD_HEIGHT - 1)uncover_recursive(x, y+1);
	if(x != BOARD_WIDTH - 1 && y != BOARD_HEIGHT - 1)uncover_recursive(x+1, y+1);
	if(x != BOARD_WIDTH - 1)uncover_recursive(x+1, y);
	if(x != BOARD_WIDTH - 1 && y != 0)uncover_recursive(x+1, y-1);
	if(y != 0)uncover_recursive(x, y-1);
}

char numberBuffer[35];

void render_board()
{
	for(int x = 0; x < BOARD_WIDTH; x++)
	{
		for(int y = 0; y < BOARD_HEIGHT; y++)
		{
			uint8_t backgroundColour;
			
			if(board[x][y].isUncovered){
				backgroundColour = board[x][y].isMine ? MINE_COLOUR : UNCOVERED_COLOUR;
			}
			else backgroundColour = UNKNOWN_COLOUR;
			
			draw_rect(x * CELL_HEIGHT * SCALE_FACTOR, y * CELL_HEIGHT * SCALE_FACTOR, CELL_HEIGHT * SCALE_FACTOR, CELL_HEIGHT * SCALE_FACTOR, (x == selectedX && y == selectedY) ?SELECTED_BORDER_COLOUR : BORDER_COLOUR);
			draw_rect(x * CELL_HEIGHT * SCALE_FACTOR + BORDER_WIDTH * SCALE_FACTOR, y * CELL_HEIGHT * SCALE_FACTOR + BORDER_WIDTH * SCALE_FACTOR, CELL_HEIGHT * SCALE_FACTOR - BORDER_WIDTH * SCALE_FACTOR * 2, CELL_HEIGHT * SCALE_FACTOR - BORDER_WIDTH * SCALE_FACTOR * 2, backgroundColour);
			
			if(!board[x][y].isMine && board[x][y].isUncovered && board[x][y].surrounding > 0)
			{
				for(int pixelX = 0; pixelX < 8; pixelX++)
				{
					for(int pixelY = 0; pixelY < 8; pixelY++)
					{
						if(numbers[board[x][y].surrounding-1][pixelY]&(1<<pixelX))
						{
							draw_rect(x * CELL_HEIGHT * SCALE_FACTOR + BORDER_WIDTH * SCALE_FACTOR + pixelX * SCALE_FACTOR, y * CELL_HEIGHT * SCALE_FACTOR + BORDER_WIDTH * SCALE_FACTOR + pixelY * SCALE_FACTOR, SCALE_FACTOR, SCALE_FACTOR, COLOR(7, 7, 3));
						}
					}
				}
			}
			if(board[x][y].isFlagged)
			{
				for(int pixelX = 0; pixelX < 8; pixelX++)
				{
					for(int pixelY = 0; pixelY < 8; pixelY++)
					{
						if(flag[pixelY]&(1<<(8-pixelX)))
						{
							draw_rect(x * CELL_HEIGHT * SCALE_FACTOR + BORDER_WIDTH * SCALE_FACTOR + pixelX * SCALE_FACTOR, y * CELL_HEIGHT * SCALE_FACTOR + BORDER_WIDTH * SCALE_FACTOR + pixelY * SCALE_FACTOR, SCALE_FACTOR, SCALE_FACTOR, COLOR(7, 7, 3));
						}
					}
				}
			}
		}
	}
	
	itoa(flagged, numberBuffer, 30);
	int length = strlen(numberBuffer);
	numberBuffer[length] = ' ';
	numberBuffer[length + 1] = '/';
	numberBuffer[length + 2] = ' ';
	itoa(NUM_MINES, numberBuffer + length + 3, 30);
	draw_text(numberBuffer, 0, BOARD_HEIGHT * CELL_HEIGHT * SCALE_FACTOR, COLOR(7, 7, 3), COLOR(0, 0, 0));
}



int mines(int argc, char *argv[])
{	
	if(argc == 1)
	{
		puts("Specify difficulty: easy, medium or hard.");
		return(1);
	}

	if(strcmp(argv[1], "easy"))
	{
			BOARD_WIDTH = 8;
			BOARD_HEIGHT = 8;
			NUM_MINES = 10;
	}
	else if(strcmp(argv[1], "medium"))
	{
			BOARD_WIDTH = 16;
			BOARD_HEIGHT = 16;
			NUM_MINES = 40;
	}
	else if(strcmp(argv[1], "hard"))
	{
			BOARD_WIDTH = 30;
			BOARD_HEIGHT = 16;
			NUM_MINES = 99;
	}
	else
	{
		puts("Invalid difficulty '");
		puts(argv[1]);
		putchar('\'');
		return(1);
	}
	

	bool playagain = true;
	
	do{
		found = 0;
		flagged = 0;
		bool first_click = true;
		clear_screen(0);
		for(int x = 0; x < BOARD_WIDTH; x++)
		{
			for(int y = 0; y < BOARD_HEIGHT; y++)
			{
				board[x][y].isMine = false;
				board[x][y].isFlagged = false;
				board[x][y].isUncovered = false;
				board[x][y].surrounding = 0;
			}
		}

		selectedX = 0;
		selectedY = 0;
		render_board();
		screen_swap();
		bool gameOver = false;
		bool gameWon = false;
		while(!gameOver)
		{
			render_board();
			screen_swap();
			if(keybuffer.start != keybuffer.end)
			{
				if(keybuffer.buffer[keybuffer.start] == 0)
				{
					switch(keybuffer.buffer[keybuffer.start + 1])
					{
						//left arrow
						case(1):
							if(selectedX == 0)selectedX = BOARD_WIDTH - 1;
							else selectedX--;
							break;
						//right arrow
						case(2):
							if(selectedX == BOARD_WIDTH - 1)selectedX = 0;
							else selectedX++;
							break;
						//up arrow
						case(3):
							if(selectedY == 0)selectedY = BOARD_HEIGHT - 1;
							else selectedY--;
							break;
						//down arrow
						case(4):
							if(selectedY == BOARD_HEIGHT - 1)selectedY = 0;
							else selectedY++;
							break;
					}
					keybuffer.start++;
				}
				
				else if(keybuffer.buffer[keybuffer.start] == '\n')
				{
					if(first_click)
					{
						for(int i = 0; i < NUM_MINES;)
						{
							int x = rand() % BOARD_WIDTH;
							int y = rand() % BOARD_HEIGHT;
							if(board[x][y].isMine || (abs(x - selectedX) <= 1 && abs(y - selectedY) <= 1))continue;
							board[x][y].isMine = true;
							i++;
						}

						for(int x = 0; x < BOARD_WIDTH; x++)
						{
							for(int y = 0; y < BOARD_HEIGHT; y++)
							{
								if(x != 0 && y != 0 && board[x-1][y-1].isMine)board[x][y].surrounding++;
								if(x != 0 && board[x-1][y].isMine)board[x][y].surrounding++;
								if(x != 0 && y != BOARD_HEIGHT - 1 && board[x-1][y+1].isMine)board[x][y].surrounding++;
								if(y != BOARD_HEIGHT - 1 && board[x][y+1].isMine)board[x][y].surrounding++;
								if(x != BOARD_WIDTH - 1 && y != BOARD_HEIGHT - 1 && board[x+1][y+1].isMine)board[x][y].surrounding++;
								if(x != BOARD_WIDTH - 1 && board[x+1][y].isMine)board[x][y].surrounding++;
								if(x != BOARD_WIDTH - 1 && y != 0 && board[x+1][y-1].isMine)board[x][y].surrounding++;
								if(y != 0 && board[x][y-1].isMine)board[x][y].surrounding++;
						
							}
						}
					
						first_click = false;
					}
					if(!board[selectedX][selectedY].isFlagged)
					{
						if(board[selectedX][selectedY].isMine)
						{
							
							gameOver = true;
							for(int x = 0; x < BOARD_WIDTH; x++)
							{
								for(int y = 0; y < BOARD_HEIGHT; y++)
								{
									board[x][y].isUncovered |= board[x][y].isMine;
								}
							}
						}
						else{
							uncover_recursive(selectedX, selectedY);
							if(found == BOARD_HEIGHT * BOARD_WIDTH - NUM_MINES)
							{
								gameWon = true;
								gameOver = true;
							}
						}
					}
				}
				else if(keybuffer.buffer[keybuffer.start] == 'f')
				{
					if(!board[selectedX][selectedY].isUncovered)
					{
						board[selectedX][selectedY].isFlagged = !board[selectedX][selectedY].isFlagged;
						if(board[selectedX][selectedY].isFlagged)flagged++;
						else flagged--;
					}
				}
				keybuffer.start++;
			}
			
		}
		render_board();
		if(!gameWon)
		{
			draw_text("You Lose :(", 0, BOARD_HEIGHT * CELL_HEIGHT + 10, COLOR(7, 7, 3), COLOR(0, 0, 0));
			draw_text("Play again? y/n", 0, BOARD_HEIGHT * CELL_HEIGHT + 20, COLOR(7, 7, 3), COLOR(0, 0, 0));
			screen_swap();
			
		}
		else
		{
			draw_text("You Win :)", 0, BOARD_HEIGHT * CELL_HEIGHT + 10, COLOR(7, 7, 3), COLOR(0, 0, 0));
			draw_text("Play again? y/n", 0, BOARD_HEIGHT * CELL_HEIGHT + 20, COLOR(7, 7, 3), COLOR(0, 0, 0));
			screen_swap();
		}
		for(;;)
			{
				screen_swap();
				if(keybuffer.start != keybuffer.end)
				{
					if(!(keybuffer.buffer[keybuffer.start] == 'y'))
					{
						if(keybuffer.buffer[keybuffer.start] == '\0')keybuffer.start++;
						playagain = false;
					}
					keybuffer.start++;
					break;
				}
			};
	}while(playagain);
	clear(0, NULL);
	return(0);
}

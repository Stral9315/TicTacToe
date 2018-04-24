#pragma once
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <stdio.h>
#include <tictactoe.h>

//Uses the windows console to display
class ConsoleRenderer
{
private:
	int BufferSize; //Number of rows and columns in the buffer. Always square.
	short CharacterSize;
	HANDLE hStdOut;
	
public:
	ConsoleRenderer();
	int Initialize();
	void ClearScreen();
	void DisplayGamestate();
	void PutSymbol(COORD pos, char symbol, WORD colour);
	void PutSymbol(int x, int y, char symbol, unsigned short colour);
	COORD PutString(int x, int y, char *str, unsigned short colour);
	void DrawHorz(int startX, int startY, int len, char symbol, unsigned short colour);
	void DrawVert(int startX, int startY, int len, char symbol, unsigned short colour);
	void DrawGameState(GameState* gs);
};

//Initializes the 'renderer' by setting up the window size
ConsoleRenderer::ConsoleRenderer()
{
	BufferSize = 32;
	CharacterSize = 8;
	hStdOut = NULL; //uninitialized
}

void ConsoleRenderer::ClearScreen()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = {0, 0};


	/* Get the number of cells in the current buffer */
	if( !GetConsoleScreenBufferInfo(hStdOut, &csbi) ) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if( !FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR) ' ',
		cellCount,
		homeCoords,
		&count
	) ) return;

	/* Fill the entire buffer with the current colors and attributes */
	if( !FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
	) ) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
}

//Initialize ConsoleRenderer
//Returns non-zero on failure
int ConsoleRenderer::Initialize()
{
	SMALL_RECT conSize = { 0, 0, 1, 1 };
	COORD conBufferSize = { BufferSize, BufferSize };
	CONSOLE_SCREEN_BUFFER_INFO ScreenBufferInfo;
	CONSOLE_FONT_INFOEX fontInfo;
	fontInfo.nFont = 1;
	fontInfo.dwFontSize = { CharacterSize, CharacterSize };
	wcscpy(fontInfo.FaceName, L"Terminal");
	fontInfo.FontFamily = FF_DONTCARE;
	fontInfo.FontWeight = FW_NORMAL;
	fontInfo.cbSize = sizeof(fontInfo);

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if( hStdOut == INVALID_HANDLE_VALUE )
		return -1;

	SetCurrentConsoleFontEx(hStdOut, FALSE, &fontInfo);
	SetConsoleWindowInfo(hStdOut, TRUE, &conSize); //Set the window to be small temporarily
	SetConsoleScreenBufferSize(hStdOut, conBufferSize); //Increase our buffer size while the window is small
	GetConsoleScreenBufferInfo(hStdOut, &ScreenBufferInfo); //Find the max window size for the new buffer size
	conSize.Bottom = ScreenBufferInfo.dwMaximumWindowSize.Y - 1;
	conSize.Right = ScreenBufferInfo.dwMaximumWindowSize.X - 1;
	SetConsoleWindowInfo(hStdOut, TRUE, &conSize); //Set that max window size
	SetConsoleMode(hStdOut, ENABLE_MOUSE_INPUT | ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS); //Precautionary

	return 0;
}


//pos: position to write to
//symbol: character to write
//colour: colour of character
void ConsoleRenderer::PutSymbol(COORD pos, char symbol, WORD colour)
{
	DWORD size = 1;
	WriteConsoleOutputAttribute(hStdOut, &colour, size, pos, &size);
	WriteConsoleOutputCharacter(hStdOut, &symbol, size, pos, &size);
}

void ConsoleRenderer::PutSymbol(int x, int y, char symbol, unsigned short colour)
{
	COORD pos = {x, y};
	PutSymbol(pos, symbol, (WORD)colour);
}

//x, y: starting position
//every \n encountered will wrap to the next line, restarting at x
//returns ending position
COORD ConsoleRenderer::PutString(int x, int y, char *str, unsigned short colour)
{
	char *p = str;
	COORD pos = {x, y};
	while( *p != '\0' )
	{
		if( *p == '\n' || pos.X > 32) //wrap to next row
		{
			pos.Y++;
			pos.X = x; //starting x
		}
		else
		{
			if( *p != '/') //do not overwrite anything here
				PutSymbol(pos, *p, WORD(colour));
			pos.X++;
		}
		

		p++;
	}
	return pos;
}

void ConsoleRenderer::DrawHorz(int startX, int startY, int len, char symbol, unsigned short colour)
{
	for( int i = 0; i < len; i++ )
	{
		PutSymbol(startX + i, startY, symbol, (WORD)colour);
	}
}

void ConsoleRenderer::DrawVert(int startX, int startY, int len, char symbol, unsigned short colour)
{
	for( int i = 0; i < len; i++ )
	{
		PutSymbol(startX, startY+i, symbol, (WORD)colour);
	}
}

void ConsoleRenderer::DrawGameState(GameState* gs)
{
	//Offset in cells from top left to draw everything
	int offset = 2;

	//Draw dividing lines
	for( int i = 1; i <= 2; i++ )
	{

		DrawVert(offset + (9 * i), offset + 1, 8 * 3 + 2, 178, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
		DrawHorz(offset + 1, offset + (9 * i), 8 * 3 + 2, 178, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
	}
	PlayerBase *p1, *p2;
	p1 = gs->GetPlayer(0);
	p2 = gs->GetPlayer(1);
	//Draw tiles
	for( int col = 0; col < 3; col++ )
	{
		for( int row = 0; row < 3; row++ )
		{
			int x, y;
			x = offset+1 + col * 9;
			y = offset+1 + row * 9;
			Tile tile = gs->boardState.AtTile(col, row);




			switch( tile )
			{
			case Tile::Blank:
				PutString(x, y, "        \n        \n        \n        \n        \n        \n        \n        ", FOREGROUND_GREEN);
				break;
			case Tile::Cross:
				PutString(x, y, "X     X \n X   X  \n  X X   \n   X    \n  X X   \n X   X  \nX     X \n        ", FOREGROUND_RED);
				break;
			case Tile::Nought:
				PutString(x, y, "        \n OOOOOO \n O    O \n O    O \n O    O \n O    O \n OOOOOO \n        ", FOREGROUND_BLUE);
				break;
			}

			bool p1Sel = (p1->x == col && p1->y == row);
			bool p2Sel = (p2->x == col && p2->y == row);
			WORD col = 0;
			col = p1Sel ? col | FOREGROUND_BLUE : col;
			col = p2Sel ? col | FOREGROUND_RED : col;

			if( p1Sel || p2Sel )
			{
				if( gs->time % 15 > 7 )
					PutString(x, y, "SSSSSSSS\nSSSSSSSS\nSSSSSSSS\nSSSSSSSS\nSSSSSSSS\nSSSSSSSS\nSSSSSSSS\nSSSSSSSS", col);
			}
		}
	}
	COORD app;
	switch( gs->phase )
	{
	case Phase::Playing:
		app = PutString(offset, 0, "TURN: ", FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
		if( gs->boardState.PlayerIsCurrent(p1) )
		{
			app = PutString(app.X, app.Y, p1->name, FOREGROUND_BLUE);
		}
		else if( gs->boardState.PlayerIsCurrent(p2) )
		{
			app = PutString(app.X, app.Y, p2->name, FOREGROUND_RED);
		}
		else
		{
			app = PutString(app.X, app.Y, "?????????", FOREGROUND_RED | FOREGROUND_BLUE); //just to alert if this somehow happens
		}
		break;
	case Phase::Intermission:
		if( gs->GetWinner() == nullptr )
		{
			app = PutString(offset, 0, "DRAW.....", FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN );
		}
		else if( p1 == gs->GetWinner() )
		{
			app = PutString(offset, 0, "YOU WON!!!", FOREGROUND_BLUE);
		}
		else if( p2 == gs->GetWinner() )
		{
			app = PutString(offset, 0, "YOU LOSE...", FOREGROUND_RED);
		}
	}

	

}
#include <PlayerConsole.h>
#include <conio.h>

Input PlayerConsole::ProcessInput()
{
	Input input;
	input.action = false;
	input.x = 0;
	input.y = 0;
	char c = 0;
	while( kbhit() )
	{
		c = getch();
		switch( c )
		{
		case 'a':
			input.x = -1;
			break;
		case 'd':
			input.x = 1;
			break;
		case 'w':
			input.y = -1;
			break;
		case 's':
			input.y = 1;
			break;
		case 'f':
			input.action = true;
			break;
		}
	}


	return input;
}
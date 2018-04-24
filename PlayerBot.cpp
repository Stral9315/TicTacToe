#include <PlayerBot.h>
#include <time.h>
#include <stdlib.h>


Input PlayerBot::ProcessInput()
{
	Input input;
	input.action = false;
	if(gs->time > lastThink + 15)
	{
		input.x = (rand() % 3) - 1;
		input.y = (rand() % 3) - 1;
		lastThink = gs->time;
	}
	else
	{
		input.x = 0;
		input.y = 0;
	}
	if( gs->boardState.PlayerIsCurrent(this) )
		input.action = true;

	return input;
}
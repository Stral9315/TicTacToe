#include <PlayerBot.h>
#include <time.h>
#include <stdlib.h>
#include <algorithm>


Input PlayerBot::ProcessInput()
{
	Input input;
	int a = 0;
	bool iAmCurrent = gs->boardState.PlayerIsCurrent(this);
	input.action = false;

	if( gs->phase == Phase::Intermission )
	{
		input.x = input.y = 0;
		input.action = false;
		return input;
	}

	if( this->x == randomTileX && this->y == randomTileY && iAmCurrent && !needTarget )
	{
		input.x = 0;
		input.y = 0;
		input.action = true;
		needTarget = true;
		return input;
	}
	if( gs->boardState.AtTile(randomTileX, randomTileY) != Tile::Blank )
	{
		needTarget = true;
		randomTileX = (rand() % 3);
		randomTileY = (rand() % 3);
	}
	if( gs->boardState.AtTile(randomTileX, randomTileY) == Tile::Blank )
		needTarget = false;
	if(gs->time > lastThink + 0.7)
	{
		if( iAmCurrent && !needTarget )
		{
			input.x = randomTileX - this->x;
			input.y = randomTileY - this->y;
			input.x = std::clamp(input.x, -1, 1);
			input.y = std::clamp(input.y, -1, 1);
		}
		else //wander
		{
 			input.x = (rand() % 3) - 1;
			input.y = (rand() % 3) - 1;
		}

		lastThink = gs->time;
	}
	else
	{
		input.x = 0;
		input.y = 0;
	}


	return input;
}
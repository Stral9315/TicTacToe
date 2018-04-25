#pragma once
#include <tictactoe.h>
#include <PlayerBase.h>

//Sources input from bot A.I
class PlayerBot : public PlayerBase
{
private:
	GameState *gs;
	double lastThink;
	bool needTarget;
	int randomTileX, randomTileY;
public:
	PlayerBot(GameState *gs) { this->gs = gs; name = "BOT"; lastThink = 0; needTarget = true; randomTileX = 1; randomTileY = 1; };
	Input ProcessInput() override;
};
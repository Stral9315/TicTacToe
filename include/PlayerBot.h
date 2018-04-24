#pragma once
#include <tictactoe.h>
#include <PlayerBase.h>

//Sources input from bot A.I
class PlayerBot : public PlayerBase
{
private:
	GameState *gs;
	int lastThink;
public:
	PlayerBot(GameState *gs) { this->gs = gs; name = "BOT"; lastThink = 0; };
	Input ProcessInput() override;
};
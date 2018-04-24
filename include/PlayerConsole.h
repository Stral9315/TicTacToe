#pragma once
#include <PlayerBase.h>

//Sources input from the console via getkb()
class PlayerConsole : public PlayerBase
{
public:
	Input ProcessInput() override;
	PlayerConsole() { name = "HUMAN"; };
};
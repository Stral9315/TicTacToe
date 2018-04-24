#pragma once

//make this more abstract later
struct Input
{
	bool action; //was the continue button pressed?
	int x; //grid x selected
	int y; //grid y selected
};

//Abstract class
class PlayerBase
{
private:
	

public:
	int x, y;
	char *name;
	PlayerBase() { x = 0; y = 0; name = nullptr; };
	virtual Input ProcessInput() = 0;
};
#pragma once
#include <PlayerBase.h>

enum Tile { Blank, Cross, Nought, Err };

struct Pos
{
	int x, y;
};

class Turn 
{
	PlayerBase *player;
	int x;
	int y;
public:
	int GetX() const { return this->x; };
	int GetY() const { return this->y; }; //not sure if I should do this or just make x/y public... or just make turn a struct
	Turn(int x, int y, PlayerBase *ply) { this->x = x; this->y = y; this->player = ply; };
	PlayerBase *GetPlayer() const { return player; };
};

struct Chain
{
	Pos pos[3];
	int len;
};


struct TurnResult
{
	enum ResultType { Continue, Winner, Draw } result;
	Chain chain;
	//PlayerBase *player; //Actually not necessary
};

//For giving warnings to the player
enum TurnValidity
{
	Valid,				//Turn is OK to consume with Advance()
	PositionInvalid,	//Tried to place piece on a non-blank tile
	TurnInvalid			//Tried to place a piece without turn
};

//Consumes Turns and changes its state
class BoardState
{
private:
	int playedTiles;
	PlayerBase* current, *ply1, *ply2; //Current turn taker
	Tile board[3][3];
public:
	PlayerBase* SwapTurn();
	bool PlayerIsCurrent(PlayerBase* ply);
	void Clear();
	void Reset();
	BoardState();
	int Init(PlayerBase *p1, PlayerBase *p2);
	TurnValidity CheckTurn(const Turn* in);
	TurnResult Advance(const Turn* in);
	Tile AtTile(int col, int row);
	Chain Walk(int startX, int startY, int strideX, int strideY, Tile valid);
};

enum Phase
{
	Playing,		//
	Intermission	//Game is over
};

class GameState
{
	PlayerBase* players[2];
	PlayerBase* winner;

public:
	Chain winningChain;
	double time;
	Phase phase;
	BoardState boardState;

	GameState();
	void RunFrame(const double *delta);
	PlayerBase* GetPlayer(const int _idx);
	PlayerBase* GetWinner();
};
#include <tictactoe.h>
#include <PlayerConsole.h>
#include <PlayerBot.h>
#include <algorithm> 

GameState::GameState()
{
	boardState = BoardState();

	time = 0;
	players[0] = new PlayerConsole();
	players[1] = new PlayerBot(this);
	phase = Playing;
	winner = nullptr;
	winningChain = {0};


	boardState.Init(players[0], players[1]);
}

PlayerBase* GameState::GetPlayer(const int _idx)
{
	return this->players[_idx];
}

PlayerBase* GameState::GetWinner()
{
	return winner;
}

void BoardState::Clear()
{
	for( int i = 0; i < 3; i++ )
	{
		for( int j = 0; j < 3; j++ )
		{
			board[i][j] = Tile::Blank;
		}
	}
}

void BoardState::Reset()
{
	playedTiles = 0;
	Clear();
}

BoardState::BoardState()
{
	Reset();
}


int BoardState::Init(PlayerBase *p1, PlayerBase *p2)
{
	ply1 = p1;
	ply2 = p2;
	current = p1;

	return 0;
}

bool BoardState::PlayerIsCurrent(PlayerBase* ply)
{
	bool a = (ply == current);
	return a;
}


TurnValidity BoardState::CheckTurn(const Turn *in)
{
	if( !PlayerIsCurrent(in->GetPlayer()) )
	{
		return TurnValidity::TurnInvalid;
	}
	if( AtTile(in->GetX(), in->GetY()) == Tile::Blank )
	{
		return TurnValidity::Valid;
	}
	else
	{
		return TurnValidity::PositionInvalid;
	}

}

Chain BoardState::Walk(int startX, int startY, int strideX, int strideY, Tile valid)
{
	Tile current = AtTile(startX, startY);
	Chain ch;
	int chain = 0;
	int x = startX;
	int y = startY;
	int i = 0;
	while( current == valid )
	{
		ch.pos[chain].x = x;
		ch.pos[chain].y = y;
		chain++;
		if( chain > 2 )
			break;
		x += strideX;
		y += strideY;
		current = AtTile(x, y);
		if( x == startX && y == startY )
			break;

	}

	ch.len = chain;
	return ch;
}


TurnResult BoardState::Advance(const Turn *in)
{
	TurnResult res;
	int x, y;
	res.result = TurnResult::ResultType::Continue;
	res.chain.len = 0;

	Tile placed = in->GetPlayer() == ply1 ? Tile::Nought : Tile::Cross;
	x = in->GetX();
	y = in->GetY();

	board[x][y] = placed;

	playedTiles++;
	Chain diag1, diag2, horz, vert;
	bool win = false;
	diag1 = Walk(0, 2, 1, -1, placed);
	diag2 = Walk(2, 2, -1, -1, placed);
	horz = Walk(0, y, 1, 0, placed);
	vert = Walk(x, 0, 0, 1, placed);
	if( win = (diag1.len == 3) )
		res.chain = diag1;
	else if( win = (diag2.len == 3) )
		res.chain = diag2;
	else if( win = (horz.len == 3) )
		res.chain = horz;
	else if( win = (vert.len == 3) )
		res.chain = vert;

	if(win)
	{
		res.result = TurnResult::ResultType::Winner;
	}
	else if( playedTiles >= 9 )
	{
		res.result = TurnResult::ResultType::Draw;
	}

	SwapTurn();

	return res;
}


//return what tile is at col/row
Tile BoardState::AtTile(int col, int row)
{
	if( col < 0 || col > 2 || row < 0 || row > 2 )
		return Tile::Err;
	return board[col][row];
}

PlayerBase* BoardState::SwapTurn()
{
	if( current == ply1 )
		current = ply2;
	else
		current = ply1;

	return current;
}


void GameState::RunFrame(const double *delta)
{
	time += *delta;

	//Loop through players

		for( int i = 0; i < 2; i++ )
		{
			int projX, projY;
			PlayerBase* ply = players[i];
			Input input = ply->ProcessInput();
			projX = ply->x + input.x;
			projY = ply->y + input.y;
			//clamp to bounds
			projX = std::clamp(projX, 0, 2);
			projY = std::clamp(projY, 0, 2);
			ply->x = projX;
			ply->y = projY;
			const Turn turn = Turn(projX, projY, ply);
			switch( phase )
			{
				case Phase::Playing:
					if( input.action ) //action was pressed
					{
						TurnValidity valid;
						valid = boardState.CheckTurn(&turn);
						if( valid == TurnValidity::Valid )
						{
							TurnResult res;
							res = boardState.Advance(&turn);
							switch( res.result )
							{
							case TurnResult::ResultType::Continue:
								winner = nullptr;
								break;
							case TurnResult::ResultType::Winner:
								winner = ply;
								winningChain = res.chain;
								phase = Phase::Intermission;
								break;
							case TurnResult::ResultType::Draw:
								winner = nullptr;
								phase = Phase::Intermission;
								break;
							}
						}
					}
					break;
				case Phase::Intermission:
					if( input.action )
					{
						phase = Phase::Playing;
						winningChain = {0};
						boardState.Reset();
					}
					break;
			}
		}

}
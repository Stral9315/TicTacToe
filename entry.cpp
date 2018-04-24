/************************************************************************
*
*	entry.cpp
*	
*	contains the entry point for the entire program
*
************************************************************************/
#include <stdio.h>
#include <PlayerBase.h>
#include <ConsoleRenderer.h>

int main(int _argc, char** _argv)
{
	ConsoleRenderer renderer = ConsoleRenderer();
	GameState gs = GameState();
	if( renderer.Initialize() != 0 )
		return -1;
	

	while( true )
	{
		gs.RunFrame();
		renderer.DrawGameState(&gs);
	}


	return 0;
}
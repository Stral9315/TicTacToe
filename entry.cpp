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
#include <chrono>


int main(int _argc, char** _argv)
{
	ConsoleRenderer renderer = ConsoleRenderer();
	GameState gs = GameState();
	if( renderer.Initialize() != 0 )
		return -1;
	
	std::chrono::duration<double> duration;
	double frametime = 0;
	while( true )
	{
		auto start = std::chrono::system_clock::now();
		gs.RunFrame(&frametime);
		

		
		renderer.DrawGameState(&gs);

		auto end = std::chrono::system_clock::now();
		duration = end - start;
		frametime = duration.count();
	}


	return 0;
}
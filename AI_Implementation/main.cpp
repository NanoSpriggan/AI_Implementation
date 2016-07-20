//
//
// NAME: MAIN.CPP
// DESCRIPTION: Mainline of my AI Implementation
//
//

#include "AI_Object.h"
#include "Pathfinding.h"
#include "SearchCell.h"
#include "Game.h"
#include <iostream>

using namespace std;

PathFinding pfft;
Game *g_game;

//#undef main

int main(int argc, char* argv[]) {
	for(int x = 0; x < 20; x++)
		cout << pfft.ScatterTerrain() << endl;

	g_game = new Game();

	g_game->Init("A* Pathfinding - Dieter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, 0, false);
	while (g_game->isRunning()) {
		g_game->handleEvents();
		g_game->Update();
		g_game->Render();
	}
	g_game->Clean();

	system("pause");
	return 0;
}
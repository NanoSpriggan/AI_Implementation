#if 1

#include "Game.h"
#include <iostream>
#include <Vector3.h>
#include <vector>
#include <time.h>
#include <direct.h>
#include "AI_Object.h"
#include "Pathfinding.h"
#include <Windows.h>
#include <SDL_ttf.h>
#include "Player.h"

int m_currentFrame;
SDL_Rect wall;
AiObject _ai1;
AiObject _ai2;
Player player;
PathFinding *pf = new PathFinding();
Terrain terra;
vector<Terrain *> terrainList;

string str_nextPos;
string str_aiObject;
string str_targetPos;

#define MOVE_RATE 3.0f;

void TestSomeShit(AiObject &ai, Vector3 targetPos);

//$(ProjectDir)

// TODO
// Finite State Machine
// Artifical Agents
// Behaviours
// Terrain stuff

using namespace std;

const int RESET_TIME = 3000;

FILE *fDebug;

#define DEBUGMSG(str) {char charStr[256]; sprintf_s(charStr, "%s\\DEBUG.txt", g_game.getStartDirectory()->c_str());fopen_s(&fDebug, charStr,"at");fprintf(fDebug,str);fclose(fDebug);}

bool Game::Init(const char * title, int xpos, int ypos, int width, int height, int flags, bool fullscreen) {
	char cCurrentPath[FILENAME_MAX];
	setStartDir(cCurrentPath);

	if (fullscreen) flags = SDL_WINDOW_FULLSCREEN;
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		//printf("SDL Init.\n");
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		TTF_Init();
		//printf("SDL Window created.\n");
		m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		//printf("SDL Renderer init.\n");
		SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
		//printf("Init completed.\n");

		// LOAD STUFF HERE

		wall.x = 800;
		wall.y = 10;
		wall.w = 10;
		wall.h = 800;

		player.Load("sprite2.png", "SPRITE", m_pRenderer);

		terra.Load("tile.png", "NORMAL", m_pRenderer);
		terra.Load("water.png", "WATER", m_pRenderer);
		terra.Load("lava.png", "LAVA", m_pRenderer);
		terra.Load("wall.png", "WALL", m_pRenderer);

		_ai1.Load("SPRITE_RED.png", "RED", m_pRenderer);
		_ai1.Load("SPRITE_BLUE.png", "BLUE", m_pRenderer);
		_ai1.Load("SPRITE_GREEN.png", "GREEN", m_pRenderer);

		_ai2.Load("SPRITE_RED.png", "RED", m_pRenderer);
		_ai2.Load("SPRITE_BLUE.png", "BLUE", m_pRenderer);
		_ai2.Load("SPRITE_GREEN.png", "GREEN", m_pRenderer);

		pf->m_initalizedStartGoal = false;
		pf->m_foundGoal = false;

		_ai1.pos = Vector3((float)(rand() % 800), (float)(rand() % 800), 0);
		_ai2.pos = Vector3((float)(rand() % 800), (float)(rand() % 800), 0);

		player.pos = Vector3(400, 400, 0);

		// don't delete this line again dickhead
		m_bRunning = true;
		isPaused = false;
		return true;
	}
	else return false;
}

void Game::Update() {
	m_currentFrame = int(((SDL_GetTicks() / 100) % 12));
	if (!isPaused) {
		// DO STUFF HERE

		TestSomeShit(_ai1, player.pos);
		TestSomeShit(_ai2, player.pos);

		SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	}
}

void Game::Render() {
	SDL_RenderClear(m_pRenderer);

	// DRAW TO SCREEN HERE

	//for (vector<Terrain*>::size_type i = 0; i != terrainList.size(); i++) {
	//	terra.Draw("NORMAL", 1, 1, CELL_SIZE, CELL_SIZE, 0, m_pRenderer, SDL_FLIP_NONE);
	//	terra.Draw("WATER", 41, 1, CELL_SIZE, CELL_SIZE, 0, m_pRenderer, SDL_FLIP_NONE);
	//	terra.Draw("LAVA", 81, 1, CELL_SIZE, CELL_SIZE, 0, m_pRenderer, SDL_FLIP_NONE);
	//	terra.Draw("WALL", 121, 1, CELL_SIZE, CELL_SIZE, 0, m_pRenderer, SDL_FLIP_NONE);
	//}

	/*for (vector<Terrain*>::size_type i = 0; i != terrainList.size(); i++) {
		terra.Draw(terraType, pos, CELL_SIZE, CELL_SIZE, 0, m_pRenderer, SDL_FLIP_NONE);
	}*/

	player.Draw("SPRITE", player.pos, 32, 32, m_currentFrame, 0, m_pRenderer, SDL_FLIP_NONE);

	/*terra.Draw("NORMAL", 0, 0, CELL_SIZE, CELL_SIZE, 0, m_pRenderer, SDL_FLIP_NONE);
	terra.Draw("WATER", 20, 0, CELL_SIZE, CELL_SIZE, 0, m_pRenderer, SDL_FLIP_NONE);
	terra.Draw("LAVA", 40, 0, CELL_SIZE, CELL_SIZE, 0, m_pRenderer, SDL_FLIP_NONE);
	terra.Draw("WALL", 60, 0, CELL_SIZE, CELL_SIZE, 0, m_pRenderer, SDL_FLIP_NONE);*/

	_ai1.Draw("RED", _ai1.pos, CELL_SIZE, CELL_SIZE, 0, m_pRenderer, SDL_FLIP_NONE);
	_ai2.Draw("GREEN", _ai2.pos, CELL_SIZE, CELL_SIZE, 0, m_pRenderer, SDL_FLIP_NONE);

	SDL_RenderPresent(m_pRenderer);
}

void Game::handleEvents() {
	SDL_Event event;
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	Uint32 currentTime = SDL_GetTicks();
	static Uint32 lastTime = 0;

	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			m_bRunning = false;
			break;
		default:
			break;
		}
	}

	if (keys[SDL_SCANCODE_A]) {
		player.pos.x -= MOVE_RATE;
	}
	if (keys[SDL_SCANCODE_D]) {
		player.pos.x += MOVE_RATE;
	}
	if (keys[SDL_SCANCODE_W]) {
		player.pos.y -= MOVE_RATE;
	}
	if (keys[SDL_SCANCODE_S]) {
		player.pos.y += MOVE_RATE;
	}

	if (keys[SDL_SCANCODE_P] && currentTime - lastTime > 1000) {
		SDL_SetRenderDrawColor(m_pRenderer, 100, 100, 255, 255);
		lastTime = currentTime;
		isPaused = !isPaused;
	}

	if (keys[SDL_SCANCODE_1] && currentTime - lastTime > 100) {
		int imX, imY;
		SDL_GetMouseState(&imX, &imY);
	}

	int imX, imY;
	SDL_GetMouseState(&imX, &imY);
	//terra.Draw("WALL", imX, imY, CELL_SIZE, CELL_SIZE, 0, m_pRenderer, SDL_FLIP_NONE);
}

void Game::Clean() {
	printf("Cleaning game.");
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
	printf("Game cleaned. Closing now.");
}

bool Game::isRunning() { return m_bRunning; }

void Game::setStartDir(char *p_str) {
	mStartDir = p_str;
}

void TestSomeShit(AiObject &ai_Object, Vector3 targetPos) {
	if (pf->m_foundGoal == true) {
		Vector3 nextPos = pf->NextPathPos(ai_Object);
		targetPos = player.pos;
		Vector3 targetPosPrev = targetPos;

		cout << "ai object: " << ai_Object.pos.x << " , " << ai_Object.pos.y << endl;
		cout << "next pos: " << nextPos.x << " , " << nextPos.y << endl;
		cout << "target pos: " << targetPos.x << " , " << targetPos.y << endl << endl;

		if (nextPos.x > ai_Object.pos.x) {
			ai_Object.pos.x += MOVE_RATE;
		}
		else if (nextPos.x < ai_Object.pos.x) {
			ai_Object.pos.x -= MOVE_RATE;
		}
		if (nextPos.y > ai_Object.pos.y) {
			ai_Object.pos.y += MOVE_RATE;
		}
		else if (nextPos.y < ai_Object.pos.y) {
			ai_Object.pos.y -= MOVE_RATE;
		}

		//targetPos = player.pos;
		Vector3 distToTarget = targetPos - Vector3(ai_Object.pos.x, ai_Object.pos.y, 0);
		
		if (floor(distToTarget.magnitude()) < 30) {
			

			pf->m_initalizedStartGoal = false;
			pf->m_foundGoal = false;
			pf->ClearOpenList();
			pf->ClearPathToGoal();
			pf->ClearVisitedList();
			TestSomeShit(ai_Object, targetPos);
		}
	}
	else {
		cout << "FIND PATH" << endl;
		pf->FindPath(ai_Object.pos, targetPos);
	}
}

#endif
#if 0

#include "Game.h"
#include <iostream>
#include <Vector3.h>
#include <vector>
#include <time.h>
#include <direct.h>
#include "AI_Object.h"
#include "Pathfinding.h"
#include <Windows.h>
#include <SDL_ttf.h>

int m_currentFrame;
SDL_Rect wall;
AiObject _ai1;
AiObject _ai2;
PathFinding *pf = new PathFinding();
Terrain terra;
vector<Terrain *> terrainList;

string str_nextPos;
string str_aiObject;
string str_targetPos;

#define MOVE_RATE 3.0f;

void TestSomeShit(AiObject &ai, Vector3 targetPos);

//$(ProjectDir)

// TODO
// Finite State Machine
// Artifical Agents
// Behaviours
// Terrain stuff

using namespace std;

const int RESET_TIME = 3000;

FILE *fDebug;

#define DEBUGMSG(str) {char charStr[256]; sprintf_s(charStr, "%s\\DEBUG.txt", g_game.getStartDirectory()->c_str());fopen_s(&fDebug, charStr,"at");fprintf(fDebug,str);fclose(fDebug);}

bool Game::Init(const char * title, int xpos, int ypos, int width, int height, int flags, bool fullscreen) {
	char cCurrentPath[FILENAME_MAX];
	setStartDir(cCurrentPath);

	if (fullscreen) flags = SDL_WINDOW_FULLSCREEN;
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		//printf("SDL Init.\n");
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		TTF_Init();
		//printf("SDL Window created.\n");
		m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		//printf("SDL Renderer init.\n");
		SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
		//printf("Init completed.\n");

		// LOAD STUFF HERE

		wall.x = 800;
		wall.y = 10;
		wall.w = 10;
		wall.h = 800;

		terra.Load("tile.png", "NORMAL", m_pRenderer);
		terra.Load("water.png", "WATER", m_pRenderer);
		terra.Load("lava.png", "LAVA", m_pRenderer);
		terra.Load("wall.png", "WALL", m_pRenderer);

		_ai1.Load("SPRITE_RED.png", "RED", m_pRenderer);
		_ai1.Load("SPRITE_BLUE.png", "BLUE", m_pRenderer);
		_ai1.Load("SPRITE_GREEN.png", "GREEN", m_pRenderer);

		_ai2.Load("SPRITE_RED.png", "RED", m_pRenderer);
		_ai2.Load("SPRITE_BLUE.png", "BLUE", m_pRenderer);
		_ai2.Load("SPRITE_GREEN.png", "GREEN", m_pRenderer);

		pf->m_initalizedStartGoal = false;
		pf->m_foundGoal = false;

		_ai1.pos = Vector3(0, 0, 0);
		_ai2.pos = Vector3((float)(rand() % 800), (float)(rand() % 800), 0);

		// don't delete this line again dickhead
		m_bRunning = true;
		isPaused = false;
		return true;
	}
	else return false;
}

void Game::Update() {
	m_currentFrame = int(((SDL_GetTicks() / 100) % 6));
	if (!isPaused) {
		// DO STUFF HERE

		TestSomeShit(_ai1, _ai2.pos);

		SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	}
}

void Game::Render() {
	SDL_RenderClear(m_pRenderer);

	// DRAW TO SCREEN HERE

	//for (vector<Terrain*>::size_type i = 0; i != terrainList.size(); i++) {
	//	terra.Draw("NORMAL", 1, 1, CELL_SIZE, CELL_SIZE, 0, m_pRenderer, SDL_FLIP_NONE);
	//	terra.Draw("WATER", 41, 1, CELL_SIZE, CELL_SIZE, 0, m_pRenderer, SDL_FLIP_NONE);
	//	terra.Draw("LAVA", 81, 1, CELL_SIZE, CELL_SIZE, 0, m_pRenderer, SDL_FLIP_NONE);
	//	terra.Draw("WALL", 121, 1, CELL_SIZE, CELL_SIZE, 0, m_pRenderer, SDL_FLIP_NONE);
	//}

	terra.Draw("NORMAL", 0, 0, CELL_SIZE, CELL_SIZE, 0, m_pRenderer, SDL_FLIP_NONE);
	terra.Draw("WATER", 20, 0, CELL_SIZE, CELL_SIZE, 0, m_pRenderer, SDL_FLIP_NONE);
	terra.Draw("LAVA", 40, 0, CELL_SIZE, CELL_SIZE, 0, m_pRenderer, SDL_FLIP_NONE);
	terra.Draw("WALL", 60, 0, CELL_SIZE, CELL_SIZE, 0, m_pRenderer, SDL_FLIP_NONE);

	_ai1.Draw("RED", _ai1.pos, CELL_SIZE, CELL_SIZE, 0, m_pRenderer, SDL_FLIP_NONE);
	_ai2.Draw("GREEN", _ai2.pos, CELL_SIZE, CELL_SIZE, 0, m_pRenderer, SDL_FLIP_NONE);

	//_ai1.Draw_Text(m_pRenderer, str_aiObject, 14, 1, 1); // I'll work on it later, not really needed all that much

	SDL_RenderPresent(m_pRenderer);
}

void Game::handleEvents() {
	SDL_Event event;
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	Uint32 currentTime = SDL_GetTicks();
	static Uint32 lastTime = 0;

	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			m_bRunning = false;
			break;
		default:
			break;
		}
	}

	if (keys[SDL_SCANCODE_P] && currentTime - lastTime > 1000) {
		SDL_SetRenderDrawColor(m_pRenderer, 100, 100, 255, 255);
		lastTime = currentTime;
		isPaused = !isPaused;
	}

	if (keys[SDL_SCANCODE_1] && currentTime - lastTime > 100) {
		int imX, imY;
		SDL_GetMouseState(&imX, &imY);
	}

	int imX, imY;
	SDL_GetMouseState(&imX, &imY);
	terra.Draw("WALL", imX, imY, CELL_SIZE, CELL_SIZE, 0, m_pRenderer, SDL_FLIP_NONE);
}

void Game::Clean() {
	printf("Cleaning game.");
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
	printf("Game cleaned. Closing now.");
}

bool Game::isRunning() { return m_bRunning; }

void Game::setStartDir(char *p_str) {
	mStartDir = p_str;
}

void TestSomeShit(AiObject &ai_Object, Vector3 targetPos) {
	if (pf->m_foundGoal == true) {
		Vector3 nextPos = pf->NextPathPos(ai_Object);

		// TEXT ON SCREEN STUFF
		string str_nextPos = "nextPos: " + to_string(nextPos.x) + " , " + to_string(nextPos.y);
		string str_aiObject = "AI Object: " + to_string(ai_Object.pos.x) + " , " + to_string(ai_Object.pos.y);
		string str_targetPos = "Target Pos: " + to_string(targetPos.x) + " , " + to_string(targetPos.y);

		if (nextPos.x > ai_Object.pos.x) {
			ai_Object.pos.x += MOVE_RATE;
		}
		else if (nextPos.x < ai_Object.pos.x) {
			ai_Object.pos.x -= MOVE_RATE;
		}
		if (nextPos.y > ai_Object.pos.y) {
			ai_Object.pos.y += MOVE_RATE;
		}
		else if (nextPos.y < ai_Object.pos.y) {
			ai_Object.pos.y -= MOVE_RATE;
		}

		Vector3 distToTarget = targetPos - Vector3(ai_Object.pos.x, ai_Object.pos.y, 0);

		if (floor(distToTarget.magnitude()) < 30) {
			cout << "ai object: " << ai_Object.pos.x << " , " << ai_Object.pos.y << endl;
			cout << "next pos: " << nextPos.x << " , " << nextPos.y << endl;
			cout << "target pos: " << targetPos.x << " , " << targetPos.y << endl << endl;

			pf->m_initalizedStartGoal = false;
			pf->m_foundGoal = false;
			pf->ClearOpenList();
			pf->ClearPathToGoal();
			pf->ClearVisitedList();
			targetPos = Vector3((float)(rand() % 800), (float)(rand() % 800), 0);
			_ai2.pos = targetPos;
			TestSomeShit(ai_Object, targetPos);
		}

		if (floor(distToTarget.magnitude()) > 100 && SDL_GetTicks()) {
		}
	}
	else {
		//cout << "FIND PATH" << endl;
		pf->FindPath(ai_Object.pos, targetPos);
	}
}

#endif
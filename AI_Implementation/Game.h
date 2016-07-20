#ifndef _GAME_H_
#define _GAME_H_

#include <SDL.h>
#include <string>
#include <SDL_image.h>
#include <SDL_ttf.h>

using namespace std;

class Game {
public:
	bool Init(const char* title, int xpos, int ypos, int width, int height, int flags, bool fullscreen);
	void Render();
	void Update();
	void handleEvents();
	void Clean();
	bool isRunning();
	void setStartDir(char *p_str);
	string* getStartDir() { return &mStartDir; }

private:
	bool m_bRunning;
	SDL_Window *m_pWindow = NULL;
	SDL_Renderer *m_pRenderer = NULL;
	int m_currentFrame;
	bool isPaused;
	string mStartDir;
};

#endif // !_GAME_H_
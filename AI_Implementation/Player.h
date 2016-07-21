#ifndef PLAYER_H
#define PLAYER_H

#include <Matrix3.h>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <map>

using namespace std;

class Player {
public:
	static const int DOT_WIDTH = 20;
	static const int DOT_HEIGHT = 20;
	static const int DOT_VEL = 10;

	Player() {
		pos = { 0, 0, 0 };
		mCollider.h = DOT_WIDTH / 2;
		mCollider.w = DOT_HEIGHT / 2;
		mVelX = 2.5f;
		mVelY = 2.5f;
		ai_Health = 100;
	}

	bool Load(string fileName, int id, SDL_Renderer *pRenderer) {
		SDL_Surface *pTempSurface = IMG_Load(fileName.c_str());
		if (pTempSurface == 0) return false;
		SDL_Texture *pTexture = SDL_CreateTextureFromSurface(pRenderer, pTempSurface);
		SDL_FreeSurface(pTempSurface);
		//SDL_Texture *pTexture = IMG_LoadTexture(pRenderer, fileName.c_str());
		if (pTexture != 0) m_textureMap[id] = pTexture; return true;
		return false;
	}

	void Draw(int id, Vector3 pos, float width, float height, int currentFrame, float rotate, SDL_Renderer *pRenderer, SDL_RendererFlip flip) {
		SDL_Rect srcRect;
		SDL_Rect dstRect;
		srcRect.x = width * currentFrame;
		srcRect.y = 0;
		srcRect.w = dstRect.w = (int)width;
		srcRect.h = dstRect.h = (int)height;
		dstRect.x = (int)pos.x;
		dstRect.y = (int)pos.y;
		SDL_RenderCopyEx(pRenderer, m_textureMap[id], &srcRect, &dstRect, rotate, 0, flip);
	}

	void Update() {

	}

	float radius() { return DOT_HEIGHT + 1; }

	map<int, SDL_Texture*> m_textureMap;

	Vector3 pos;
private:
	//int mPosX, mPosY;
	float mVelX, mVelY;
	SDL_Rect mCollider;
	int ai_Health;
};

#endif // !PLAYER_H
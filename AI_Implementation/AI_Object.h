#ifndef AI_OBJECT_H
#define AI_OBJECT_H

#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <map>
#include <Vector3.h>

using namespace std;

class AiObject {
public:
	static const int DOT_WIDTH = 20;
	static const int DOT_HEIGHT = 20;
	static const int DOT_VEL = 10;

	AiObject() {
		pos = { 0, 0, 0 };
		mCollider.h = DOT_WIDTH / 2;
		mCollider.w = DOT_HEIGHT / 2;
		mVelX = 2.5f;
		mVelY = 2.5f;
		ai_Health = 100;
	}

	bool Load(string fileName, string id, SDL_Renderer *pRenderer) {
		SDL_Surface *pTempSurface = IMG_Load(fileName.c_str());
		if (pTempSurface == 0) return false;
		SDL_Texture *pTexture = SDL_CreateTextureFromSurface(pRenderer, pTempSurface);
		SDL_FreeSurface(pTempSurface);
		//SDL_Texture *pTexture = IMG_LoadTexture(pRenderer, fileName.c_str());
		if (pTexture != 0) m_textureMap[id] = pTexture; return true;
		return false;
	}

	void Draw(string id, Vector3 pos, float width, float height, float rotate, SDL_Renderer *pRenderer, SDL_RendererFlip flip) {
		SDL_Rect srcRect;
		SDL_Rect dstRect;
		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = dstRect.w = (int)width;
		srcRect.h = dstRect.h = (int)height;
		dstRect.x = (int)pos.x;
		dstRect.y = (int)pos.y;
		SDL_RenderCopyEx(pRenderer, m_textureMap[id], &srcRect, &dstRect, rotate, 0, flip);
	}

	//void Draw_Text(SDL_Renderer * m_pRenderer, char * string, int size, int x, int y) {
	//	TTF_Font* font = TTF_OpenFont("", size);
	//	SDL_Color color = { 255, 255, 255, 255 };
	//	SDL_Surface *textSurface = TTF_RenderText_Solid(font, string, color);
	//	SDL_Texture *texTex = SDL_CreateTextureFromSurface(m_pRenderer, textSurface);
	//	SDL_FreeSurface(textSurface);
	//	SDL_Rect textLoc; // = SDL_QueryTexture(texTex, NULL, NULL, &x, &y);
	//	SDL_Rect textToBeLoc;
	//	textLoc.x = 0;
	//	textLoc.y = 0;
	//	textToBeLoc.x = x;
	//	textToBeLoc.y = y;
	//	SDL_RenderCopy(m_pRenderer, texTex, &textLoc, &textToBeLoc);
	//	//TTF_CloseFont();
	//}

	float radius() { return DOT_HEIGHT + 1; }

	map<string, SDL_Texture*> m_textureMap;

	Vector3 pos;
private:
	//int mPosX, mPosY;
	float mVelX, mVelY;
	SDL_Rect mCollider;
	int ai_Health;
};

#endif // !AI_OBJECT_H
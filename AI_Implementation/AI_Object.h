#ifndef AI_OBJECT_H
#define AI_OBJECT_H

#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <map>
#include <Vector3.h>
#include "Agent.h"
#include "State.h"
#include "StateMachine.h"

using namespace std;

class AiObject : public Agent {
public:
	AiObject() { Vector3 pos; }
	AiObject(Vector3 pos) {
		Init(pos);
		m_pStateMachine = new StateMachine();
		m_pStateMachine->ChangeState(this, new Seek());
	}

	~AiObject() {/* delete m_pStateMachine;*/ }

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

	float radius() { return 32; }
	map<string, SDL_Texture*> m_textureMap;

	Vector3 pos;

};

#endif // !AI_OBJECT_H
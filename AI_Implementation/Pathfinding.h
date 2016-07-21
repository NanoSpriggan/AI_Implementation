#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <Vector3.h>
#include <vector>
#include "SearchCell.h"
#include "AI_Object.h"

using namespace std;

enum TerrainType {
	CELL_NORMAL,
	CELL_WATER = 5,
	CELL_LAVA = 10,
	CELL_BLOCKED = 1024
};

class Terrain {
public:
	bool Load(string fileName, string id, SDL_Renderer *pRenderer) {
		SDL_Surface *pTempSurface = IMG_Load(fileName.c_str());
		if (pTempSurface == 0) return false;
		SDL_Texture *pTexture = SDL_CreateTextureFromSurface(pRenderer, pTempSurface);
		SDL_FreeSurface(pTempSurface);
		//SDL_Texture *pTexture = IMG_LoadTexture(pRenderer, fileName.c_str());
		if (pTexture != 0) m_textureMap[id] = pTexture; return true;
		return false;
	}

	void Draw(string id, int x, int y, float width, float height, float rotate, SDL_Renderer *pRenderer, SDL_RendererFlip flip) {
		SDL_Rect srcRect;
		SDL_Rect dstRect;
		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = dstRect.w = (int)width;
		srcRect.h = dstRect.h = (int)height;
		dstRect.x = x;
		dstRect.y = y;
		SDL_RenderCopyEx(pRenderer, m_textureMap[id], &srcRect, &dstRect, rotate, 0, flip);
	}
	Vector3 pos;
	vector<Terrain *> terrainList;
	map<string, SDL_Texture*> m_textureMap;
};

class PathFinding {
public:
	PathFinding();
	~PathFinding();

	TerrainType ScatterTerrain();

	void FindPath(Vector3 currentPos, Vector3 targetPos);
	Vector3 NextPathPos(AiObject ai);
	void ClearOpenList() { m_openList.clear(); }
	void ClearVisitedList() { m_visitedList.clear(); }
	void ClearPathToGoal() { m_pathToGoal.clear(); }
	bool m_initalizedStartGoal;
	bool m_foundGoal;

private:
	void SetStartAndGoal(SearchCell start, SearchCell goal);
	void PathOpened(int m_x, int m_y, float newCost, SearchCell *parent/*, TerrainType terrain*/);
	SearchCell *GetNextCell();
	void ContinuePath();
	SearchCell *m_startCell;
	SearchCell *m_goalCell;
	vector<SearchCell*> m_openList;
	vector<SearchCell*> m_visitedList;
	vector<Vector3*> m_pathToGoal;
};

#endif // !PATHFINDING_H
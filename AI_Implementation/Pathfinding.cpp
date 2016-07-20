#include "Pathfinding.h"

PathFinding::PathFinding() {
	m_initalizedStartGoal = false;
	m_foundGoal = false;
}

PathFinding::~PathFinding() {
}

TerrainType PathFinding::ScatterTerrain() {
	int result = rand() % 4;
	TerrainType terrain;

	//TerrainType terrain = TerrainType(rand() % 4);
	//return terrain;

	/*if (result = 0) { terrain = CELL_NORMAL; }
	if (result = 1) { terrain = CELL_WATER; }
	if (result = 2) { terrain = CELL_LAVA; }
	if (result = 3) { terrain = CELL_BLOCKED; }
*/

	switch (result) {
	case 0:
		terrain = CELL_NORMAL;
		break;
	case 1:
		terrain = CELL_WATER;
		break;
	case 2:
		terrain = CELL_LAVA;
		break;
	case 3:
		terrain = CELL_BLOCKED;
		break;
	default:
		break;
	}

	return terrain;
}

void PathFinding::FindPath(Vector3 currentPos, Vector3 targetPos) {
	if (!m_initalizedStartGoal) {
		for (unsigned int i = 0; i < m_openList.size(); i++) delete m_openList[i];
		m_openList.clear();
		for (unsigned int i = 0; i < m_visitedList.size(); i++) delete m_visitedList[i];
		m_visitedList.clear();
		for (unsigned int i = 0; i < m_pathToGoal.size(); i++) delete m_pathToGoal[i];
		m_pathToGoal.clear();

		// INIT START
		SearchCell start;
		start.m_x_coord = (int)(currentPos.x / CELL_SIZE);
		start.m_y_coord = (int)(currentPos.y / CELL_SIZE);

		// INIT GOAL
		SearchCell goal;
		goal.m_x_coord = (int)(targetPos.x / CELL_SIZE);
		goal.m_y_coord = (int)(targetPos.y / CELL_SIZE);

		SetStartAndGoal(start, goal);
		m_initalizedStartGoal = true;
	}
	if (m_initalizedStartGoal) ContinuePath();
}

Vector3 PathFinding::NextPathPos(AiObject ai) {
	unsigned int index = 1;
	Vector3 nextPos;
	nextPos.x = m_pathToGoal[m_pathToGoal.size() - index]->x;
	nextPos.y = m_pathToGoal[m_pathToGoal.size() - index]->y;

	Vector3 distance = nextPos - ai.pos;
	if (index < m_pathToGoal.size()) {
		if (distance.magnitude() < ai.radius()) {
			m_pathToGoal.erase(m_pathToGoal.end() - index);
		}
	}
	return nextPos;
}

void PathFinding::SetStartAndGoal(SearchCell start, SearchCell goal) {
	m_startCell = new SearchCell(start.m_x_coord, start.m_y_coord, NULL);
	m_goalCell = new SearchCell(goal.m_x_coord, goal.m_y_coord, &goal);
	m_startCell->g = 0;
	m_startCell->h = m_startCell->ManhattanDistance(m_goalCell);
	m_startCell->parent = 0;
	m_openList.push_back(m_startCell);
}

void PathFinding::PathOpened(int m_x, int m_y, float newCost, SearchCell * parent/*, TerrainType terrain*/) {
	/*if(CELL_BLOCKED){
		return;
	}*/

	unsigned int id = m_y * WORLD_SIZE + m_x;
	for (unsigned int i = 0; i < m_visitedList.size(); i++) {
		if (id == m_visitedList[i]->m_id) return;
	}
	SearchCell *newChild = new SearchCell(m_x, m_y, parent);
	newChild->g = newCost;
	newChild->h = parent->ManhattanDistance(m_goalCell);

	for (unsigned int i = 0; i < m_openList.size(); i++) {
		if (id == m_openList[i]->m_id) {
			float newF = newChild->g + newCost + m_openList[i]->h;
			if (m_openList[i]->GetF() > newF) {
				m_openList[i]->g = newChild->g + newCost/* + terrain*/;
				m_openList[i]->parent = newChild;
			}
			else /* if the F is not better */ {
				delete newChild;
				return;
			}
		}
	}
	m_openList.push_back(newChild);
}

SearchCell* PathFinding::GetNextCell() {
	float bestF = 999999.0f;
	unsigned int cellIndex = -1;

	SearchCell *nextCell = NULL;
	for (unsigned int i = 0; i < m_openList.size(); i++)
		if (m_openList[i]->GetF() < bestF) {
			bestF = m_openList[i]->GetF();
			cellIndex = i;
		}
	if (cellIndex >= 0) {
		nextCell = m_openList[cellIndex];
		m_visitedList.push_back(nextCell);
		m_openList.erase(m_openList.begin() + cellIndex);
	}
	return nextCell;
}

void PathFinding::ContinuePath() {
	for (int i = 0; i < 50; i++) {
		if (m_openList.empty()) return;
		SearchCell *currentCell = GetNextCell();
		if (currentCell->m_id == m_goalCell->m_id) {
			m_goalCell->parent = currentCell->parent;
			SearchCell *getPath;
			for (getPath = m_goalCell; getPath != NULL; getPath = getPath->parent) m_pathToGoal.push_back(new Vector3((float)(getPath->m_x_coord * CELL_SIZE), (float)(getPath->m_y_coord * CELL_SIZE), 0));
			m_foundGoal = true;
			return;
		}
		else {
			// right side
			PathOpened(currentCell->m_x_coord + 1, currentCell->m_y_coord, currentCell->g + 1, currentCell);
			// left side
			PathOpened(currentCell->m_x_coord - 1, currentCell->m_y_coord, currentCell->g + 1, currentCell);
			// up
			PathOpened(currentCell->m_x_coord, currentCell->m_y_coord + 1, currentCell->g + 1, currentCell);
			// down
			PathOpened(currentCell->m_x_coord, currentCell->m_y_coord - 1, currentCell->g + 1, currentCell);

			// left-up diagonal
			PathOpened(currentCell->m_x_coord - 1, currentCell->m_y_coord + 1, currentCell->g + 1.414f, currentCell);
			// left down diagonal
			PathOpened(currentCell->m_x_coord - 1, currentCell->m_y_coord - 1, currentCell->g + 1.414f, currentCell);

			// right up diagonal
			PathOpened(currentCell->m_x_coord + 1, currentCell->m_y_coord + 1, currentCell->g + 1.414f, currentCell);
			// right down diagonal
			PathOpened(currentCell->m_x_coord + 1, currentCell->m_y_coord - 1, currentCell->g + 1.414f, currentCell);

			for (unsigned int i = 0; i < m_openList.size(); i++) {
				if (currentCell->m_id == m_openList[i]->m_id) {
					m_openList.erase(m_openList.begin() + 1);
				}
			}
		}
	}
}
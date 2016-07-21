#ifndef _SEARCH_CELL_H_
#define _SEARCH_CELL_H_

#include <math.h>
#define WORLD_SIZE 64
#define CELL_SIZE 20.0f

struct SearchCell {
public:
	int m_x_coord, m_y_coord;
	int m_id;
	SearchCell *parent;
	float g, h, f;

	SearchCell() : parent(0) {}
	SearchCell(int m_x, int m_y, SearchCell *m_parent = 0) : m_x_coord(m_x), m_y_coord(m_y), parent(m_parent), m_id(m_y * WORLD_SIZE + m_x), g(0), h(0) {}
	float GetF() { return g + h; }
	float ManhattanDistance(SearchCell *nodeEnd) {
		float x = (float)(fabs(this->m_x_coord - nodeEnd->m_x_coord));
		float z = (float)(fabs(this->m_y_coord - nodeEnd->m_y_coord));
		return x + z;
	}
};

#endif // !_SEARCH_CELL_H_
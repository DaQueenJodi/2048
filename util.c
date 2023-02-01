#include "util.h"
#include <stdlib.h>
#include <assert.h>

Point POINT(int x, int y)
{
	Point p = {x, y};
	return p;
}

int rand_range(int start, int end) {
	assert(start <= end);
	if (start >= 0) assert(end > 0);
	return rand() % (end - start) + start;
}

inline bool point_cmp(Point a, Point b) {
	return (a.x == b.x && a.y == b.y);
}

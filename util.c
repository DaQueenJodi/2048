#include "util.h"
#include <stdlib.h>
#include <assert.h>

Point POINT(int x, int y)
{
	Point p = {x, y};
	return p;
}

int rand_range(int start, int end)
{
	assert(start < end);
	return rand() % (end - start) + start;
}

bool point_cmp(Point a, Point b) {
	return (a.x == b.x && a.y == b.y);
}

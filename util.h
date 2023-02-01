#ifndef UTIL_H

typedef struct {
	int x;
	int y;
} Point;

Point POINT(int x, int y);
int rand_range(int start, int end);
#include <stdbool.h>
bool point_cmp(Point a, Point b);

#endif // UTIL_H

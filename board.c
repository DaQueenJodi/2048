#include "board.h"
#include <stdlib.h>
#include <string.h>
#include "util.h"

void board_free(Board *b) 
{
	free(b);
}

Board *board_new(void) {
	Board *b = malloc(sizeof(Board));
	memset(b->cells, 0, sizeof(b->cells));
	return b;
}

void board_spawn_tile(Board *b) {
	Point points[BOARD_SIZE];
	size_t len = 0;
	for (size_t y = 0; y < BOARD_SIZEH; y++) {
		for (size_t x = 0; x < BOARD_SIZEW; x++) {
			if (b->cells[x][y] == CELL_EMPTY)
				points[len++] = POINT(x, y);
		}
	}
	size_t index = rand_range(0, len - 1);
	Point p = points[index];
	b->cells[p.x][p.y] = CELL(2);
}

#include <stdio.h>
#include <assert.h>
#include <math.h>
#define MAX_CELL_LEN 4
void board_print(Board *b)
{
	for (size_t y = 0; y < BOARD_SIZEH; y++) {
		for (size_t x = 0; x < BOARD_SIZEW; x++) {
			char str[MAX_CELL_LEN];
			size_t i_len;
			Cell c = b->cells[x][y];
			if (c == CELL_EMPTY) {
				i_len = 1;
				str[0] = '.';
			} else i_len = snprintf(str, sizeof(str), "%d", c);
			assert(i_len <= MAX_CELL_LEN);
			for (size_t i = i_len; i < MAX_CELL_LEN; i++) {
				str[i] = ' ';
		}
		printf("%s",str); 
		}
		putchar('\n');
	}
}
#include <stdbool.h>
static inline void move(Board *b, Point dest, Point src)
{
	b->cells[dest.x][dest.y] += b->cells[src.x][src.y];
	b->cells[src.x][src.y] = CELL_EMPTY;
}

static bool move_left(Board *b, Point p) {
	if (p.x == 0) return false;
	Point lp = POINT(p.x - 1, p.y); 
	Cell cc = b->cells[p.x][p.y];
	Cell lc = b->cells[lp.x][lp.y];
	if (lc == CELL_EMPTY || lc == cc) {
		move(b, lp, p);
		return true;
	}
	return false;
}
static bool move_right(Board *b, Point p) {
	if (p.x == BOARD_SIZEW - 1) return false;
  Point rp = POINT(p.x + 1, p.y);
	Cell cc = b->cells[p.x][p.y];
	Cell rc = b->cells[rp.x][rp.y];
	if (rc == CELL_EMPTY || rc == cc) {
		move(b, rp, p);
		return true;
	}
	return false;
}

static bool move_up(Board *b, Point p) {
	if (p.y == 0) return false;
	Point up = POINT(p.x, p.y - 1);
	Cell cc = b->cells[p.x][p.y];
	Cell uc = b->cells[up.x][up.y];
	if (uc == CELL_EMPTY || uc == cc) {
		move(b, up, p);
		return true;
	}
	return false;
}
static bool move_down(Board *b, Point p) {
	if (p.y == BOARD_SIZEH - 1) return false;
	Point dp = POINT(p.x, p.y + 1);
	Cell cc = b->cells[p.x][p.y];
	Cell dc = b->cells[dp.x][dp.y];
	if (dc == 0 || dc == cc) {
		move(b, dp, p);
		return true;
	}
	return false;
}


void board_move(Board *b, Direction dir)
{
	bool (*movefunc)(Board *, Point) = NULL;
	switch (dir) {
		case MOVE_LEFT:
			movefunc = move_left;
			break;
		case MOVE_RIGHT:
			movefunc = move_right;
			break;
		case MOVE_UP:
			movefunc = move_up;
			break;
		case MOVE_DOWN:
			movefunc = move_down;
			break;
		default:
			assert(0 && "unreachable");
	}

	bool moved[BOARD_SIZE]; // just make sure theres something
	size_t moved_i = 0;
	while (true) {
		bool done = (bool)moved_i; // if moved_i is 0, false by default
		for (size_t i = 0; i < moved_i; i++) {
			if (moved[i]) done = false;
		}
		if (done) return;
		moved_i = 0;
		for (size_t y = 0; y < BOARD_SIZEH; y++) {
			for (size_t x = 0; x < BOARD_SIZEW; x++) {
				Point p = POINT(x, y);
				if (b->cells[p.x][p.y] == CELL_EMPTY) continue;
				moved[moved_i++] = movefunc(b, p);
			}
		}
	}
}

#include "board.h"
#include <stdlib.h>
#include <string.h>
#include "util.h"

#include "globals.h"

void board_free(Board *b) 
{
	free(b);
}

Board *board_new(void) {
	Board *b = malloc(sizeof(Board));
	memset(b->cells, 0, sizeof(b->cells));
	b->score = (Score){.num = 0, .last_add = 0};
	return b;
}

void board_spawn_tile(Board *b) {
	Point points[BOARD_SIZE];
	size_t len = 0;
	for (size_t y = 0; y < BOARD_SIZEH; y++) {
		for (size_t x = 0; x < BOARD_SIZEW; x++) {
			pthread_mutex_lock(&board_mutex);
			if (b->cells[x][y] == CELL_EMPTY)
				points[len++] = POINT(x, y);
			pthread_mutex_unlock(&board_mutex);
		}
	}
	size_t index = len > 1 ? rand_range(0, len - 1): 0;
	Point p = points[index];
	pthread_mutex_lock(&board_mutex);
	b->cells[p.x][p.y] = CELL(2);
	pthread_mutex_unlock(&board_mutex);
}

#include <stdio.h>
#include <assert.h>
#include <math.h>
static Point combined[BOARD_SIZE] = {0};
static size_t num_combined = 0;

#include <stdbool.h>
static bool move(Board *b, Point dest, Point src, bool dry) {
	pthread_mutex_lock(&board_mutex);
	Cell sc = b->cells[src.x][src.y];
	Cell dc = b->cells[dest.x][dest.y];
	pthread_mutex_unlock(&board_mutex);
	bool should_move = false;
	if (dc == CELL_EMPTY) should_move = true;
	else if (sc == dc) {
		bool is_combined = false;
		for (size_t i = 0; i < num_combined; i++) {
			if (point_cmp(combined[i], src)) {
				is_combined = true;
				break;
			}
		}
		if (!is_combined) {
			should_move = true;
			b->score.num += sc + dc;
			b->score.last_add = sc + dc;
			if (!dry) combined[num_combined++] = dest;
		}
	}
	if (should_move && !dry) {
		pthread_mutex_lock(&board_mutex);
		b->cells[dest.x][dest.y] += b->cells[src.x][src.y];
		b->cells[src.x][src.y] = CELL_EMPTY;
		pthread_mutex_unlock(&board_mutex);
	}
	return should_move;
}
static bool move_left(Board *b, Point p) {
	if (p.x == 0) return false;
	Point lp = POINT(p.x - 1, p.y); 
	return move(b, lp, p, false);
}
static bool move_right(Board *b, Point p) {
	if (p.x == BOARD_SIZEW - 1) return false;
  Point rp = POINT(p.x + 1, p.y);
	return move(b, rp, p, false);
}

static bool move_up(Board *b, Point p) {
	if (p.y == 0) return false;
	Point up = POINT(p.x, p.y - 1);
	return move(b, up, p, false);
}
static bool move_down(Board *b, Point p) {
	if (p.y == BOARD_SIZEH - 1) return false;
	Point dp = POINT(p.x, p.y + 1);
	return move(b, dp, p, false);
}


bool board_move(Board *b, Direction dir)
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
	num_combined = 0;
	bool has_moved = false;
	bool moved[BOARD_SIZE]; // just make sure theres something
	size_t moved_i = 0;
	while (true) {
		bool done = has_moved ? true : false;
		for (size_t i = 0; i < moved_i; i++) {
			if (moved[i]) done = false;
		}
		// if its the first turn, return did not move, otherwise return that you moved
		if (done) return moved_i > 0 ? true : false;
		moved_i = 0;
		for (size_t y = 0; y < BOARD_SIZEH; y++) {
			for (size_t x = 0; x < BOARD_SIZEW; x++) {
				Point p = POINT(x, y);
				pthread_mutex_lock(&board_mutex);
				Cell c = b->cells[p.x][p.y];
				pthread_mutex_unlock(&board_mutex);
				if (c == CELL_EMPTY) continue;
				bool is_combined = false;
				for (size_t i = 0; i < num_combined; i++) {
					if (point_cmp(combined[i], p)) { 
						is_combined = true;
						break; 
					}
					if (is_combined) continue;
				}
				moved[moved_i++] = movefunc(b, p);
			}
		}
		has_moved = true;
	}
}


BoardWinStatus board_check_win(Board *b) {
	bool has_empty = false;
	for (size_t y = 0; y < BOARD_SIZEH; y++) {
		for (size_t x = 0; x < BOARD_SIZEW; x++) {
			pthread_mutex_lock(&board_mutex);
			Cell c = b->cells[x][y];
			pthread_mutex_unlock(&board_mutex);
			if (c == CELL_EMPTY) has_empty = true;
			else if (c == 2048) return BOARD_WIN;
		}
	}
	// TODO: check if there are available moves
	if (!has_empty) return BOARD_LOSS;
	return BOARD_NADDA;
}

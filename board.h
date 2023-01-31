#ifndef BOARD_H

#define BOARD_SIZEW 4
#define BOARD_SIZEH 4
#define BOARD_SIZE BOARD_SIZEW * BOARD_SIZEH

#include "colors.h"
typedef struct {
	int num;
	Color color;
} Cell;
#define CELL(n) n
#define CELL_EMPTY 0

typedef struct {
	Cell cells[BOARD_SIZEH][BOARD_SIZEW];
} Board;

Board *board_new(void);
void board_free(Board *b);

void board_spawn_tile(Board *b);

typedef enum {
	MOVE_UP,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT
} Direction;

void board_move(Board *b, Direction dir);
void board_print(Board *b);
#endif // BOARD_H

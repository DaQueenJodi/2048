#ifndef BOARD_H
#define BOARD_H

#define BOARD_SIZEW 4
#define BOARD_SIZEH 4
#define BOARD_SIZE BOARD_SIZEW * BOARD_SIZEH

typedef int Cell;
#define CELL(n) n
#define CELL_EMPTY 0
typedef struct {
	long num;
	long last_add;
} Score;
typedef struct {
	Cell cells[BOARD_SIZEH][BOARD_SIZEW];
	Score score;
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
#include <stdbool.h>
bool board_move(Board *b, Direction dir);
typedef enum {
	BOARD_LOSS,
	BOARD_WIN,
	BOARD_NADDA,
} BoardWinStatus;
BoardWinStatus board_check_win(Board *b);
#endif // BOARD_H

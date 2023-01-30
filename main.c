#include <stdio.h>
#include "board.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
int main(void) {
	srand(time(NULL));
	Board *b = board_new();
	board_spawn_tile(b);
	bool quit = false;
	while (!quit) {
		board_spawn_tile(b);
		board_print(b);
		Direction dir;		
		while (true) {
			char c = getchar();
			switch (c) {
				case 'h':
					dir = MOVE_LEFT;
					break;
				case 'j':
					dir = MOVE_DOWN;
					break;
				case 'k':
					dir = MOVE_UP;
					break;
				case 'l':
					dir = MOVE_RIGHT;
					break;
				case 'q':
					return 0;
				default:
					dir = 0xFF;
			}
			if (dir != 0xFF) break;
			else continue;
		}
		board_move(b, dir);
	}

	return 0;
}

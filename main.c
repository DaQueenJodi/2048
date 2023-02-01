#include <stdio.h>
#include "board.h"
#include "tui.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
int main(void) {
	Board *b = board_new();
	tui_setup(b);
	atexit(tui_cleanup);
	srand(time(NULL));
	board_spawn_tile(b);
	bool quit = false;
	while (!quit) {
		board_spawn_tile(b);
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
			if (dir != 0xFF && board_move(b, dir)) break;
			else continue;
		}
	}
	return 0;
}

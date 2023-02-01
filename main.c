#include <stdio.h>
#include "board.h"
#include "tui.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
int main(void) {
	tui_setup();
	srand(time(NULL));
	Board *b = board_new();
	board_spawn_tile(b);
	bool quit = false;
	while (!quit) {
		board_spawn_tile(b);
		tui_draw_board(b);
		BoardWinStatus status = board_check_win(b);
		if (status != BOARD_NADDA) {
			tui_cleanup();
			quit = true;
		}
		else if (status == BOARD_LOSS) puts("you lost");
		else puts("you won");
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
					tui_cleanup();
					return 0;
				default:
					dir = 0xFF;
			}
			if (dir != 0xFF && board_move(b, dir)) break;
			else continue;
		}
	}

	tui_cleanup();
	return 0;
}

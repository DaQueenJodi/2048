#include "tui.h"
#include <stddef.h>

typedef struct {
	size_t len;
	char *buff;
} DynBuffer;
#include <stdlib.h>
DynBuffer *dyn_buffer_new(void) {
	DynBuffer *db = malloc(sizeof(DynBuffer));
	db->len = 0;
	db->buff = NULL;
	return db;
}
#include <assert.h>
#include <string.h>
void dyn_buffer_append(DynBuffer *db, char *str, size_t len) {
	if (len == 0) return;
	db->buff = realloc(db->buff, db->len + len);
	assert(db->buff != NULL);
	memcpy(db->buff + db->len, str, len);
	db->len += len;
}
void dyn_buffer_free(DynBuffer *db) {
	free(db->buff);
	free(db);
}

#include <unistd.h>
static void clear_screen(DynBuffer *screen) {
	// clear screen
	dyn_buffer_append(screen, "\033[2J", 4);
	// move cursor to top left corner
	dyn_buffer_append(screen, "\033[;f", 4);
}
#define MAX_CELL_COLOR_LEN 69
#define RET_STR(str) memcpy(res, str, sizeof(str) - 1); return sizeof(str) - 1;
#include <string.h>
static size_t cell_to_color_str(Cell c, char* res) {
	switch (c) {
		case CELL_EMPTY:
			RET_STR("\033[2m" "\033[47m");
		case 2:
			RET_STR("\033[97m" "\033[41m");
		case 4:
			RET_STR("\033[97m" "\033[42m");
		case 8:
			RET_STR("\033[97m" "\033[43m");
		case 16:
			RET_STR("\033[97m" "\033[44m");
		case 32:
			RET_STR("\033[97m" "\033[45m");
		case 64:
			RET_STR("\033[97m" "\033[46m");
		case 128:
			RET_STR("\033[97m" "\033[47m");
		case 256:
			RET_STR("\033[97m" "\033[100m");
		case 512:
			RET_STR("\033[97m" "\033[101m");
		case 1024:
			RET_STR("\033[97m" "\033[102m");
		case 2048:
			RET_STR("\033[97m" "\033[103m");
		default:
			assert(0 && "unreachable");
	}
}
#define MAX_CELL_LEN 4
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
static char *cell_to_str(Cell c) {
	char cell_str[MAX_CELL_LEN];
	size_t s_len;
	if (c == CELL_EMPTY) s_len = 0;
	else s_len = snprintf(cell_str, MAX_CELL_LEN, "%d", c);
	assert(s_len <= MAX_CELL_LEN);
	size_t lpadnum = floor((MAX_CELL_LEN - s_len) / 2);
	size_t rpadnum = MAX_CELL_LEN - s_len -  lpadnum;
	char *ret = malloc(lpadnum + rpadnum + s_len);
	size_t counter = 0;
	for (size_t i = 0; i < rpadnum; i++) {
		ret[counter++] = ' ';
	}
	for (size_t i = 0; i < s_len; i++) {
		ret[counter++] = cell_str[i];
	}
	for (size_t i = 0; i < lpadnum; i++) {
		ret[counter++] = ' ';
	}
	return ret;
}
void tui_draw_board(Board *b) {
	DynBuffer *screen = dyn_buffer_new();
	clear_screen(screen);
	for (size_t y = 0; y < BOARD_SIZEH; y++) {
		for (size_t x = 0; x < BOARD_SIZEW; x++) {
			Cell c = b->cells[x][y];
			char color_str[MAX_CELL_COLOR_LEN];
			size_t color_len = cell_to_color_str(c, color_str);
			dyn_buffer_append(screen, color_str, color_len);
			char *cell_str = cell_to_str(c);
			dyn_buffer_append(screen, cell_str, MAX_CELL_LEN);
			free(cell_str);
			dyn_buffer_append(screen, " ", 1);
			dyn_buffer_append(screen, "\033[0m", 4);
		}
		dyn_buffer_append(screen, "\r\n", 2);
	}
	write(STDIN_FILENO, screen->buff, screen->len);
	dyn_buffer_free(screen);
}

struct termios old_termios;
#include <termios.h>
void tui_setup(void) {
		tcgetattr(STDIN_FILENO, &old_termios);
		struct termios raw;
		raw = old_termios;
		raw.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
  	raw.c_iflag &= ~(IXON | ICRNL  | IGNBRK| PARMRK | BRKINT | INPCK | ISTRIP);
  	raw.c_oflag &= ~(OPOST);
  	raw.c_cflag |= (CS8);

		tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
		// hide cursor
		write(STDIN_FILENO, "\033[?25l", 6);
}
#include <termios.h>
void tui_cleanup(void) {
	write(STDIN_FILENO, "\033[?25h", 6);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &old_termios);
}

#include "tui.h"
#include <stddef.h>
#include "globals.h"

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
pthread_mutex_t timer_mutex;
#include <sys/time.h>
#include <time.h>
#include <pthread.h>

struct timeval elapsed_time;
static void *update_timer(void *ignore) {
struct timeval current_time;
	(void)ignore;
	struct timeval initial_time;
	gettimeofday(&initial_time, NULL);
	struct timespec sleep_ammount = {0, 100 * 1000}; // 100 miliseconds
	while (true) {
		gettimeofday(&current_time, NULL);
		pthread_mutex_lock(&timer_mutex);
		elapsed_time.tv_usec = current_time.tv_usec - initial_time.tv_usec;
		elapsed_time.tv_sec = current_time.tv_sec - initial_time.tv_sec;
		pthread_mutex_unlock(&timer_mutex);
		nanosleep(&sleep_ammount, NULL);
	}
}
#include <pthread.h>
#include <stdio.h>
#define MAX_TIME_LEN 69
static void draw_timer(DynBuffer *screen) {
	struct timeval time;
	pthread_mutex_lock(&timer_mutex);
	time = elapsed_time;
	pthread_mutex_unlock(&timer_mutex);
	char time_str[MAX_TIME_LEN];
	size_t str_len = snprintf(time_str, MAX_TIME_LEN, "time: %zu", time.tv_sec);
	dyn_buffer_append(screen, time_str, str_len);
	dyn_buffer_append(screen, "\r\n", 2);
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
#define MAX_CELL_LEN 5
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
void tui_draw_board(DynBuffer *screen, Board *b) {
	for (size_t y = 0; y < BOARD_SIZEH; y++) {
		for (size_t x = 0; x < BOARD_SIZEW; x++) {
			pthread_mutex_lock(&board_mutex);
			Cell c = b->cells[x][y];
			pthread_mutex_unlock(&board_mutex);
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
}
#define MAX_SCORE_LEN 69
static void draw_score(DynBuffer *screen, Board *b) {
	long score;
	pthread_mutex_lock(&board_mutex);
	score = b->score;
	pthread_mutex_unlock(&board_mutex);
	char score_str[MAX_SCORE_LEN];
	size_t score_len = snprintf(score_str, MAX_SCORE_LEN, "score: %ld", score);
	dyn_buffer_append(screen, score_str, score_len);
	dyn_buffer_append(screen, "\r\n\r\n", 4);
}
static void *update_tui(void *board_ptr) {
	Board *b = (Board *)board_ptr;
	while (!quit) {
		DynBuffer *screen = dyn_buffer_new();
		clear_screen(screen);
		draw_timer(screen);
		draw_score(screen, b);
		tui_draw_board(screen, b);
		write(STDOUT_FILENO, screen->buff, screen->len);
		dyn_buffer_free(screen);
		usleep(100);
	}
	return NULL;
}

struct termios old_termios;
#include <termios.h>
void tui_setup(Board *b) {
		// set up timer
		pthread_t ignore;
		pthread_create(&ignore, NULL, update_timer, NULL);
		pthread_create(&ignore, NULL, update_tui, b);
		pthread_mutex_init(&timer_mutex, NULL);
		pthread_mutex_init(&quit_mutex, NULL);
		pthread_mutex_init(&board_mutex, NULL);


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
	pthread_mutex_destroy(&timer_mutex);
	write(STDIN_FILENO, "\033[?25h", 6);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &old_termios);
}


void tui_draw_end(bool won) {
	DynBuffer *screen = dyn_buffer_new();
	clear_screen(screen);
	dyn_buffer_append(screen, "you ", 4);
	dyn_buffer_append(screen, won ? "won!" : "lost!", won ? 4: 5);
	dyn_buffer_append(screen, "\r\n ", 2);
	write(STDOUT_FILENO, screen->buff, screen->len);
}

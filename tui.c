#include "tui.h"

typedef struct {
	size_t len;
	char *buff;
} DynBuffer;
#include <stdlib.h>
DynBuffer *dyn_buffer_new(void) {
	DynBuffer *db = malloc(sizeof(DynBuffer));
	db->len = 0;
	db->buff = NULL;
}
#include <assert.h>
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
void tui_draw_board(Board *b) {
	DynBuffer *screen = dyn_buffer_new();
	for (size_t y = 0; y < BOARD_SIZEH; y++) {
		for (size_t x = 0; x < BOARD_SIZEW; x++) {
			dyn
		}
	}
	dyn_buffer_free(screen);
}

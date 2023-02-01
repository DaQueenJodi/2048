SRCS    := main.c util.c board.c tui.c globals.c
CFLAGS  := -Wextra -Wall -Wpedantic -ggdb
LDFLAGS := -lm -lpthread
BIN     := 2048
CC		  := clang
.PHONY: clean run all

all: $(BIN)
$(BIN): $(SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRCS) -o $(BIN)

clean:
	rm $(BIN)
dbg: debug
debug:
	gdb $(BIN)
run:
	./$(BIN)

SRCS    := main.c util.c board.c tui.c
CFLAGS  := -Wextra -Wall -Wpedantic -ggdb
LDFLAGS := -lm
BIN     := 2048
CC		  := clang
.PHONY: clean run all

all: $(BIN)
$(BIN): $(SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRCS) -o $(BIN)

clean:
	rm $(BIN)

run:
	./$(BIN)

CC := gcc
CFLAGS := -Wall -Werror -Wextra -std=c11
GCOV_FLAGS := -fprofile-arcs -ftest-coverage

CHECK_FLAGS := -lcheck -lm -lsubunit -lpthread -lrt
LDFLAGS := -lncurses

PREFIX ?= /usr/local
BINDIR := $(PREFIX)/bin

BUILD := build
DIST := brick_games.tar.gz

TETRIS_SRC = \
brick_game/tetris/src/tetris.c \
brick_game/tetris/src/fsm.c \
brick_game/tetris/src/tetris_backend.c \
brick_game/tetris/src/tetris_shapes.c \
gui/cli/brick_frontend.c

TEST_SRC = \
brick_game/tetris/tests/tests.c \
brick_game/tetris/tests/test_fsm.c \
brick_game/tetris/tests/test_backend.c \
brick_game/tetris/tests/test_shapes.c \
brick_game/tetris/src/fsm.c \
brick_game/tetris/src/tetris_backend.c \
brick_game/tetris/src/tetris_shapes.c \
gui/cli/brick_frontend.c

TETRIS = $(BUILD)/tetris
TEST_BIN = $(BUILD)/test_runner

.PHONY: all install uninstall clean dvi dist test gcov_report

all: $(TETRIS)

$(TETRIS): $(TETRIS_SRC)
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clang:
	clang-format -style=Google -i $$(find . -name "*.c" -o -name "*.h")

clean:
	rm -rf $(BUILD)
	rm -rf report
	rm -f *.gcno *.gcda
	rm -f memleaks.log
	rm -f $(DIST)
	rm -f high_score.txt

cppcheck:
	cppcheck --enable=all \
	--suppress=missingIncludeSystem \
	--suppress=checkersReport \
	$$(find . -name "*.c" -o -name "*.h")

dist: clean
	@tar -czf ../$(DIST) --exclude='$(BUILD)' --exclude='report' --exclude='*.gcno' --exclude='*.gcda' --exclude='$(DIST)' -C .. $$(basename $$(pwd))
	@mv ../$(DIST) .

dvi:
	@echo "DVI documentation is not available for this project."

gcov_report:
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(TEST_SRC) -o $(TEST_BIN) $(CHECK_FLAGS) $(LDFLAGS)
	./$(TEST_BIN)
	mkdir -p report
	gcovr -r . \
	--object-directory $(BUILD) \
	--exclude ".*tests/.*" \
	--exclude ".*brick_game/gui/.*" \
	--exclude-branches-by-pattern ".*" \
	--html --html-details -o report/index.html

install: all
	mkdir -p $(BINDIR)
	cp $(TETRIS) $(BINDIR)

test:
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(TEST_SRC) -o $(TEST_BIN) $(CHECK_FLAGS) $(LDFLAGS)
	./$(TEST_BIN)

uninstall:
	rm -f $(BINDIR)/tetris

valgrind: test
	valgrind --tool=memcheck \
	--leak-check=full \
	--show-leak-kinds=all \
	--track-origins=yes \
	--log-file=memleaks.log \
	./$(TEST_BIN)
#ifndef DEFINES_H
#define DEFINES_H

#include <ncurses.h>

#define WIN_INIT(time)    \
  {                       \
    initscr();            \
    noecho();             \
    curs_set(0);          \
    keypad(stdscr, TRUE); \
    timeout(time);        \
  }

#define GET_USER_INPUT getch()

#define PRINT_FROG(x, y) mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), "@")
#define MVPRINTW(y, x, ...) \
  mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), __VA_ARGS__)
#define MVADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), c)
#define CLEAR_BACKPOS(y, x) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), ' ')

#define YOU_WON "tests/game_progress/you_won.txt"
#define YOU_LOSE "tests/game_progress/you_lose.txt"

#define LEVEL_DIR "../brick_game/frogger/tests/levels/level_"
#define WELCOMING "WELCOME"
#define INTRO_MESSAGE "Press ENTER"
#define GAMEOVER_MESSAGE "YOU LOSE"
#define WINNING_MESSAGE "YOU WIN"
#define PAUSE_MESSAGE "Paused"
#define PAUSE_HINT "P for pause"
#define CLOSE_MESSAGE "Closing..."
#define EXIT_MESSAGE "exit by ESC"
#define INTRO_MESSAGE_LEN 11
#define LEVEL_CNT 5

#define LEVELNAME_MAX ((sizeof(LEVEL_DIR) - 1) + 11 + (sizeof(".txt") - 1))

#define MAX_WIN_COUNT 10

#define ROWS_MAP 21
#define COLS_MAP 90

#define BOARDS_BEGIN 2

#define FROGSTART_X (BOARD_M / 2)
#define FROGSTART_Y (BOARD_N)
#define INITIAL_TIMEOUT 150
#define BASE_TIMEOUT 500
#define MIN_TIMEOUT 50

#define BOARD_N (ROWS_MAP + MAP_PADDING * 2)
#define BOARD_M 30
#define BOARD_Y BOARD_N
#define BOARD_X BOARD_M
#define HUD_WIDTH 12
#define MAP_PADDING 3

#define TETRIS_N 20
#define TETRIS_M 10
#define TETRIS_OFFSET_Y ((BOARD_N - TETRIS_N) / 2)
#define TETRIS_OFFSET_X ((BOARD_M - TETRIS_M) / 2)

#define SHAPE_H 5
#define SHAPE_W 5
#define SHAPE_COUNT 7

#define BANNER_N 10
#define BANNER_M 100

#define SUCCESS 0
#define ERROR 1

#define NO_INPUT -1

#define ESCAPE 27
#define ENTER_KEY 10
#define SPACE 32

#endif

#include "../include/tetris.h"

#include <locale.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

int main() {
  srand(time(NULL));
  WIN_INIT(50);
  setlocale(LC_ALL, "");
  print_overlay(tetris);
  tetris_game_loop();
  endwin();

  return SUCCESS;
}

void tetris_game_loop() {
  game_info_t field;
  user_action_t action;

  bool break_flag = TRUE;
  bool hold = FALSE;
  int signal = 0;
  brick_state state = START;

  update_current_state(&field, &state);

  while (break_flag) {
    if (state == EXIT) {
      break_flag = FALSE;
    }

    user_input(&action, signal, hold);
    sigact_tetris(&field, action, &state);

    if (state == MOVING || state == SPAWN || state == PAUSED ||
        state == START || state == GAMEOVER || state == WIN) {
      signal = GET_USER_INPUT;
    }

    signal == NO_INPUT ? hold = FALSE : TRUE;
  }
}
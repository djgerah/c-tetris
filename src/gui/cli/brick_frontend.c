#include "../../include/brick_frontend.h"

#include <ncurses.h>
// #include <string.h>

void print_file_error() {
  clear();

  MVPRINTW(0, 0, "An error occured opening high score file!");
  MVPRINTW(2, 0, "Make sure the file is writable or exists.");
  MVPRINTW(3, 0, "There should be file named high_score.txt.");
  MVPRINTW(4, 0, "Press any key to exit.");
}

void print_overlay(game_type game) {
  if (game == tetris) {
    print_rectangle(TETRIS_OFFSET_Y, TETRIS_OFFSET_Y + TETRIS_N + 1,
                    TETRIS_OFFSET_X - 5, TETRIS_OFFSET_X + TETRIS_M * 2 - 4);
    print_rectangle(13, 20, BOARD_X + 3, BOARD_X + HUD_WIDTH + 5);

    MVPRINTW(14, BOARD_X + 5, "NEXT:");
  }

  print_rectangle(0, BOARD_Y + 1, 0, BOARD_X + 1);
  print_rectangle(0, BOARD_Y + 1, BOARD_X + 2, BOARD_X + HUD_WIDTH + 6);

  print_rectangle(1, 3, BOARD_X + 3, BOARD_X + HUD_WIDTH + 5);
  print_rectangle(4, 6, BOARD_X + 3, BOARD_X + HUD_WIDTH + 5);
  print_rectangle(7, 9, BOARD_X + 3, BOARD_X + HUD_WIDTH + 5);
  print_rectangle(10, 12, BOARD_X + 3, BOARD_X + HUD_WIDTH + 5);

  MVPRINTW(2, BOARD_X + 5, "LEVEL:");
  MVPRINTW(5, BOARD_X + 5, "SPEED:");
  MVPRINTW(8, BOARD_X + 5, "SCORE:");
  MVPRINTW(11, BOARD_X + 5, "RECORD:");

  if (game != tetris) {
    print_rectangle(10, 12, BOARD_X + 3, BOARD_X + HUD_WIDTH + 5);
    MVPRINTW(11, BOARD_X + 5, "LIVES:");
  }

  MVPRINTW(BOARD_Y / 2, BOARD_X / 2 - 2, WELCOMING);
}

void print_rectangle(int top_y, int bottom_y, int left_x, int right_x) {
  MVADDCH(top_y, left_x, ACS_ULCORNER);

  int x = left_x + 1;

  for (; x < right_x; x++) MVADDCH(top_y, x, ACS_HLINE);
  MVADDCH(top_y, x, ACS_URCORNER);

  for (int y = top_y + 1; y < bottom_y; y++) {
    MVADDCH(y, left_x, ACS_VLINE);
    MVADDCH(y, right_x, ACS_VLINE);
  }

  MVADDCH(bottom_y, left_x, ACS_LLCORNER);

  x = left_x + 1;
  for (; x < right_x; x++) MVADDCH(bottom_y, x, ACS_HLINE);
  MVADDCH(bottom_y, x, ACS_LRCORNER);
}

void print_stats(game_type game, brick_stats_t *stats) {
  MVPRINTW(2, BOARD_X + 11, "%5d", stats->level);
  MVPRINTW(5, BOARD_X + 11, "%5d", stats->speed);
  MVPRINTW(8, BOARD_X + 11, "%5d", stats->score);
  MVPRINTW(11, BOARD_X + 11, "%5d", stats->high_score);

  if (game != tetris) {
    MVPRINTW(11, BOARD_X + 11, "%5d", stats->lives);
  }
}

void save_high_score(const brick_stats_t *tetris, brick_state *state) {
  FILE *file = fopen("high_score.txt", "w");

  if (!file) {
    *state = FILE_ERROR;
    return;
  }

  fprintf(file, "%d\n", tetris->high_score);

  fclose(file);
}

void read_high_score(int *high_score, brick_state *state) {
  *high_score = 0;

  FILE *file = fopen("high_score.txt", "r");

  if (!file) {
    *state = FILE_ERROR;
    return;
  }

  fscanf(file, "%d", high_score);
  fclose(file);
}

void print_field(const int field[TETRIS_N][TETRIS_M]) {
  for (int row = 0; row < TETRIS_N; row++) {
    for (int col = 0; col < TETRIS_M; col++) {
      int y = TETRIS_OFFSET_Y + row + 1;
      int x = TETRIS_OFFSET_X + col * 2 - 4;

      if (field[row][col]) {
        MVADDCH(y, x, ACS_CKBOARD);
        MVADDCH(y, x + 1, ACS_CKBOARD);
      } else {
        MVADDCH(y, x, ' ');
        MVADDCH(y, x + 1, ' ');
      }
    }
  }
}

void preview_next_figure(const field_info_t *field) {
  int y = 15;
  int x = BOARD_X + 6;

  for (int row = 0; row < SHAPE_H; row++) {
    for (int col = 0; col < SHAPE_W; col++) {
      if (field->next.matrix[row][col]) {
        MVADDCH(y + row, x + col * 2, ACS_CKBOARD);
        MVADDCH(y + row, x + col * 2 + 1, ACS_CKBOARD);
      } else {
        MVADDCH(y + row, x + col * 2, ' ');
        MVADDCH(y + row, x + col * 2 + 1, ' ');
      }
    }
  }
}

void start_msg() {
  print_rectangle(21, 25, BOARD_X + 3, BOARD_X + HUD_WIDTH + 5);

  MVPRINTW(22, BOARD_X + 5, INTRO_MESSAGE);
  MVPRINTW(23, BOARD_X + 9, "or");
  MVPRINTW(24, BOARD_X + 5, EXIT_MESSAGE);
  refresh();
}

void winning_msg() {
  MVPRINTW(BOARD_Y / 2, BOARD_X / 2 - 2, WINNING_MESSAGE);
  refresh();
}

void hint_msg() {
  print_rectangle(21, 25, BOARD_X + 3, BOARD_X + HUD_WIDTH + 5);

  MVPRINTW(22, BOARD_X + 5, PAUSE_HINT);
  refresh();
}

void pause_msg() {
  MVPRINTW(BOARD_Y / 2, BOARD_X / 2 - 2, PAUSE_MESSAGE);
  refresh();
}

void exit_msg() {
  MVPRINTW(BOARD_Y / 2, BOARD_X / 2 - 4, CLOSE_MESSAGE);
  refresh();
  napms(1000);
}

void gameover_msg() {
  MVPRINTW(BOARD_Y / 2, BOARD_X / 2 - 2, GAMEOVER_MESSAGE);
  refresh();
}

void procces_error() {
  print_file_error();
  refresh();
  napms(2000);
  clear();
  print_overlay(tetris);
}

void update_timeout(int speed) {
  int timeout_ms = BASE_TIMEOUT / speed;
  if (timeout_ms < MIN_TIMEOUT) timeout_ms = MIN_TIMEOUT;
  timeout(timeout_ms);
}
#include "../include/tetris_backend.h"

#include <string.h>

#include "../../../include/brick_frontend.h"
#include "../include/tetris_shapes.h"

void level_up(brick_stats_t *tetris) {
  int level = (tetris->score / 600) + 1;

  if (level > 10) {
    level = 10;
    tetris->won = TRUE;
  }

  tetris->level = level;
  tetris->speed = level;

  update_timeout(tetris->speed);
}

void add_score(int lines, brick_stats_t *tetris, brick_state *state) {
  int score = 0;

  if (lines == 1) {
    score = 100;
  } else if (lines == 2) {
    score = 300;
  } else if (lines == 3) {
    score = 700;
  } else if (lines >= 4) {
    score = 1500;
  }

  tetris->score += score;

  if (tetris->score > tetris->high_score) {
    tetris->high_score = tetris->score;
    save_high_score(tetris, state);
  }

  if (*state == FILE_ERROR) {
    procces_error();
    *state = START;
  }

  level_up(tetris);
}

void stamp_figure(field_info_t *field, int target[TETRIS_N][TETRIS_M]) {
  for (int row = 0; row < SHAPE_H; row++) {
    for (int col = 0; col < SHAPE_W; col++) {
      if (!field->curr.matrix[row][col]) continue;

      int x = field->curr.pos.x + col;
      int y = field->curr.pos.y + row;

      if (y < 0 || y >= TETRIS_N || x < 0 || x >= TETRIS_M) continue;

      target[y][x] = 1;
    }
  }
}

void lock_figure(field_info_t *field) {
  stamp_figure(field, field->figures_placed);
}

void overlay_active_figure(field_info_t *field, bool show_figure) {
  if (show_figure) {
    stamp_figure(field, field->field);
  }
}

void update_field(field_info_t *field) {
  memset(field->field, 0, sizeof(field->field));
}

void build_field(field_info_t *field) {
  update_field(field);

  for (int y = 0; y < TETRIS_N; y++) {
    for (int x = 0; x < TETRIS_M; x++) {
      if (field->figures_placed[y][x]) {
        field->field[y][x] = 1;
      }
    }
  }
}

void shift_down(field_info_t *field, int start) {
  for (int row = start; row > 0; row--) {
    memcpy(field->figures_placed[row], field->figures_placed[row - 1],
           sizeof(field->figures_placed[row]));
  }

  memset(field->figures_placed[0], 0, sizeof(field->figures_placed[0]));
}

void clear_lines(brick_stats_t *stats, field_info_t *field,
                 brick_state *state) {
  int cleared = 0;

  for (int y = TETRIS_N - 1; y >= 0; y--) {
    bool filled = TRUE;

    for (int x = 0; x < TETRIS_M; x++) {
      if (!field->figures_placed[y][x]) {
        filled = FALSE;
        break;
      }
    }

    if (filled) {
      shift_down(field, y);

      cleared++;
      y++;
    }
  }

  if (cleared > 0) {
    add_score(cleared, stats, state);
  }
}

bool check_collide(const int figures_placed[TETRIS_N][TETRIS_M],
                   int curr_figure_pos_x, int curr_figure_pos_y,
                   const int matrix[SHAPE_H][SHAPE_W]) {
  bool rc = FALSE;

  for (int row = 0; row < SHAPE_H; row++) {
    for (int col = 0; col < SHAPE_W; col++) {
      if (!matrix[row][col]) continue;

      int i = curr_figure_pos_x + col;
      int j = curr_figure_pos_y + row;

      if (i < 0 || i >= TETRIS_M || j >= TETRIS_N ||
          (j >= 0 && figures_placed[j][i]))
        rc = TRUE;
    }
  }

  return rc;
}

bool is_square(int type) { return type == SHAPE_O; }

void transpose_matrix(int matrix[SHAPE_H][SHAPE_W],
                      int result[SHAPE_W][SHAPE_H]) {
  for (int row = 0; row < SHAPE_H; row++) {
    for (int col = 0; col < SHAPE_W; col++) {
      result[col][SHAPE_H - 1 - row] = matrix[row][col];
    }
  }
}

bool figure_dropping(field_info_t *field) {
  bool rc = TRUE;

  if (check_collide(field->figures_placed, field->curr.pos.x,
                    field->curr.pos.y + 1, field->curr.matrix)) {
    rc = FALSE;
  } else {
    field->curr.pos.y++;
  }

  return rc;
}

void render_game(brick_stats_t *stats, field_info_t *field,
                 const brick_state *state) {
  bool condition = (*state == SPAWN || *state == MOVING || *state == SHIFTING ||
                    *state == ATTACH || *state == PAUSED ||
                    *state == GAMEOVER || *state == EXIT);

  build_field(field);
  overlay_active_figure(field, condition);
  print_field(field->field);
  preview_next_figure(field);
  print_stats(tetris, stats);

  refresh();
}

void tetris_stats_init(brick_stats_t *tetris, brick_state *state) {
  tetris->score = 0;
  read_high_score(&tetris->high_score, state);
  tetris->level = 1;
  tetris->speed = 1;
  tetris->lives = 3;
  tetris->won = FALSE;
  update_timeout(tetris->speed);

  if (*state == FILE_ERROR) {
    // procces_error();
    *state = START;
  }
}

void init_figures(figure_t *curr_figure, figure_t *next_figure) {
  load_shape(curr_figure);

  curr_figure->pos.x = TETRIS_M / 2 - 2;
  curr_figure->pos.y = -2;

  next_figure->type = rand_shape();
  load_shape(next_figure);
}

void init_tetris_game(field_info_t *tetris) {
  memset(tetris->field, 0, sizeof(tetris->field));
  memset(tetris->figures_placed, 0, sizeof(tetris->figures_placed));
  memset(tetris->next.matrix, 0, sizeof(tetris->next.matrix));

  tetris->curr.type = rand_shape();

  init_figures(&tetris->curr, &tetris->next);
}
#ifndef OBJECTS_H
#define OBJECTS_H

#include <stdbool.h>

#include "defines.h"

typedef enum { frogger = 0, tetris } game_type;

typedef enum {
  SHAPE_I = 0,
  SHAPE_O,
  SHAPE_T,
  SHAPE_S,
  SHAPE_Z,
  SHAPE_J,
  SHAPE_L,
} shape_type;

typedef struct {
  int x;
  int y;
} pos_t;

typedef struct {
  char matrix[BANNER_N + 1][BANNER_M + 1];
} banner_t;

typedef struct {
  char finish[BOARD_M + 2];
  char ways[ROWS_MAP + 2][COLS_MAP + 2];
} board_t;

typedef struct {
  shape_type type;
  pos_t pos;

  int matrix[SHAPE_H][SHAPE_W];
} figure_t;

typedef struct {
  int field[TETRIS_N][TETRIS_M];
  int figures_placed[TETRIS_N][TETRIS_M];

  figure_t curr;
  figure_t next;

  int score;
  int high_score;
  int level;
  int speed;
  int lives;
  bool won;
} game_info_t;

#endif

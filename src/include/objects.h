#ifndef OBJECTS_H
#define OBJECTS_H

#include <stdbool.h>

#include "defines.h"

typedef enum { tetris = 0 } game_type;

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
  shape_type type;
  pos_t pos;

  int matrix[SHAPE_H][SHAPE_W];
} figure_t;

typedef struct {
  int score;
  int high_score;
  int level;
  int speed;
  int lives;
  bool won;
} brick_stats_t;

typedef struct {
  int field[TETRIS_N][TETRIS_M];
  int figures_placed[TETRIS_N][TETRIS_M];

  figure_t curr;
  figure_t next;
} field_info_t;

#endif

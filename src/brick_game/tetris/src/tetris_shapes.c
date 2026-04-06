#include "../include/tetris_shapes.h"

#include <stdlib.h>
#include <string.h>

const int SHAPES[SHAPE_COUNT][SHAPE_H][SHAPE_W] = {
    // SHAPE_I
    {{0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0},
     {0, 1, 1, 1, 0},
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0}},

    // SHAPE_O
    {{0, 0, 0, 0, 0},
     {0, 1, 1, 0, 0},
     {0, 1, 1, 0, 0},
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0}},

    // SHAPE_T
    {{0, 0, 0, 0, 0},
     {0, 0, 1, 0, 0},
     {0, 1, 1, 1, 0},
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0}},

    // SHAPE_S
    {{0, 0, 0, 0, 0},
     {0, 0, 1, 1, 0},
     {0, 1, 1, 0, 0},
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0}},

    // SHAPE_Z
    {{0, 0, 0, 0, 0},
     {0, 1, 1, 0, 0},
     {0, 0, 1, 1, 0},
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0}},

    // SHAPE_J
    {{0, 0, 0, 0, 0},
     {0, 1, 0, 0, 0},
     {0, 1, 1, 1, 0},
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0}},

    // SHAPE_L
    {{0, 0, 0, 0, 0},
     {0, 0, 0, 1, 0},
     {0, 1, 1, 1, 0},
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0}}};

int rand_shape() { return rand() % SHAPE_COUNT; }

void load_shape(figure_t *figure) {
  memcpy(figure->matrix, SHAPES[figure->type], sizeof(figure->matrix));
}
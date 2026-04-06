#ifndef TETRIS_SHAPES_H
#define TETRIS_SHAPES_H

#include <stdint.h>

#include "../../../include/objects.h"

extern const int SHAPES[SHAPE_COUNT][SHAPE_H][SHAPE_W];

int rand_shape();
void load_shape(figure_t *figure);

#endif
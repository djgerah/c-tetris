#ifndef TETRIS_BACKEND_H
#define TETRIS_BACKEND_H

#include "../../../include/defines.h"
#include "../../../include/fsm.h"
#include "../../../include/objects.h"

void level_up(brick_stats_t *tetris);
void add_score(int lines, brick_stats_t *tetris, brick_state *state);
void shift_down(field_info_t *field, int start);
void stamp_figure(field_info_t *field, int target[TETRIS_N][TETRIS_M]);
void lock_figure(field_info_t *field);
void clear_lines(brick_stats_t *stats, field_info_t *field, brick_state *state);
bool check_collide(const int figures_placed[TETRIS_N][TETRIS_M],
                   int curr_figure_pos_x, int curr_figure_pos_y,
                   const int matrix[SHAPE_H][SHAPE_W]);
bool is_square(int type);
void transpose_matrix(int matrix[SHAPE_H][SHAPE_W],
                      int result[SHAPE_W][SHAPE_H]);
bool figure_dropping(field_info_t *field);
void render_game(brick_stats_t *stats, field_info_t *field,
                 const brick_state *state);
void tetris_stats_init(brick_stats_t *tetris, brick_state *state);
void init_figures(figure_t *curr_figure, figure_t *next_figure);
void init_tetris_game(field_info_t *field);
void overlay_active_figure(field_info_t *field, bool show_figure);
void update_field(field_info_t *field);
void build_field(field_info_t *field);

#endif
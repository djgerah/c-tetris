#ifndef TETRIS_BACKEND_H
#define TETRIS_BACKEND_H

#include "../../../include/defines.h"
#include "../../../include/fsm.h"
#include "../../../include/objects.h"

void level_up(game_info_t *tetris);
void add_score(int lines, game_info_t *tetris, brick_state *state);
void shift_down(game_info_t *field, int start);
void stamp_figure(game_info_t *field, int target[TETRIS_N][TETRIS_M]);
void lock_figure(game_info_t *field);
void clear_lines(game_info_t *field, brick_state *state);
bool check_collide(const int figures_placed[TETRIS_N][TETRIS_M],
                   int curr_figure_pos_x, int curr_figure_pos_y,
                   const int matrix[SHAPE_H][SHAPE_W]);
bool is_square(int type);
void transpose_matrix(int matrix[SHAPE_H][SHAPE_W],
                      int result[SHAPE_W][SHAPE_H]);
bool figure_dropping(game_info_t *field);
void render_game(game_info_t *field, const brick_state *state);
void tetris_stats_init(game_info_t *tetris, brick_state *state);
void init_figures(figure_t *curr_figure, figure_t *next_figure);
void init_tetris_game(game_info_t *field);
void overlay_active_figure(game_info_t *field, bool show_figure);
void update_field(game_info_t *field);
void build_field(game_info_t *field);

#endif
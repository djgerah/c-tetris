#ifndef BRICK_FRONTEND_H
#define BRICK_FRONTEND_H

#include "fsm.h"
#include "objects.h"

void print_level_error();
void print_file_error();
void print_overlay(game_type game);
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x);
void print_stats(game_type game, brick_stats_t *stats);
void print_banner(const brick_stats_t *stats);
void save_high_score(const brick_stats_t *tetris, brick_state *state);
void read_high_score(int *high_score, brick_state *state);
void print_field(const int field[TETRIS_N][TETRIS_M]);
void preview_next_figure(const field_info_t *field);
void start_msg();
void hint_msg();
void pause_msg();
void exit_msg();
void gameover_msg();
void procces_error();
void update_timeout(int speed);
void winning_msg();

#endif
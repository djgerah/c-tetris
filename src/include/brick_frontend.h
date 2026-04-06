#ifndef FROGGER_FRONTEND_H
#define FROGGER_FRONTEND_H

#include "fsm.h"
#include "objects.h"

void print_level_error();
void print_file_error();
void print_overlay(game_type game);
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x);
void print_stats(game_type game, game_info_t *stats);
void print_board(const board_t *game, const pos_t *frog);
void print_cars(const board_t *game);
void print_finished(const board_t *game);
void print_banner(const game_info_t *stats);
int read_banner(const game_info_t *stats, banner_t *banner);
void save_high_score(const game_info_t *tetris, brick_state *state);
void read_high_score(int *high_score, brick_state *state);
void print_field(const int field[TETRIS_N][TETRIS_M]);
void preview_next_figure(const game_info_t *field);
void start_msg();
void hint_msg();
void pause_msg();
void exit_msg();
void gameover_msg();
void procces_error();
void update_timeout(int speed);
void winning_msg();

#endif
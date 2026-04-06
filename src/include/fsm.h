#ifndef FSM_H
#define FSM_H

#include "objects.h"

typedef enum {
  Start = 0,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action,
  Hard_drop,
  Nosig
} user_action_t;

typedef enum {
  START = 0,
  SPAWN,
  MOVING,
  SHIFTING,
  REACH,
  COLLIDE,
  FILE_ERROR,
  ATTACH,
  PAUSED,
  GAMEOVER,
  WIN,
  EXIT
} brick_state;

void user_input(user_action_t *action, int user_input, bool hold);
void sigact_tetris(brick_stats_t *stats, field_info_t *filed, user_action_t sig,
                   brick_state *state);
void move_left(field_info_t *field);
void move_right(field_info_t *field);
void hard_drop(field_info_t *figure, brick_state *state);
void rotate(field_info_t *figure);
void spawn_figure(field_info_t *field, brick_state *state);
void on_start_state_tetris(user_action_t sig, brick_state *state);
void on_spawn_state_tetris(field_info_t *figure, brick_state *state);
void on_moving_state_tetris(brick_stats_t *stats, field_info_t *figure,
                            user_action_t sig, brick_state *state);
void on_shifting_state_tetris(field_info_t *figure, brick_state *state);
void on_attach_state_tetris(brick_stats_t *stats, field_info_t *figure,
                            brick_state *state);
void on_paused_state_tetris(user_action_t sig, brick_state *state);
void on_gameover_state_tetris(brick_stats_t *stats, field_info_t *field,
                              user_action_t sig, brick_state *state);
void on_win_state_tetris(brick_stats_t *stats, field_info_t *field,
                         user_action_t sig, brick_state *state);
void on_exit_state_tetris(brick_stats_t *stats, field_info_t *game,
                          const brick_state *state);
void update_current_state(brick_stats_t *stats, field_info_t *field,
                          brick_state *state);

#endif
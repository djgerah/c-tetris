#include "../../../include/fsm.h"
#include "../include/tetris_backend.h"
#include "../include/tetris_shapes.h"
#include "tests.h"

START_TEST(test_user_input_keys) {
  user_action_t action;

  user_input(&action, NO_INPUT, false);
  ck_assert_int_eq(action, Nosig);

  user_input(&action, 'p', false);
  ck_assert_int_eq(action, Pause);

  user_input(&action, 'P', false);
  ck_assert_int_eq(action, Pause);

  user_input(&action, KEY_LEFT, false);
  ck_assert_int_eq(action, Left);

  user_input(&action, KEY_RIGHT, false);
  ck_assert_int_eq(action, Right);

  user_input(&action, KEY_DOWN, false);
  ck_assert_int_eq(action, Down);

  user_input(&action, KEY_DOWN, true);
  ck_assert_int_eq(action, Hard_drop);

  user_input(&action, 'q', false);
  ck_assert_int_eq(action, Terminate);

  user_input(&action, 'Q', false);
  ck_assert_int_eq(action, Terminate);

  user_input(&action, ESCAPE, false);
  ck_assert_int_eq(action, Terminate);

  user_input(&action, SPACE, false);
  ck_assert_int_eq(action, Action);

  user_input(&action, ENTER_KEY, false);
  ck_assert_int_eq(action, Start);
}
END_TEST

START_TEST(test_start_state) {
  brick_state state = START;

  on_start_state_tetris(Start, &state);

  ck_assert_int_eq(state, SPAWN);

  state = START;

  on_start_state_tetris(Terminate, &state);

  ck_assert_int_eq(state, EXIT);

  state = START;

  on_start_state_tetris(Nosig, &state);

  ck_assert_int_eq(state, START);
}
END_TEST

START_TEST(test_pause_state) {
  brick_state state = PAUSED;

  on_paused_state_tetris(Pause, &state);
  ck_assert_int_eq(state, MOVING);

  state = PAUSED;

  on_paused_state_tetris(Terminate, &state);

  ck_assert_int_eq(state, EXIT);
}
END_TEST

START_TEST(test_shifting_state_attach) {
  game_info_t field;

  init_tetris_game(&field);
  brick_state state = SHIFTING;

  on_shifting_state_tetris(&field, &state);

  ck_assert(state == MOVING);

  hard_drop(&field, &state);

  ck_assert(state == ATTACH);
}
END_TEST

START_TEST(test_sigact_start) {
  game_info_t field;

  init_tetris_game(&field);
  brick_state state = START;

  sigact_tetris(&field, Start, &state);

  ck_assert_int_eq(state, SPAWN);
}
END_TEST

START_TEST(test_sigact_spawn) {
  game_info_t field;

  init_tetris_game(&field);
  brick_state state = SPAWN;

  sigact_tetris(&field, Nosig, &state);

  ck_assert(state == MOVING || state == GAMEOVER);
}
END_TEST

START_TEST(test_sigact_shifting) {
  game_info_t field;

  init_tetris_game(&field);
  brick_state state = SHIFTING;

  sigact_tetris(&field, Nosig, &state);

  ck_assert(state == MOVING || state == ATTACH);
}
END_TEST

START_TEST(test_sigact_attach) {
  game_info_t field;

  init_tetris_game(&field);
  brick_state state = ATTACH;

  tetris_stats_init(&field, &state);

  sigact_tetris(&field, Nosig, &state);

  ck_assert_int_eq(state, SPAWN);
}
END_TEST

START_TEST(test_gameover_restart) {
  game_info_t field;

  init_tetris_game(&field);
  brick_state state = GAMEOVER;

  on_gameover_state_tetris(&field, Start, &state);

  ck_assert_int_eq(state, START);
}
END_TEST

START_TEST(test_on_gameover_state_escape) {
  game_info_t field;

  init_tetris_game(&field);
  brick_state state = GAMEOVER;

  on_gameover_state_tetris(&field, Terminate, &state);

  ck_assert_int_eq(state, EXIT);
}
END_TEST

START_TEST(test_on_gameover_state) {
  game_info_t field;

  init_tetris_game(&field);
  brick_state state = GAMEOVER;

  on_gameover_state_tetris(&field, Nosig, &state);

  ck_assert_int_eq(state, GAMEOVER);
}
END_TEST

START_TEST(test_move_left_success) {
  game_info_t field;
  init_tetris_game(&field);

  field.curr.type = SHAPE_O;
  load_shape(&field.curr);

  field.curr.pos.x = TETRIS_M / 2;
  field.curr.pos.y = TETRIS_N / 2;

  int x = field.curr.pos.x;

  move_left(&field);

  ck_assert_int_eq(field.curr.pos.x, x - 1);
}
END_TEST

START_TEST(test_move_left_blocked) {
  game_info_t field;
  init_tetris_game(&field);

  field.curr.type = SHAPE_O;
  load_shape(&field.curr);

  field.curr.pos.x = 2;
  field.curr.pos.y = 0;

  field.figures_placed[1][1] = 1;
  field.figures_placed[1][2] = 1;
  field.figures_placed[2][1] = 1;
  field.figures_placed[2][2] = 1;

  int x = field.curr.pos.x;

  move_left(&field);

  ck_assert_int_eq(field.curr.pos.x, x);
}
END_TEST

START_TEST(test_move_right_success) {
  game_info_t field;
  init_tetris_game(&field);

  field.curr.type = SHAPE_O;
  load_shape(&field.curr);

  field.curr.pos.x = TETRIS_M / 2;
  field.curr.pos.y = TETRIS_N / 2;

  int x = field.curr.pos.x;

  move_right(&field);

  ck_assert_int_eq(field.curr.pos.x, x + 1);
}
END_TEST

START_TEST(test_move_right_blocked) {
  game_info_t field;
  init_tetris_game(&field);

  field.curr.type = SHAPE_O;
  load_shape(&field.curr);

  field.curr.pos.x = 2;
  field.curr.pos.y = 0;

  field.figures_placed[1][3] = 1;
  field.figures_placed[1][5] = 1;
  field.figures_placed[2][3] = 1;
  field.figures_placed[2][5] = 1;

  int x = field.curr.pos.x;

  move_right(&field);

  ck_assert_int_eq(field.curr.pos.x, x);
}
END_TEST

START_TEST(test_hard_drop) {
  game_info_t field;
  init_tetris_game(&field);
  brick_state state = MOVING;

  field.curr.type = SHAPE_I;
  load_shape(&field.curr);

  field.curr.pos.x = TETRIS_M / 2 - 2;
  field.curr.pos.y = 0;

  hard_drop(&field, &state);

  ck_assert_int_eq(field.curr.pos.y, TETRIS_N - 3);
  ck_assert_int_eq(state, ATTACH);
}
END_TEST

START_TEST(test_rotate) {
  game_info_t field;
  init_tetris_game(&field);

  field.curr.type = SHAPE_T;
  load_shape(&field.curr);

  field.next.type = SHAPE_T;
  load_shape(&field.next);

  rotate(&field);
  rotate(&field);
  rotate(&field);
  rotate(&field);

  ck_assert_int_eq(
      memcmp(field.next.matrix, field.curr.matrix, sizeof(field.next.matrix)),
      0);
}
END_TEST

START_TEST(test_rotate_square) {
  game_info_t field;
  init_tetris_game(&field);

  field.curr.type = SHAPE_O;
  load_shape(&field.curr);

  field.next.type = SHAPE_O;
  load_shape(&field.next);

  rotate(&field);

  ck_assert_int_eq(
      memcmp(field.next.matrix, field.curr.matrix, sizeof(field.next.matrix)),
      0);
}
END_TEST

START_TEST(test_spawn_figure_success) {
  game_info_t field;
  init_tetris_game(&field);
  brick_state state = SPAWN;

  field.next.type = SHAPE_I;
  load_shape(&field.next);

  spawn_figure(&field, &state);

  ck_assert_int_eq(state, MOVING);
}
END_TEST

START_TEST(test_spawn_figure_gameover) {
  game_info_t field;
  init_tetris_game(&field);
  brick_state state = SPAWN;

  field.curr.type = SHAPE_I;
  load_shape(&field.curr);

  field.curr.pos.x = 5;
  field.curr.pos.y = 0;

  for (int row = 0; row < TETRIS_M; row++) {
    field.figures_placed[0][row] = 1;
  }

  spawn_figure(&field, &state);

  ck_assert_int_eq(state, GAMEOVER);
}
END_TEST

START_TEST(test_on_attach_state) {
  game_info_t field;

  init_tetris_game(&field);
  brick_state state = ATTACH;

  tetris_stats_init(&field, &state);

  on_attach_state_tetris(&field, &state);

  ck_assert_int_eq(state, SPAWN);
}
END_TEST

START_TEST(test_on_exit_state) {
  game_info_t field;

  init_tetris_game(&field);
  brick_state state = EXIT;

  on_exit_state_tetris(&field, &state);

  ck_assert_int_eq(state, EXIT);
}
END_TEST

START_TEST(test_on_spawn_state) {
  game_info_t field;
  init_tetris_game(&field);
  brick_state state = SPAWN;

  on_spawn_state_tetris(&field, &state);

  ck_assert(state == MOVING || state == GAMEOVER);
}
END_TEST

START_TEST(test_on_moving_state_action) {
  game_info_t field;

  init_tetris_game(&field);
  brick_state state = MOVING;

  on_moving_state_tetris(&field, Action, &state);

  ck_assert_int_eq(state, MOVING);
}
END_TEST

START_TEST(test_on_moving_state_pause) {
  game_info_t field;

  init_tetris_game(&field);
  brick_state state = MOVING;

  on_moving_state_tetris(&field, Pause, &state);

  ck_assert_int_eq(state, PAUSED);
}
END_TEST

START_TEST(test_on_moving_state_tetris_Nosig) {
  game_info_t field;

  init_tetris_game(&field);
  brick_state state = MOVING;

  tetris_stats_init(&field, &state);

  on_moving_state_tetris(&field, Nosig, &state);

  ck_assert_int_eq(state, SHIFTING);
}
END_TEST

START_TEST(test_on_moving_state_move_left) {
  game_info_t field;

  init_tetris_game(&field);
  brick_state state = MOVING;

  int x = field.curr.pos.x;

  on_moving_state_tetris(&field, Left, &state);

  ck_assert_int_eq(field.curr.pos.x, x - 1);
}
END_TEST

START_TEST(test_on_moving_state_move_right) {
  game_info_t field;

  init_tetris_game(&field);
  brick_state state = MOVING;

  int x = field.curr.pos.x;

  on_moving_state_tetris(&field, Right, &state);

  ck_assert_int_eq(field.curr.pos.x, x + 1);
}
END_TEST

START_TEST(test_on_moving_state_hard_drop) {
  game_info_t field;

  init_tetris_game(&field);
  brick_state state = MOVING;

  on_moving_state_tetris(&field, Hard_drop, &state);

  ck_assert_int_eq(state, ATTACH);
}
END_TEST

START_TEST(test_on_moving_state_down) {
  game_info_t field;

  init_tetris_game(&field);
  brick_state state = MOVING;

  int y = field.curr.pos.y;

  on_moving_state_tetris(&field, Down, &state);

  ck_assert_int_eq(field.curr.pos.y, y + 1);
  ck_assert_int_eq(state, MOVING);
}
END_TEST

START_TEST(test_on_moving_state_escape) {
  game_info_t field;

  init_tetris_game(&field);
  brick_state state = MOVING;

  on_moving_state_tetris(&field, Terminate, &state);

  ck_assert_int_eq(state, EXIT);
}
END_TEST

START_TEST(test_on_paused_state) {
  brick_state state = PAUSED;

  on_paused_state_tetris(Nosig, &state);

  ck_assert_int_eq(state, PAUSED);
}
END_TEST

START_TEST(test_sigact_moving) {
  game_info_t field;

  init_tetris_game(&field);
  brick_state state = MOVING;

  sigact_tetris(&field, Pause, &state);

  ck_assert_int_eq(state, PAUSED);
}
END_TEST

START_TEST(test_sigact_paused) {
  game_info_t field;

  init_tetris_game(&field);
  brick_state state = PAUSED;

  sigact_tetris(&field, Pause, &state);

  ck_assert_int_eq(state, MOVING);
}
END_TEST

START_TEST(test_sigact_gameover) {
  game_info_t field;

  init_tetris_game(&field);
  brick_state state = GAMEOVER;

  sigact_tetris(&field, Terminate, &state);

  ck_assert_int_eq(state, EXIT);
}
END_TEST

START_TEST(test_sigact_exit) {
  game_info_t field;

  init_tetris_game(&field);
  brick_state state = EXIT;

  sigact_tetris(&field, Nosig, &state);

  ck_assert_int_eq(state, EXIT);
}
END_TEST

START_TEST(test_on_win_state_restart) {
  game_info_t field;

  init_tetris_game(&field);
  brick_state state = WIN;

  on_win_state_tetris(&field, Start, &state);

  ck_assert_int_eq(state, START);
}
END_TEST

START_TEST(test_on_win_state_exit) {
  game_info_t field;

  init_tetris_game(&field);
  brick_state state = WIN;

  on_win_state_tetris(&field, Terminate, &state);

  ck_assert_int_eq(state, EXIT);
}
END_TEST

Suite *fsm_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("fsm");

  tc_core = tcase_create("core");

  tcase_add_test(tc_core, test_user_input_keys);
  tcase_add_test(tc_core, test_start_state);
  tcase_add_test(tc_core, test_pause_state);
  tcase_add_test(tc_core, test_shifting_state_attach);
  tcase_add_test(tc_core, test_sigact_start);
  tcase_add_test(tc_core, test_sigact_spawn);
  tcase_add_test(tc_core, test_sigact_shifting);
  tcase_add_test(tc_core, test_sigact_attach);
  tcase_add_test(tc_core, test_gameover_restart);
  tcase_add_test(tc_core, test_on_gameover_state_escape);
  tcase_add_test(tc_core, test_on_gameover_state);
  tcase_add_test(tc_core, test_move_left_success);
  tcase_add_test(tc_core, test_move_left_blocked);
  tcase_add_test(tc_core, test_move_right_success);
  tcase_add_test(tc_core, test_move_right_blocked);
  tcase_add_test(tc_core, test_hard_drop);
  tcase_add_test(tc_core, test_rotate);
  tcase_add_test(tc_core, test_rotate_square);
  tcase_add_test(tc_core, test_spawn_figure_success);
  tcase_add_test(tc_core, test_spawn_figure_gameover);
  tcase_add_test(tc_core, test_on_attach_state);
  tcase_add_test(tc_core, test_on_exit_state);
  tcase_add_test(tc_core, test_on_spawn_state);
  tcase_add_test(tc_core, test_on_moving_state_action);
  tcase_add_test(tc_core, test_on_moving_state_pause);
  tcase_add_test(tc_core, test_on_moving_state_tetris_Nosig);
  tcase_add_test(tc_core, test_on_moving_state_move_left);
  tcase_add_test(tc_core, test_on_moving_state_move_right);
  tcase_add_test(tc_core, test_on_moving_state_hard_drop);
  tcase_add_test(tc_core, test_on_moving_state_down);
  tcase_add_test(tc_core, test_on_moving_state_escape);
  tcase_add_test(tc_core, test_on_paused_state);
  tcase_add_test(tc_core, test_sigact_moving);
  tcase_add_test(tc_core, test_sigact_paused);
  tcase_add_test(tc_core, test_sigact_gameover);
  tcase_add_test(tc_core, test_sigact_exit);
  tcase_add_test(tc_core, test_on_win_state_restart);
  tcase_add_test(tc_core, test_on_win_state_exit);

  suite_add_tcase(s, tc_core);

  return s;
}
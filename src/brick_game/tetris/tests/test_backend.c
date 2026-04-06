#include "../../../include/fsm.h"
#include "../include/tetris_backend.h"
#include "../include/tetris_shapes.h"
#include "tests.h"

START_TEST(test_level_up_1) {
  brick_state state = START;
  brick_stats_t stats;

  tetris_stats_init(&stats, &state);

  ck_assert_int_eq(stats.level, 1);
  ck_assert_int_eq(stats.score, 0);
}
END_TEST

START_TEST(test_level_up_3) {
  brick_state state = START;
  brick_stats_t stats;

  tetris_stats_init(&stats, &state);
  stats.score = 1200;
  level_up(&stats);

  ck_assert_int_eq(stats.level, 3);
}
END_TEST

START_TEST(test_add_score_1_line) {
  brick_state state = START;
  brick_stats_t stats;

  tetris_stats_init(&stats, &state);
  add_score(1, &stats, &state);

  ck_assert_int_eq(stats.score, 100);
}
END_TEST

START_TEST(test_add_score_2_lines) {
  brick_state state = START;
  brick_stats_t stats;

  tetris_stats_init(&stats, &state);
  add_score(2, &stats, &state);

  ck_assert_int_eq(stats.score, 300);
}
END_TEST

START_TEST(test_add_score_3_lines) {
  brick_state state = START;
  brick_stats_t stats;

  tetris_stats_init(&stats, &state);
  add_score(3, &stats, &state);

  ck_assert_int_eq(stats.score, 700);
}
END_TEST

START_TEST(test_add_score_4_lines) {
  brick_state state = START;
  brick_stats_t stats;

  tetris_stats_init(&stats, &state);
  add_score(4, &stats, &state);

  ck_assert_int_eq(stats.score, 1500);
}
END_TEST

START_TEST(test_add_score_win_instant) {
  brick_state state = START;
  brick_stats_t stats;

  tetris_stats_init(&stats, &state);

  for (int i = 0; i < 4; i++) {
    add_score(4, &stats, &state);
  }

  ck_assert_int_eq(stats.score, 6000);
  ck_assert_int_eq(stats.level, 10);
  ck_assert_int_eq(stats.won, TRUE);
}
END_TEST

START_TEST(test_check_collide_empty) {
  brick_stats_t field;
  init_tetris_game(&field);

  bool result = check_collide(field.field, field.curr.pos.x,
                              field.curr.pos.y * 2, field.curr.matrix);

  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_check_collide_wall) {
  brick_stats_t field;
  init_tetris_game(&field);

  bool result = check_collide(field.field, field.curr.pos.x - TETRIS_N,
                              field.curr.pos.y * 2, field.curr.matrix);

  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_check_collide_floor) {
  brick_stats_t field;
  init_tetris_game(&field);

  bool result =
      check_collide(field.field, field.curr.pos.x, TETRIS_N, field.curr.matrix);

  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_figure_dropping_true) {
  brick_stats_t field;
  init_tetris_game(&field);

  bool result = figure_dropping(&field);

  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_figure_dropping_false) {
  brick_stats_t field;
  init_tetris_game(&field);

  field.curr.pos.x = TETRIS_M / 2 - 2;
  field.curr.pos.y = TETRIS_N - 3;

  bool result = figure_dropping(&field);

  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_shift_down) {
  brick_stats_t field;
  init_tetris_game(&field);

  for (int col = 0; col < TETRIS_M; col++) {
    field.figures_placed[1][col] = 1;
  }

  shift_down(&field, 1);

  for (int col = 0; col < TETRIS_M; col++) {
    ck_assert_int_eq(field.figures_placed[1][col], 0);
  }
}
END_TEST

START_TEST(test_is_square) {
  ck_assert_int_eq(is_square(SHAPE_O), 1);
  ck_assert_int_eq(is_square(SHAPE_I), 0);
}
END_TEST

START_TEST(test_transpose_matrix) {
  int matrix[SHAPE_H][SHAPE_W] = {{0, 0, 0, 0, 0},
                                  {0, 0, 1, 0, 0},
                                  {0, 1, 1, 1, 0},
                                  {0, 0, 0, 0, 0},
                                  {0, 0, 0, 0, 0}};

  int result[SHAPE_H][SHAPE_W] = {0};

  transpose_matrix(matrix, result);

  int expected[SHAPE_W][SHAPE_H] = {{0, 0, 0, 0, 0},
                                    {0, 0, 1, 0, 0},
                                    {0, 0, 1, 1, 0},
                                    {0, 0, 1, 0, 0},
                                    {0, 0, 0, 0, 0}};

  for (int row = 0; row < SHAPE_H; row++) {
    for (int col = 0; col < SHAPE_W; col++) {
      ck_assert_int_eq(result[row][col], expected[row][col]);
    }
  }
}
END_TEST

START_TEST(test_stamp_figure) {
  brick_stats_t field;
  init_tetris_game(&field);
  int target[TETRIS_N][TETRIS_M] = {0};

  /* SHAPE_S */
  field.curr.type = SHAPE_S;
  load_shape(&field.curr);

  field.curr.pos.x = 0;
  field.curr.pos.y = 0;

  stamp_figure(&field, target);

  ck_assert_int_eq(target[1][2], 1);
  ck_assert_int_eq(target[1][3], 1);
  ck_assert_int_eq(target[2][1], 1);
  ck_assert_int_eq(target[2][2], 1);
}
END_TEST

START_TEST(test_stamp_figure_out_of_bounds) {
  brick_stats_t field;
  init_tetris_game(&field);

  int target[TETRIS_N][TETRIS_M] = {0};

  field.curr.type = SHAPE_Z;
  load_shape(&field.curr);

  field.curr.pos.x = -5;
  field.curr.pos.y = -5;

  stamp_figure(&field, target);

  for (int row = 0; row < TETRIS_N; row++) {
    for (int col = 0; col < TETRIS_M; col++) {
      ck_assert_int_eq(target[row][col], 0);
    }
  }
}
END_TEST

START_TEST(test_lock_figure) {
  brick_stats_t field;
  init_tetris_game(&field);

  field.curr.type = SHAPE_J;
  load_shape(&field.curr);

  field.curr.pos.x = TETRIS_M / 2 - 2;
  field.curr.pos.y = TETRIS_N / 3;

  lock_figure(&field);

  ck_assert_int_eq(field.figures_placed[7][4], 1);
  ck_assert_int_eq(field.figures_placed[8][4], 1);
  ck_assert_int_eq(field.figures_placed[8][5], 1);
  ck_assert_int_eq(field.figures_placed[8][6], 1);
}
END_TEST

START_TEST(test_overlay_active_figure) {
  brick_stats_t field;
  init_tetris_game(&field);

  field.curr.type = SHAPE_L;
  load_shape(&field.curr);

  field.curr.pos.x = TETRIS_M / 2 - 2;
  field.curr.pos.y = TETRIS_N / 3;

  overlay_active_figure(&field, true);

  ck_assert_int_eq(field.field[7][6], 1);
  ck_assert_int_eq(field.field[8][4], 1);
  ck_assert_int_eq(field.field[8][5], 1);
  ck_assert_int_eq(field.field[8][6], 1);
}
END_TEST

START_TEST(test_not_overlay_active_figure) {
  brick_stats_t field;
  init_tetris_game(&field);

  field.curr.type = SHAPE_L;
  load_shape(&field.curr);

  field.curr.pos.x = TETRIS_M / 2 - 2;
  field.curr.pos.y = TETRIS_N / 3;

  overlay_active_figure(&field, false);

  ck_assert_int_eq(field.field[8][6], 0);
  ck_assert_int_eq(field.field[9][4], 0);
  ck_assert_int_eq(field.field[9][5], 0);
  ck_assert_int_eq(field.field[9][6], 0);
}
END_TEST

START_TEST(test_update_field) {
  brick_stats_t field;
  init_tetris_game(&field);

  /* SHAPE_J */
  field.field[6][6] = 1;
  field.field[7][6] = 1;
  field.field[7][7] = 1;
  field.field[7][8] = 1;

  update_field(&field);

  ck_assert_int_eq(field.field[6][6], 0);
  ck_assert_int_eq(field.field[7][6], 0);
  ck_assert_int_eq(field.field[7][7], 0);
  ck_assert_int_eq(field.field[7][8], 0);
}
END_TEST

START_TEST(test_build_field) {
  brick_stats_t field;
  init_tetris_game(&field);

  field.curr.type = SHAPE_T;
  load_shape(&field.curr);

  field.curr.pos.x = TETRIS_M / 2 - 2;
  field.curr.pos.y = TETRIS_N / 3;

  stamp_figure(&field, field.figures_placed);
  build_field(&field);

  ck_assert_int_eq(field.field[7][5], 1);
  ck_assert_int_eq(field.field[8][4], 1);
  ck_assert_int_eq(field.field[8][5], 1);
  ck_assert_int_eq(field.field[8][6], 1);
}
END_TEST

START_TEST(test_init_tetris_game) {
  brick_stats_t field;
  init_tetris_game(&field);

  for (int i = 0; i < TETRIS_N; i++) {
    for (int j = 0; j < TETRIS_M; j++) {
      ck_assert_int_eq(field.field[i][j], 0);
    }
  }

  ck_assert_int_ge(field.curr.type, 0);
  ck_assert_int_lt(field.curr.type, SHAPE_COUNT);
}
END_TEST

Suite *backend_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("tetris_backend");

  tc_core = tcase_create("core");

  tcase_add_test(tc_core, test_level_up_1);
  tcase_add_test(tc_core, test_level_up_3);
  tcase_add_test(tc_core, test_add_score_1_line);
  tcase_add_test(tc_core, test_add_score_2_lines);
  tcase_add_test(tc_core, test_add_score_3_lines);
  tcase_add_test(tc_core, test_add_score_4_lines);
  tcase_add_test(tc_core, test_add_score_win_instant);
  tcase_add_test(tc_core, test_check_collide_empty);
  tcase_add_test(tc_core, test_check_collide_wall);
  tcase_add_test(tc_core, test_check_collide_floor);
  tcase_add_test(tc_core, test_figure_dropping_true);
  tcase_add_test(tc_core, test_figure_dropping_false);
  tcase_add_test(tc_core, test_shift_down);
  tcase_add_test(tc_core, test_is_square);
  tcase_add_test(tc_core, test_transpose_matrix);
  tcase_add_test(tc_core, test_stamp_figure);
  tcase_add_test(tc_core, test_stamp_figure_out_of_bounds);
  tcase_add_test(tc_core, test_lock_figure);
  tcase_add_test(tc_core, test_overlay_active_figure);
  tcase_add_test(tc_core, test_not_overlay_active_figure);
  tcase_add_test(tc_core, test_update_field);
  tcase_add_test(tc_core, test_build_field);
  tcase_add_test(tc_core, test_init_tetris_game);

  suite_add_tcase(s, tc_core);

  return s;
}
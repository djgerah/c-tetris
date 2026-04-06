#include "../include/tetris_backend.h"
#include "../include/tetris_shapes.h"
#include "tests.h"

START_TEST(test_rand_shape) {
  for (int i = 0; i < 1000; i++) {
    int shape = rand_shape();

    ck_assert_int_ge(shape, 0);
    ck_assert_int_lt(shape, SHAPE_COUNT);
  }
}
END_TEST

START_TEST(test_load_shape) {
  figure_t figure;

  for (int type = 0; type < SHAPE_COUNT; type++) {
    figure.type = type;
    load_shape(&figure);

    for (int r = 0; r < SHAPE_H; r++) {
      for (int c = 0; c < SHAPE_W; c++) {
        ck_assert_int_eq(figure.matrix[r][c], SHAPES[type][r][c]);
      }
    }
  }
}
END_TEST

START_TEST(test_init_figures) {
  game_info_t field;
  init_tetris_game(&field);

  field.curr.type = SHAPE_O;
  field.next.type = SHAPE_T;

  init_figures(&field.curr, &field.next);

  ck_assert_int_eq(field.curr.pos.x, TETRIS_M / 2 - 2);
  ck_assert_int_eq(field.curr.pos.y, -2);
  ck_assert_int_ge(field.next.type, 2);
  ck_assert_int_lt(field.next.type, SHAPE_COUNT);
}
END_TEST

Suite *shapes_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("tetris_shapes");

  tc_core = tcase_create("core");

  tcase_add_test(tc_core, test_rand_shape);
  tcase_add_test(tc_core, test_load_shape);
  tcase_add_test(tc_core, test_init_figures);

  suite_add_tcase(s, tc_core);

  return s;
}
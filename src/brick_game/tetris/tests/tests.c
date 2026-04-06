#include "tests.h"

#include <stdlib.h>

int run_testcase(Suite *testcase) {
  int number_failed = 0;

  SRunner *sr = srunner_create(testcase);
  srunner_run_all(sr, CK_NORMAL);

  number_failed += srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0 ? 0 : 1);
}

int main(void) {
  int number_failed = 0;

  Suite *test_suites[] = {shapes_suite(), backend_suite(), fsm_suite(), NULL};

  Suite **current_suite = test_suites;

  while (*current_suite != NULL) {
    number_failed += run_testcase(*current_suite);
    current_suite++;
  }

  return (number_failed == 0 ? 0 : 1);
}
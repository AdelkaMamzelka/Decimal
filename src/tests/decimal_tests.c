#include "decimal_tests.h"

int main(void) {
  run_tests();
  return 0;
}

void run_tests(void) {
  Suite* list_cases[] = {suite_main_functions_tests(),
                         suite_from_int_to_decimal(),
                         suite_from_decimal_to_int(),
                         suite_from_decimal_to_float(),
                         suite_mul(),
                         add_cases(),
                         sub_cases(),
                         floor_suite(),
                         round_suite(),
                         suite_truncate(),
                         negate_suite(),
                         suite_is_equal_test(),
                         suite_is_not_equal_test(),
                         suite_is_greater_test(),
                         suite_is_greater_or_equal_test(),
                         suite_is_less_test(),
                         suite_is_less_or_equal_test(),
                         NULL};
  for (Suite** current_testcase = list_cases; *current_testcase != NULL;
       current_testcase++) {
    run_testcase(*current_testcase);
  }
}

void run_testcase(Suite* testcase) {
  static int counter_testcase = 1;
  if (counter_testcase > 1) putchar('\n');
  printf("CURRENT TEST: %d\n", counter_testcase);
  counter_testcase++;

  SRunner* sr = srunner_create(testcase);
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);
  srunner_free(sr);
}

START_TEST(mul_test) {
  decimal dec_1 = {{0, 0, 0, 0}};
  dec_1.bits[0] = dec_1.bits[1] = dec_1.bits[2] = UINT_MAX;
  decimal dec_2 = {{2, 0, 0, 0}};
  decimal result = {{0, 0, 0, 0}};
  decimal dec_2_minus = {{2, 0, 0, 0}};  // -2
  set_bit(1, 127, &dec_2_minus);

  ck_assert_int_eq(mul(dec_1, dec_2, &result), 1);        // +infinity
  ck_assert_int_eq(mul(dec_1, dec_2_minus, &result), 2);  // -infinity
  ck_assert_int_eq(mul(dec_2, dec_2_minus, &result), 0);

  big_decimal big_result = {{0, 0, 0, 0, 0, 0, 0}};
  big_decimal result_2_expected = {{4, 0, 0, 0, 0, 0, 0}};
  set_big(1, 225, &result_2_expected);  // -4
  decimal_to_big_decimal(result, &big_result);
  ck_assert_int_eq(sravni_mant(result_2_expected, result_2_expected), 0);
}
END_TEST

Suite* suite_main_functions_tests(void) {
  Suite* s = suite_create("main_functions_tests");
  TCase* tc = tcase_create("main_functions_tests_tc");

  tcase_add_test(tc, mul_test);

  suite_add_tcase(s, tc);
  return s;
}

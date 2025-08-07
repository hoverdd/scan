#include <check.h>
#include "config.h"

START_TEST(test_validate_port_range_valid)
{
    ck_assert_int_eq(validate_port_range(1, 65535), 1);
    ck_assert_int_eq(validate_port_range(80, 80), 1);
}
END_TEST

START_TEST(test_validate_port_range_invalid)
{
    ck_assert_int_eq(validate_port_range(0, 65535), 0);
    ck_assert_int_eq(validate_port_range(1, 70000), 0);
    ck_assert_int_eq(validate_port_range(100, 50), 0);
}
END_TEST

Suite *config_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Config");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_validate_port_range_valid);
    tcase_add_test(tc_core, test_validate_port_range_invalid);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = config_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? 0 : 1;
}
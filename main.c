#include <stdlib.h>
#include <check.h>

#include <fontconfig/fontconfig.h>


Suite* matrix_suite();
Suite* range_suite();
Suite* weight_suite();

START_TEST (test_bla_ble)
{
    /* unit test code */


    ck_assert(5 > 1);

//     ck_assert(5 < 1);
}
END_TEST

Suite* bla_suite()
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Bla");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_bla_ble);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(int argc, char **argv) {

    FcInit(); // TODO: Reinitalize for every test?

    Suite *s = bla_suite();
    SRunner *sr = srunner_create(s);

    srunner_add_suite(sr, matrix_suite());
    srunner_add_suite(sr, range_suite());
    srunner_add_suite(sr, weight_suite());

    srunner_run_all(sr, CK_NORMAL);
    int number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    FcFini(); // TODO: Reinitalize for every test?

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

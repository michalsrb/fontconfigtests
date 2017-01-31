#include <check.h>
#include <limits.h>
#include <stddef.h>

#include <fontconfig/fontconfig.h>

START_TEST (test_range_create_double_destroy)
{
    FcRange *range;

    // Normal range
    range = FcRangeCreateDouble(0.0, 1.0);
    ck_assert(range != NULL);
    FcRangeDestroy(range);

    // Single number range
    range = FcRangeCreateDouble(0.0, 0.0);
    ck_assert(range != NULL);
    FcRangeDestroy(range);

    // Negative range
    // TODO: Shouldn't this fail?
    range = FcRangeCreateDouble(1.0, 0.0);
    ck_assert(range != NULL);
    FcRangeDestroy(range);
}
END_TEST

START_TEST (test_range_create_integer_destroy)
{
    FcRange *range;

    // Normal range
    range = FcRangeCreateInteger(0, 1);
    ck_assert(range != NULL);
    FcRangeDestroy(range);

    // Single number range
    range = FcRangeCreateInteger(0, 0);
    ck_assert(range != NULL);
    FcRangeDestroy(range);

    // Negative range
    // TODO: Shouldn't this fail?
    range = FcRangeCreateInteger(1, 0);
    ck_assert(range != NULL);
    FcRangeDestroy(range);
}
END_TEST

START_TEST (test_range_get_double_from_double)
{
    FcRange *range;
    double begin, end;

    // TODO: Do not compare doubles with == ?

    range = FcRangeCreateDouble(0.0, 1.0);
    ck_assert(FcRangeGetDouble(range, &begin, &end));
    ck_assert(begin == 0.0);
    ck_assert(end == 1.0);
    FcRangeDestroy(range);
}
END_TEST

START_TEST (test_range_get_double_from_integer)
{
    FcRange *range;
    double begin, end;

    range = FcRangeCreateInteger(0, 1);
    ck_assert(FcRangeGetDouble(range, &begin, &end));
    ck_assert(begin == 0.0);
    ck_assert(end == 1.0);
    FcRangeDestroy(range);
}
END_TEST

START_TEST (test_range_get_double_unusual)
{
    FcRange *range;
    double begin, end;

    range = FcRangeCreateInteger(0, 1);

    ck_assert(FcRangeGetDouble(range, &begin, &end));
    ck_assert(begin == 0.0);
    ck_assert(end == 1.0);

    ck_assert(FcRangeGetDouble(range, NULL, &end));
    ck_assert(end == 1.0);

    ck_assert(FcRangeGetDouble(range, &begin, NULL));
    ck_assert(begin == 0.0);

    ck_assert(FcRangeGetDouble(range, NULL, NULL));

    ck_assert(!FcRangeGetDouble(NULL, NULL, NULL));

    FcRangeDestroy(range);
}
END_TEST

START_TEST (test_range_copy)
{
    FcRange *range = FcRangeCreateDouble(0.0, 1.0);
    FcRange *range_copy = FcRangeCopy(range);

    ck_assert(range_copy);

    double begin, end;
    FcRangeGetDouble(range_copy, &begin, &end);
    ck_assert(begin == 0.0);
    ck_assert(end == 1.0);

    FcRangeDestroy(range);
    FcRangeDestroy(range_copy);
}
END_TEST

Suite* range_suite()
{
    Suite *s = suite_create("FcRange");

    TCase *tc_range_create = tcase_create("FcRangeCreate+FcRangeDestroy");
    tcase_add_test(tc_range_create, test_range_create_double_destroy);
    tcase_add_test(tc_range_create, test_range_create_integer_destroy);
    suite_add_tcase(s, tc_range_create);

    TCase *tc_range_get_double = tcase_create("FcRangeGetDouble");
    tcase_add_test(tc_range_get_double, test_range_get_double_from_double);
    tcase_add_test(tc_range_get_double, test_range_get_double_from_integer);
    tcase_add_test(tc_range_get_double, test_range_get_double_unusual);
    suite_add_tcase(s, tc_range_get_double);

    TCase *tc_range_copy = tcase_create("FcRangeCopy");
    tcase_add_test(tc_range_copy, test_range_copy);
    suite_add_tcase(s, tc_range_copy);

    return s;
}

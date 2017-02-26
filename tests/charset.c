#include <check.h>
#include <limits.h>
#include <stddef.h>

#include <fontconfig/fontconfig.h>

START_TEST (test_charset_create_destroy)
{
    FcCharSet *charset = FcCharSetCreate();

    ck_assert(charset != null);

    FcCharSetDestroy(charset);
}
END_TEST

Suite* range_suite()
{
    Suite *s = suite_create("FcCharSet");

    TCase *tc_range_create = tcase_create("FcCharSetCreate+FcCharSetDestroy");
    tcase_add_test(tc_range_create, test_charset_create_destroy);
    suite_add_tcase(s, tc_range_create);

    return s;
}

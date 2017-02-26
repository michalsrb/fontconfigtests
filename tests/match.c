#include <check.h>
#include <limits.h>
#include <stddef.h>

#include <fontconfig/fontconfig.h>

Suite* match_suite()
{
    Suite *s = suite_create("FcFontMatch");

    return s;
}

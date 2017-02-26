#include <check.h>
#include <limits.h>

#include <fontconfig/fontconfig.h>


// FontConfig weights ordered in increasing boldness
static const int fc_weights[] = {
  FC_WEIGHT_THIN,
  FC_WEIGHT_EXTRALIGHT,
  FC_WEIGHT_ULTRALIGHT,
  FC_WEIGHT_LIGHT,
  FC_WEIGHT_DEMILIGHT,
  FC_WEIGHT_SEMILIGHT,
  FC_WEIGHT_BOOK,
  FC_WEIGHT_REGULAR,
  FC_WEIGHT_NORMAL,
  FC_WEIGHT_MEDIUM,
  FC_WEIGHT_DEMIBOLD,
  FC_WEIGHT_SEMIBOLD,
  FC_WEIGHT_BOLD,
  FC_WEIGHT_EXTRABOLD,
  FC_WEIGHT_ULTRABOLD,
  FC_WEIGHT_BLACK,
  FC_WEIGHT_HEAVY,
  FC_WEIGHT_EXTRABLACK,
  FC_WEIGHT_ULTRABLACK
};

START_TEST (test_weight_from_opentype)
{
    // TODO: FcWeightFromOpenType handles numbers 1...9 differently than the remaining 10...1000, find out why!

    int ot_weight = _i;
    int fc_weight = FcWeightFromOpenType(ot_weight);

    // Make sure that the result is between THIN and ULTRABLACK
    ck_assert_int_ge(fc_weight, FC_WEIGHT_THIN);
    ck_assert_int_le(fc_weight, FC_WEIGHT_ULTRABLACK);

    if (ot_weight > 1) {
        int previous_fc_weight = FcWeightFromOpenType(ot_weight - 1);

        // Make sure that the result is bigger or equal to previous result
        ck_assert_int_ge(fc_weight, previous_fc_weight);
    }
}
END_TEST

START_TEST (test_weight_from_opentype_out_of_range)
{
    ck_assert_int_eq(FcWeightFromOpenType(INT_MIN), -1);
    ck_assert_int_eq(FcWeightFromOpenType(-10), -1);
    ck_assert_int_eq(FcWeightFromOpenType(-1), -1);
    ck_assert_int_eq(FcWeightFromOpenType(0), -1); // TODO: This fails, it is in conflict with FcWeightFromOpenType documentation!

    ck_assert_int_eq(FcWeightFromOpenType(1001), -1);
    ck_assert_int_eq(FcWeightFromOpenType(INT_MAX), -1);
}
END_TEST


START_TEST (test_weight_to_opentype)
{
    int ot_weight = FcWeightToOpenType(fc_weights[_i]);

    // Make sure that the result is between 1 and 1000
    ck_assert_int_ge(ot_weight, 1);
    ck_assert_int_le(ot_weight, 1000);

    if (_i > 0) {
        int prev_ot_weight = FcWeightToOpenType(fc_weights[_i - 1]);

        // Make sure that the result is bigger or equal to previous result
        ck_assert_int_ge(ot_weight, prev_ot_weight);
    }
}
END_TEST

START_TEST (test_weight_to_opentype_less_than_thin)
{
    ck_assert_int_eq(FcWeightToOpenType(FC_WEIGHT_THIN - 1), -1);
    ck_assert_int_eq(FcWeightToOpenType(FC_WEIGHT_THIN - 10), -1);
    ck_assert_int_eq(FcWeightToOpenType(FC_WEIGHT_THIN - 100), -1);
    ck_assert_int_eq(FcWeightToOpenType(INT_MIN), -1);
}
END_TEST

START_TEST (test_weight_to_opentype_more_than_extrablack)
{
    ck_assert_int_eq(FcWeightToOpenType(FC_WEIGHT_EXTRABLACK + 1), -1);
    ck_assert_int_eq(FcWeightToOpenType(FC_WEIGHT_EXTRABLACK + 10), -1);
    ck_assert_int_eq(FcWeightToOpenType(FC_WEIGHT_EXTRABLACK + 100), -1);
    ck_assert_int_eq(FcWeightToOpenType(INT_MAX), -1);
}
END_TEST

START_TEST (test_weight_to_from_opentype)
{
    int fc_weight = fc_weights[_i];
    int ot_weight = FcWeightToOpenType(fc_weight);
    int fc_weight_again = FcWeightFromOpenType(ot_weight);

    ck_assert_int_eq(fc_weight, fc_weight_again);
}
END_TEST

Suite* weight_suite()
{
    Suite *s = suite_create("FcWeight");

    TCase *tc_weight_from_opentype = tcase_create("FcWeightFromOpenType");
    tcase_add_loop_test(tc_weight_from_opentype, test_weight_from_opentype, 1, 1000);
    tcase_add_test(tc_weight_from_opentype, test_weight_from_opentype_out_of_range);
    suite_add_tcase(s, tc_weight_from_opentype);

    TCase *tc_weight_to_opentype = tcase_create("FcWeightToOpenType");
    tcase_add_loop_test(tc_weight_to_opentype, test_weight_to_opentype, 0, sizeof(fc_weights) / sizeof(fc_weights[0]) - 1);
    tcase_add_test(tc_weight_to_opentype, test_weight_to_opentype_less_than_thin);
    tcase_add_test(tc_weight_to_opentype, test_weight_to_opentype_more_than_extrablack);
    suite_add_tcase(s, tc_weight_to_opentype);

    TCase *tc_weight_to_from_opentype = tcase_create("FcWeightToFromOpenType");
    tcase_add_loop_test(tc_weight_to_from_opentype, test_weight_to_from_opentype, 0, sizeof(fc_weights) / sizeof(fc_weights[0]) - 1);
    suite_add_tcase(s, tc_weight_to_from_opentype);

    return s;
}

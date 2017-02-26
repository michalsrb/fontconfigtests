#include <check.h>
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>

#include <fontconfig/fontconfig.h>

START_TEST (test_matrix_init)
{
    FcMatrix matrix;

    FcMatrixInit(&matrix);

    ck_assert(matrix.xx == 1.0);
    ck_assert(matrix.xy == 0.0);
    ck_assert(matrix.yx == 0.0);
    ck_assert(matrix.yy == 1.0);
}
END_TEST

START_TEST (test_matrix_copy)
{
    FcMatrix matrix;

    matrix.xx = 2.0;
    matrix.xy = 3.0;
    matrix.yx = 4.0;
    matrix.yy = 5.0;

    FcMatrix *matrix_copy = FcMatrixCopy(&matrix);

    ck_assert(matrix_copy);

    ck_assert(matrix.xx == matrix_copy->xx);
    ck_assert(matrix.xy == matrix_copy->xy);
    ck_assert(matrix.yx == matrix_copy->yx);
    ck_assert(matrix.yy == matrix_copy->yy);

    //FcMatrixFree(matrix_copy);
    free(matrix_copy); // TODO: Why is FcMatrixFree not exposed?
}
END_TEST

START_TEST (test_matrix_copy_null)
{
    ck_assert(FcMatrixCopy(NULL) == NULL);
}
END_TEST

START_TEST (test_matrix_equal)
{
    FcMatrix matrix;

    matrix.xx = 2.0;
    matrix.xy = 3.0;
    matrix.yx = 4.0;
    matrix.yy = 5.0;

    FcMatrix *matrix_copy;

    matrix_copy = FcMatrixCopy(&matrix);
    ck_assert(FcMatrixEqual(&matrix, matrix_copy));
    free(matrix_copy);

    matrix_copy = FcMatrixCopy(&matrix);
    matrix_copy->xx = -1.0;
    ck_assert(!FcMatrixEqual(&matrix, matrix_copy));
    free(matrix_copy);

    matrix_copy = FcMatrixCopy(&matrix);
    matrix_copy->xy = -1.0;
    ck_assert(!FcMatrixEqual(&matrix, matrix_copy));
    free(matrix_copy);

    matrix_copy = FcMatrixCopy(&matrix);
    matrix_copy->yx = -1.0;
    ck_assert(!FcMatrixEqual(&matrix, matrix_copy));
    free(matrix_copy);

    matrix_copy = FcMatrixCopy(&matrix);
    matrix_copy->yy = -1.0;
    ck_assert(!FcMatrixEqual(&matrix, matrix_copy));
    free(matrix_copy);

    ck_assert(FcMatrixEqual(NULL, NULL));

    ck_assert(!FcMatrixEqual(&matrix, NULL));
    ck_assert(!FcMatrixEqual(NULL, &matrix));
}
END_TEST

START_TEST (test_matrix_multiply)
{
    FcMatrix a, b, c;

    a.xx = 2.0;
    a.xy = 3.0;
    a.yx = 4.0;
    a.yy = 5.0;

    b.xx = 6.0;
    b.xy = 7.0;
    b.yx = 8.0;
    b.yy = 9.0;

    FcMatrixMultiply(&c, &a, &b);

    ck_assert(c.xx == a.xx * b.xx + a.xy * b.yx);
    ck_assert(c.xy == a.xx * b.xy + a.xy * b.yy);
    ck_assert(c.yx == a.yx * b.xx + a.yy * b.yx);
    ck_assert(c.yy == a.yx * b.xy + a.yy * b.yy);

    FcMatrixMultiply(&a, &a, &b);

    ck_assert(FcMatrixEqual(&a, &c));
}
END_TEST

START_TEST (test_matrix_rotate)
{
    FcMatrix matrix;

    matrix.xx = 2.0;
    matrix.xy = 3.0;
    matrix.yx = 4.0;
    matrix.yy = 5.0;

    double sin = 1.0;
    double cos = 0.0;
    FcMatrixRotate(&matrix, cos, sin);

    ck_assert(matrix.xx == -4.0);
    ck_assert(matrix.xy == -5.0);
    ck_assert(matrix.yx == 2.0);
    ck_assert(matrix.yy == 3.0);
}
END_TEST

START_TEST (test_matrix_scale)
{
    FcMatrix matrix;

    matrix.xx = 2.0;
    matrix.xy = 3.0;
    matrix.yx = 4.0;
    matrix.yy = 5.0;

    FcMatrixScale(&matrix, 2.0, 0.5);

    ck_assert(matrix.xx == 4.0);
    ck_assert(matrix.xy == 6.0);
    ck_assert(matrix.yx == 2.0);
    ck_assert(matrix.yy == 2.5);
}
END_TEST

#include <stdio.h>

START_TEST (test_matrix_shear)
{
    FcMatrix matrix;

    matrix.xx = 2.0;
    matrix.xy = 3.0;
    matrix.yx = 4.0;
    matrix.yy = 5.0;

    FcMatrixShear(&matrix, 2.0, 0.5);

    ck_assert(matrix.xx == 10.0);
    ck_assert(matrix.xy == 13.0);
    ck_assert(matrix.yx == 5.0);
    ck_assert(matrix.yy == 6.5);

}
END_TEST

Suite* matrix_suite()
{
    Suite *s = suite_create("FcMatrix");

    TCase *tc_matrix_init = tcase_create("FcMatrixInit");
    tcase_add_test(tc_matrix_init, test_matrix_init);
    suite_add_tcase(s, tc_matrix_init);

    TCase *tc_matrix_copy = tcase_create("FcMatrixCopy");
    tcase_add_test(tc_matrix_copy, test_matrix_copy);
    tcase_add_test(tc_matrix_copy, test_matrix_copy_null);
    suite_add_tcase(s, tc_matrix_copy);

    TCase *tc_matrix_equal = tcase_create("FcMatrixEqual");
    tcase_add_test(tc_matrix_equal, test_matrix_equal);
    suite_add_tcase(s, tc_matrix_equal);

    TCase *tc_matrix_multiply = tcase_create("FcMatrixMultiply");
    tcase_add_test(tc_matrix_multiply, test_matrix_multiply);
    suite_add_tcase(s, tc_matrix_multiply);

    TCase *tc_matrix_rotate = tcase_create("FcMatrixRotate");
    tcase_add_test(tc_matrix_rotate, test_matrix_rotate);
    suite_add_tcase(s, tc_matrix_rotate);

    TCase *tc_matrix_scale = tcase_create("FcMatrixScale");
    tcase_add_test(tc_matrix_scale, test_matrix_scale);
    suite_add_tcase(s, tc_matrix_scale);

    TCase *tc_matrix_shear = tcase_create("FcMatrixShear");
    tcase_add_test(tc_matrix_shear, test_matrix_shear);
    suite_add_tcase(s, tc_matrix_shear);

    return s;
}

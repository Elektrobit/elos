// SPDX-License-Identifier: MIT
#include "safuVecPop_utest.h"

int main() {
    const struct CMUnitTest tests[] = {cmocka_unit_test(safuTestSafuVecPopSuccess),
                                       cmocka_unit_test(safuTestSafuVecPopEmpty),
                                       cmocka_unit_test(safuTestSafuVecPopErrUninitialized)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}

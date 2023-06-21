// SPDX-License-Identifier: MIT
#include "safuLogFunc_utest.h"

int main() {
    const struct CMUnitTest tests[] = {cmocka_unit_test(safuTestSafuLogFuncSuccess),
                                       cmocka_unit_test(safuTestSafuLogFuncErrIntSize)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}

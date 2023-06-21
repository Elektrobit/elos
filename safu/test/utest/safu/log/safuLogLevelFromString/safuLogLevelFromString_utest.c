// SPDX-License-Identifier: MIT
#include "safuLogLevelFromString_utest.h"

int main() {
    const struct CMUnitTest tests[] = {cmocka_unit_test(safuTestSafuLogLevelFromStringSuccess),
                                       cmocka_unit_test(safuTestSafuLogLevelFromStringErrInvalidLogLevel)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}

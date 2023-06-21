// SPDX-License-Identifier: MIT
#include "safuLogLevelToString_utest.h"

int main() {
    const struct CMUnitTest tests[] = {cmocka_unit_test(safuTestSafuLogLevelToStringSuccess),
                                       cmocka_unit_test(safuTestSafuLogLevelToStringErrInvalidLogLevel)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}

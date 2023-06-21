// SPDX-License-Identifier: MIT
#include "safuLogSetFilterStr_utest.h"

int main() {
    const struct CMUnitTest tests[] = {cmocka_unit_test(safuTestSafuLogSetFilterStringSuccess),
                                       cmocka_unit_test(safuTestSafuLogSetFilterStringErrToLongFilter)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}

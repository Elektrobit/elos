// SPDX-License-Identifier: MIT
#include "safuVecInsert_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(safuTestSafuVecInsertConsecutiveWithResize),
        cmocka_unit_test(safuTestSafuVecInsertConsecutiveWithoutResize),
        cmocka_unit_test(safuTestSafuVecInsertWithResize),
        cmocka_unit_test(safuTestSafuVecInsertWithoutResize),
        cmocka_unit_test(safuTestSafuVecInsertFrontWithResize),
        cmocka_unit_test(safuTestSafuVecInsertFrontWithoutResize),
        cmocka_unit_test(safuTestSafuVecInsertErrInvalidIndex),
        cmocka_unit_test(safuTestSafuVecInsertExterrRemalloc),
        cmocka_unit_test(safuTestSafuVecInsertErrNullVector),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

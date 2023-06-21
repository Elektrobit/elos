// SPDX-License-Identifier: MIT
#include "safuVecRemove_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(safuTestSafuVecRemoveErrIndexOutOfBounds),
        cmocka_unit_test(safuTestSafuVecRemoveErrVecInvalid),
        cmocka_unit_test(safuTestSafuVecRemoveLastElement),
        cmocka_unit_test(safuTestSafuVecRemoveFromMiddle),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

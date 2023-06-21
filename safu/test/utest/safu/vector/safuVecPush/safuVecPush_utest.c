// SPDX-License-Identifier: MIT
#include "safuVecPush_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(safuTestSafuVecPushWithResize),
        cmocka_unit_test(safuTestSafuVecPushWithoutResize),
        cmocka_unit_test(safuTestSafuVecPushExterrRealloc),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

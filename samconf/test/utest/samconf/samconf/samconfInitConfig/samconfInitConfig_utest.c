// SPDX-License-Identifier: MIT
#include "samconfInitConfig_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(samconfTestSamconfInitConfigSuccess),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

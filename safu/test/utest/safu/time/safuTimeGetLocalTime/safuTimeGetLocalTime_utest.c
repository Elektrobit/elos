// SPDX-License-Identifier: MIT
#include "safuTimeGetLocalTime_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(safuTestSafuTimeGetLocalTimeTimeReturnValidValue),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

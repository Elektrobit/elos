// SPDX-License-Identifier: MIT
#include "safuRecvExactly_utest.h"

int main() {
    const struct CMUnitTest tests[] = {cmocka_unit_test(safuTestSafuRecvExactly)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}

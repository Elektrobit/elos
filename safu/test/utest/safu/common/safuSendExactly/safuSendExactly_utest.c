// SPDX-License-Identifier: MIT
#include "safuSendExactly_utest.h"

int main() {
    const struct CMUnitTest tests[] = {cmocka_unit_test(safuTestSafuSendExactly)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}

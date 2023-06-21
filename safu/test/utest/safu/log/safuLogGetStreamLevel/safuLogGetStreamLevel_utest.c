// SPDX-License-Identifier: MIT
#include "safuLogGetStreamLevel_utest.h"

int main() {
    const struct CMUnitTest tests[] = {cmocka_unit_test(safuTestSafuLogGetStreamLevelSuccess)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}

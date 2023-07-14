// SPDX-License-Identifier: MIT
#include "elosEventInitialize_utest.h"

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(elosTestElosEventInitializeErrEvent),
                                       TEST_CASE(elosTestElosEventInitializeSuccess)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}

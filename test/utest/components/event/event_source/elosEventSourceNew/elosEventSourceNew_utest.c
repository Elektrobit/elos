// SPDX-License-Identifier: MIT
#include "elosEventSourceNew_utest.h"

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(elosTestElosEventSourceNewSuccess),
                                       TEST_CASE(elosTestElosEventSourceNewExterrCalloc)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}

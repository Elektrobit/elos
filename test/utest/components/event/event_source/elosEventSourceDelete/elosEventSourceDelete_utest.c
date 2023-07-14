// SPDX-License-Identifier: MIT
#include "elosEventSourceDelete_utest.h"

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(elosTestElosEventSourceDeleteInitializedSuccess),
                                       TEST_CASE(elosTestElosEventSourceDeleteUninitializedSuccess)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}

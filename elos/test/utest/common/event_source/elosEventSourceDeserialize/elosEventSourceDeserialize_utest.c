// SPDX-License-Identifier: MIT
#include "elosEventSourceDeserialize_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosEventSourceDeserializeErrNull),
        TEST_CASE(elosTestElosEventSourceDeserializeExterrJsonTokener),
        TEST_CASE(elosTestElosEventSourceDeserializeSuccessComplete),
        TEST_CASE(elosTestElosEventSourceDeserializeSuccessEmpty),
        TEST_CASE(elosTestElosEventSourceDeserializeSuccessUninitializedAppName),
        TEST_CASE(elosTestElosEventSourceDeserializeSuccessUninitializedFileName),
        TEST_CASE(elosTestElosEventSourceDeserializeSuccessUninitializedPID),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

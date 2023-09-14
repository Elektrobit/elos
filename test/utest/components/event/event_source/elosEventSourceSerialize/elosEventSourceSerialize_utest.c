// SPDX-License-Identifier: MIT
#include "elosEventSourceSerialize_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosEventSourceSerializeErrNull),
        TEST_CASE(elosTestElosEventSourceSerializeExterrEventSourceToJsonObject),
        TEST_CASE(elosTestElosEventSourceSerializeExterrJsonObjString),
        TEST_CASE(elosTestElosEventSourceSerializeExterrStrdup),
        TEST_CASE(elosTestElosEventSourceSerializeSuccessComplete),
        TEST_CASE(elosTestElosEventSourceSerializeSuccessEmpty),
        TEST_CASE(elosTestElosEventSourceSerializeSuccessUninitializedAppName),
        TEST_CASE(elosTestElosEventSourceSerializeSuccessUninitializedFileName),
        TEST_CASE(elosTestElosEventSourceSerializeSuccessUninitializedPID),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

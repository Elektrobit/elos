// SPDX-License-Identifier: MIT
#include "elosEventFilterNodeInitialize_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestEloEventFilterNodeInitializeErrDoubleInit),
        TEST_CASE(elosTestEloEventFilterNodeInitializeErrMutex),
        TEST_CASE(elosTestEloEventFilterNodeInitializeErrParam),
        TEST_CASE(elosTestEloEventFilterNodeInitializeSuccess),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

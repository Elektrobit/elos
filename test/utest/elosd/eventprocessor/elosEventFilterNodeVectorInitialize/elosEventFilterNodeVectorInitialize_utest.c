// SPDX-License-Identifier: MIT
#include "elosEventFilterNodeVectorInitialize_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestEloEventFilterNodeVectorInitializeErrParam),
        TEST_CASE(elosTestEloEventFilterNodeVectorInitializeSuccess),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

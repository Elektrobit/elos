// SPDX-License-Identifier: MIT
#include "elosEventFilterExecuteInTimeRange_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosEventFilterExecuteInTimeRangeSuccessOutOfRange),
        TEST_CASE(elosTestElosEventFilterExecuteInTimeRangeSuccessInRange),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

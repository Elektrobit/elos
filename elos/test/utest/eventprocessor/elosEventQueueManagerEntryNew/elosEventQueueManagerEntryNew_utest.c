// SPDX-License-Identifier: MIT
#include "elosEventQueueManagerEntryNew_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestEloEventQueueManagerEntryNewErrParam),
        TEST_CASE(elosTestEloEventQueueManagerEntryNewErrPushByParam),
        TEST_CASE(elosTestEloEventQueueManagerEntryNewSuccess),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

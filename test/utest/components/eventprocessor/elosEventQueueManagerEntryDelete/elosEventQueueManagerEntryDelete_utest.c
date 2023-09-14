// SPDX-License-Identifier: MIT
#include "elosEventQueueManagerEntryDelete_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestEloEventQueueManagerEntryDeleteErrParam),
        TEST_CASE(elosTestEloEventQueueManagerEntryDeleteErrPushByParam),
        TEST_CASE(elosTestEloEventQueueManagerEntryDeleteErrWrongId),
        TEST_CASE(elosTestEloEventQueueManagerEntryDeleteSuccess),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

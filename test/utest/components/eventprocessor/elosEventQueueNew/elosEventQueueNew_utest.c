// SPDX-License-Identifier: MIT
#include "elosEventQueueNew_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(_eventQueueNewUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestEloEventQueueNewSuccess),
        TEST_CASE(elosTestEloEventQueueNewErrParam),
        TEST_CASE(elosTestEloEventQueueNewAllocationFailed),
        TEST_CASE(elosTestEloEventQueueNewInitializeFailed),
    };

    return RUN_TEST_SUITE(tests, _eventQueueNewUtest);
}

static int _eventQueueNewUtestSetup(UNUSED void **state) {
    return 0;
}

static int _eventQueueNewUtestTeardown(UNUSED void **state) {
    return 0;
}

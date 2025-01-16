// SPDX-License-Identifier: MIT
#include "elosEventQueuePush_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(_eventQueuePushUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestEloEventQueuePushErrDeepCopy), TEST_CASE(elosTestEloEventQueuePushErrMutex),
        TEST_CASE(elosTestEloEventQueuePushErrParam),    TEST_CASE(elosTestEloEventQueuePushErrVecPush),
        TEST_CASE(elosTestEloEventQueuePushSuccess),
    };

    return RUN_TEST_SUITE(tests, _eventQueuePushUtest);
}

static int _eventQueuePushUtestSetup(void **state) {
    *state = safuAllocMem(NULL, sizeof(elosTestState_t));
    return 0;
}

static int _eventQueuePushUtestTeardown(void **state) {
    free(*state);
    return 0;
}

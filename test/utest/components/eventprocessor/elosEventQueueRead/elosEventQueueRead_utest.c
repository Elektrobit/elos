// SPDX-License-Identifier: MIT
#include "elosEventQueueRead_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(_eventQueueReadUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestEloEventQueueReadErrFunc),
        TEST_CASE(elosTestEloEventQueueReadErrMutex),
        TEST_CASE(elosTestEloEventQueueReadErrParam),
        TEST_CASE(elosTestEloEventQueueReadSuccess),
    };

    return RUN_TEST_SUITE(tests, _eventQueueReadUtest);
}

static int _eventQueueReadUtestSetup(void **state) {
    *state = safuAllocMem(NULL, sizeof(elosUnitTestState_t));
    return 0;
}

static int _eventQueueReadUtestTeardown(void **state) {
    free(*state);
    return 0;
}

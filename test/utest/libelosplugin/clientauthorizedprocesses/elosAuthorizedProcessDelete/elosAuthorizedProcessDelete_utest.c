// SPDX-License-Identifier: MIT
#include "elosAuthorizedProcessDelete_utest.h"

#include <safu/common.h>

TEST_SUITE_FUNC_PROTOTYPES(elosAuthorizedProcessDeleteUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosAuthorizedProcessDeleteSuccess),
        TEST_CASE(elosTestElosAuthorizedProcessDeleteAuthorizedprocessParamNull),
    };
    return RUN_TEST_SUITE(tests, elosAuthorizedProcessDeleteUtest);
}

static int elosAuthorizedProcessDeleteUtestSetup(void **state) {
    elosTestState_t *testState = safuAllocMem(NULL, sizeof(elosTestState_t));
    memset(testState, 0, sizeof(elosTestState_t));
    assert_non_null_msg(testState, "Failed to allocate elosTestState");
    *state = testState;
    return 0;
}

static int elosAuthorizedProcessDeleteUtestTeardown(void **state) {
    elosTestState_t *testState = *state;
    free(testState);
    return 0;
}

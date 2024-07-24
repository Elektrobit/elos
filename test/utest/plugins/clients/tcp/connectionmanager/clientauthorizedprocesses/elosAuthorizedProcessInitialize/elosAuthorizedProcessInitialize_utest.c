// SPDX-License-Identifier: MIT
#include "elosAuthorizedProcessInitialize_utest.h"

#include <safu/common.h>

TEST_SUITE_FUNC_PROTOTYPES(elosAuthorizedProcessInitializeUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosAuthorizedProcessInitializeSuccess),
        TEST_CASE(elosTestElosAuthorizedProcessInitializeAuthorizedprocessParamNull),
        TEST_CASE(elosTestElosAuthorizedProcessInitializeConfigParamNull),
        TEST_CASE(elosTestElosAuthorizedProcessInitializeExtErrConfigGet),
    };
    return RUN_TEST_SUITE(tests, elosAuthorizedProcessInitializeUtest);
}

static int elosAuthorizedProcessInitializeUtestSetup(void **state) {
    elosTestState_t *testState = safuAllocMem(NULL, sizeof(elosTestState_t));
    memset(testState, 0, sizeof(elosTestState_t));
    assert_non_null_msg(testState, "Failed to allocate elosTestState");
    *state = testState;
    return 0;
}

static int elosAuthorizedProcessInitializeUtestTeardown(void **state) {
    elosTestState_t *testState = *state;
    free(testState);
    return 0;
}

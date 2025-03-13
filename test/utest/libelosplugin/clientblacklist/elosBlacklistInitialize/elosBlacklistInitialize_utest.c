// SPDX-License-Identifier: MIT
#include "elosBlacklistInitialize_utest.h"

#include <safu/common.h>

TEST_SUITE_FUNC_PROTOTYPES(elosBlacklistInitializeUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosBlacklistInitializeSuccess),
        TEST_CASE(elosTestElosBlacklistInitializeExtErrFilterString),
        TEST_CASE(elosTestElosBlacklistInitializeExtErrConfigGetString),
        TEST_CASE(elosTestElosBlacklistInitializeErrBlacklistParameterNull),
        TEST_CASE(elosTestElosBlacklistInitializeErrConfigParameterNull),
        TEST_CASE(elosTestElosBlacklistInitializeErrFilterCreate),
    };
    return RUN_TEST_SUITE(tests, elosBlacklistInitializeUtest);
}

static int elosBlacklistInitializeUtestSetup(void **state) {
    elosTestState_t *testState = safuAllocMem(NULL, sizeof(elosTestState_t));
    memset(testState, 0, sizeof(elosTestState_t));
    assert_non_null_msg(testState, "Failed to allocate elosTestState");
    *state = testState;
    return 0;
}

static int elosBlacklistInitializeUtestTeardown(void **state) {
    elosTestState_t *testState = *state;
    free(testState);
    return 0;
}

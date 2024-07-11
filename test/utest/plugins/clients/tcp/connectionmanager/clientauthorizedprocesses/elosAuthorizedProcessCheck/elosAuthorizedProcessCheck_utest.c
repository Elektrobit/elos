// SPDX-License-Identifier: MIT
#include "elosAuthorizedProcessCheck_utest.h"

#include <safu/common.h>

TEST_SUITE_FUNC_PROTOTYPES(elosAuthorizedProcessCheckUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosAuthorizedProcessCheckSuccess),
        TEST_CASE(elosTestElosAuthorizedProcessCheckAuthorizedprocessParamNull),
        TEST_CASE(elosTestElosAuthorizedProcessCheckProcessParamNull),
        TEST_CASE(elosTestElosAuthorizedProcessCheckSuccessNoMatch),
    };
    return RUN_TEST_SUITE(tests, elosAuthorizedProcessCheckUtest);
}

static int elosAuthorizedProcessCheckUtestSetup(void **state) {
    elosTestState_t *testState = safuAllocMem(NULL, sizeof(elosTestState_t));
    memset(testState, 0, sizeof(elosTestState_t));
    assert_non_null_msg(testState, "Failed to allocate elosTestState");
    *state = testState;
    return 0;
}

static int elosAuthorizedProcessCheckUtestTeardown(void **state) {
    elosTestState_t *testState = *state;
    free(testState);
    return 0;
}

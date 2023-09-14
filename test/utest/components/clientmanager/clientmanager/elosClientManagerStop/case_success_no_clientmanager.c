// SPDX-License-Identifier: MIT

#include "elosClientManagerStop_utest.h"
#include "mock_clientauthorization.h"

int elosTestElosClientManagerStopNoManagerStartedSetup(void **state) {
    elosClientManagerStopUtestInitParameters(state);
    return 0;
}

int elosTestElosClientManagerStopNoManagerStartedTeardown(void **state) {
    elosClientManagerStopUtestDeleteParameters(state);
    return 0;
}

void elosTestElosClientManagerStopNoManagerStarted(void **state) {
    elosUteststateT_t *testState = *state;
    int retval = 0;

    TEST("elosClientManagerStop");
    SHOULD("%s", "work properly even if no client manager was started");

    MOCK_FUNC_AFTER_CALL(elosClientAuthorizationDelete, 0);
    expect_value(elosClientAuthorizationDelete, clientAuth, &(testState->context->clientAuth));
    will_return(elosClientAuthorizationDelete, SAFU_RESULT_OK);

    retval = elosClientManagerStop(testState->context);
    assert_int_equal(retval, 0);
}

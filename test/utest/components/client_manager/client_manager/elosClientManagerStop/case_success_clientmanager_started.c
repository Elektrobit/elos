// SPDX-License-Identifier: MIT

#include "elosClientManagerStop_utest.h"
#include "mock_client_authorization.h"

int elosTestElosClientManagerStopClientManagerActiveSetup(void **state) {
    elosUteststateT_t *testState = *state;

    MOCK_FUNC_AFTER_CALL(elosClientAuthorizationInitialize, 0);
    expect_value(elosClientAuthorizationInitialize, clientAuth, &(testState->context->clientAuth));
    will_return(elosClientAuthorizationInitialize, SAFU_RESULT_OK);

    elosClientManagerStopUtestInitParameters(state);
    elosClientManagerStart(testState->context, testState->parameters);
    return 0;
}

int elosTestElosClientManagerStopClientManagerActiveTeardown(void **state) {
    elosClientManagerStopUtestDeleteParameters(state);
    return 0;
}

void elosTestElosClientManagerStopClientManagerActive(void **state) {
    elosUteststateT_t *testState = *state;
    int retval = 0;

    TEST("elosClientManagerStop");
    SHOULD("%s", "stop a client manager");

    MOCK_FUNC_AFTER_CALL(elosClientAuthorizationDelete, 0);
    expect_value(elosClientAuthorizationDelete, clientAuth, &(testState->context->clientAuth));
    will_return(elosClientAuthorizationDelete, SAFU_RESULT_OK);

    retval = elosClientManagerStop(testState->context);
    assert_int_equal(retval, 0);
}

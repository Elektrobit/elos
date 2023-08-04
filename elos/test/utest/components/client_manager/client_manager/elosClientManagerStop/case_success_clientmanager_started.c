// SPDX-License-Identifier: MIT

#include "elosClientManagerStop_utest.h"
#include "mock_client_authorization.h"
#include "mock_client_authorized_processes.h"
#include "mock_client_manager.h"
#include "mock_eventdispatcher.h"

int elosTestElosClientManagerStopClientManagerActiveSetup(void **state) {
    elosUteststateT_t *testState = *state;

    MOCK_FUNC_AFTER_CALL(elosClientAuthorizationInitialize, 0);
    expect_value(elosClientAuthorizationInitialize, clientAuth, &(testState->context->clientAuth));
    will_return(elosClientAuthorizationInitialize, SAFU_RESULT_OK);

    MOCK_FUNC_ALWAYS(elosAuthorizedProcessInitialize);
    expect_any_always(elosAuthorizedProcessInitialize, authorizedprocesses);
    expect_any_always(elosAuthorizedProcessInitialize, config);
    will_return_always(elosAuthorizedProcessInitialize, SAFU_RESULT_OK);

    MOCK_FUNC_ALWAYS(elosBlacklistInitialize);
    expect_any_always(elosBlacklistInitialize, blacklist);
    expect_any_always(elosBlacklistInitialize, config);
    will_return_always(elosBlacklistInitialize, SAFU_RESULT_OK);

    MOCK_FUNC_ALWAYS(elosEventDispatcherBufferAdd);
    expect_any_always(elosEventDispatcherBufferAdd, eventDispatcher);
    expect_any_always(elosEventDispatcherBufferAdd, eventBuffer);
    will_return_always(elosEventDispatcherBufferAdd, SAFU_RESULT_OK);

    elosClientManagerStopUtestInitParameters(state);
    elosClientManagerStart(testState->context, testState->parameters);
    return 0;
}

int elosTestElosClientManagerStopClientManagerActiveTeardown(void **state) {
    elosClientManagerStopUtestDeleteParameters(state);
    MOCK_FUNC_NEVER(elosBlacklistInitialize);
    MOCK_FUNC_NEVER(elosAuthorizedProcessInitialize);
    MOCK_FUNC_NEVER(elosEventDispatcherBufferAdd);
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

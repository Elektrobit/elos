// SPDX-License-Identifier: MIT

#include <cmocka_mocks/mock_libc.h>

#include "elosClientManagerStart_utest.h"
#include "mock_client_authorization.h"

int elosTestElosClientManagerStartExtErrListenSetup(void **state) {
    elosClientManagerStartUtestInitParameters(state);
    return 0;
}

int elosTestElosClientManagerStartExtErrListenTeardown(void **state) {
    elosUteststateT_t *testState = *state;

    MOCK_FUNC_AFTER_CALL(elosClientAuthorizationDelete, 0);
    expect_value(elosClientAuthorizationDelete, clientAuth, &(testState->context->clientAuth));
    will_return(elosClientAuthorizationDelete, SAFU_RESULT_OK);

    elosClientManagerStop(testState->context);
    elosClientManagerStartUtestDeleteParameters(state);
    return 0;
}

void elosTestElosClientManagerStartExtErrListen(void **state) {
    elosUteststateT_t *testState = *state;
    int retval = 0;

    TEST("elosClientManagerStart");
    SHOULD("%s", "return -1 when listen fails");
    SKIP("%s", "broken, needs to be fixed by #16382");

    MOCK_FUNC_AFTER_CALL(elosClientAuthorizationInitialize, 0);
    expect_value(elosClientAuthorizationInitialize, clientAuth, &(testState->context->clientAuth));
    will_return(elosClientAuthorizationInitialize, SAFU_RESULT_OK);

    MOCK_FUNC_AFTER_CALL(listen, 0);
    expect_any(__wrap_listen, fd);
    expect_value(__wrap_listen, n, CLIENT_MANAGER_LISTEN_QUEUE_LENGTH);
    will_return(__wrap_listen, -1);

    retval = elosClientManagerStart(testState->context, testState->parameters);

    assert_int_equal(retval, -1);
}

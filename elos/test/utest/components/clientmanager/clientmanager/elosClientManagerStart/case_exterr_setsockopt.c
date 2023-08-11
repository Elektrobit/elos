// SPDX-License-Identifier: MIT

#include <cmocka_mocks/mock_libc.h>

#include "elosClientManagerStart_utest.h"
#include "mock_clientauthorization.h"

#define MOCK_FD 42

int elosTestElosClientManagerStartExtErrSetsockoptSetup(void **state) {
    elosClientManagerStartUtestInitParameters(state);
    return 0;
}

int elosTestElosClientManagerStartExtErrSetsockoptTeardown(void **state) {
    elosUteststateT_t *testState = *state;

    MOCK_FUNC_AFTER_CALL(elosClientAuthorizationDelete, 0);
    expect_value(elosClientAuthorizationDelete, clientAuth, &(testState->context->clientAuth));
    will_return(elosClientAuthorizationDelete, SAFU_RESULT_OK);

    elosClientManagerStop(testState->context);
    elosClientManagerStartUtestDeleteParameters(state);
    return 0;
}

void elosTestElosClientManagerStartExtErrSetsockopt(void **state) {
    elosUteststateT_t *testState = *state;
    int retval = 0;

    TEST("elosClientManagerStart");
    SHOULD("%s", "return -1 when setsockopt fails");

    MOCK_FUNC_AFTER_CALL(elosClientAuthorizationInitialize, 0);
    expect_value(elosClientAuthorizationInitialize, clientAuth, &(testState->context->clientAuth));
    will_return(elosClientAuthorizationInitialize, SAFU_RESULT_OK);

    MOCK_FUNC_AFTER_CALL(socket, 0);
    expect_value(__wrap_socket, domain, AF_INET);
    expect_value(__wrap_socket, type, SOCK_STREAM);
    expect_value(__wrap_socket, protocol, 0);
    will_return(__wrap_socket, MOCK_FD);

    MOCK_FUNC_AFTER_CALL(setsockopt, 0);
    expect_value(__wrap_setsockopt, fd, MOCK_FD);
    expect_value(__wrap_setsockopt, level, SOL_SOCKET);
    expect_value(__wrap_setsockopt, optname, SO_REUSEADDR);
    expect_any(__wrap_setsockopt, optval);
    expect_value(__wrap_setsockopt, optlen, sizeof(int));
    will_return(__wrap_setsockopt, -1);

    retval = elosClientManagerStart(testState->context, testState->parameters);

    assert_int_equal(retval, -1);
}

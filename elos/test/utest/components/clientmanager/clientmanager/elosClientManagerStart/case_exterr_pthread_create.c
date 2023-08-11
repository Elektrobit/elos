// SPDX-License-Identifier: MIT

#include <cmocka_mocks/mock_libc.h>

#include "elosClientManagerStart_utest.h"
#include "mock_clientauthorization.h"

#define MOCK_FD 42

int elosTestElosClientManagerStartExtErrPthreadCreateSetup(void **state) {
    elosClientManagerStartUtestInitParameters(state);
    return 0;
}

int elosTestElosClientManagerStartExtErrPthreadCreateTeardown(void **state) {
    elosUteststateT_t *testState = *state;

    MOCK_FUNC_AFTER_CALL(pthread_join, 0);
    expect_any(__wrap_pthread_join, __th);
    expect_any(__wrap_pthread_join, __thread_return);
    will_return(__wrap_pthread_join, 0);

    MOCK_FUNC_AFTER_CALL(elosClientAuthorizationDelete, 0);
    expect_value(elosClientAuthorizationDelete, clientAuth, &(testState->context->clientAuth));
    will_return(elosClientAuthorizationDelete, SAFU_RESULT_OK);

    elosClientManagerStop(testState->context);
    elosClientManagerStartUtestDeleteParameters(state);
    return 0;
}

void elosTestElosClientManagerStartExtErrPthreadCreate(void **state) {
    elosUteststateT_t *testState = *state;
    int retval = 0;

    TEST("elosClientManagerStart");
    SHOULD("%s", "return -1 when pthread_create fails");

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
    will_return(__wrap_setsockopt, 0);

    MOCK_FUNC_AFTER_CALL(bind, 0);
    expect_value(__wrap_bind, fd, MOCK_FD);
    expect_value(__wrap_bind, addr, &(testState->context->addr));
    expect_value(__wrap_bind, len, sizeof(testState->context->addr));
    will_return(__wrap_bind, 0);

    MOCK_FUNC_AFTER_CALL(listen, 0);
    expect_value(__wrap_listen, fd, MOCK_FD);
    expect_value(__wrap_listen, n, CLIENT_MANAGER_LISTEN_QUEUE_LENGTH);
    will_return(__wrap_listen, 0);

    MOCK_FUNC_AFTER_CALL(pthread_create, 0);
    expect_value(__wrap_pthread_create, __newthread, &(testState->context->listenThread));
    expect_value(__wrap_pthread_create, __attr, 0);
    expect_any(__wrap_pthread_create, __start_routine);
    expect_value(__wrap_pthread_create, __arg, testState->context);
    will_return(__wrap_pthread_create, -1);

    retval = elosClientManagerStart(testState->context, testState->parameters);

    assert_int_equal(retval, -1);
}

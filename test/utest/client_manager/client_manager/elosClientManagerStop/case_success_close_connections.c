// SPDX-License-Identifier: MIT

#include <cmocka_extensions/mock_extensions.h>
#include <cmocka_mocks/mock_libc.h>

#include "elosClientManagerStop_utest.h"
#include "mock_client_authorization.h"

#define MOCK_FD                42
#define LISTENING_THREAD_COUNT 1

int elosTestElosClientManagerStopCloseConnectionsSetup(void **state) {
    elosUteststateT_t *testState = *state;
    elosClientManagerStopUtestInitParameters(state);

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
    will_return(__wrap_pthread_create, 0);

    elosClientManagerStart(testState->context, testState->parameters);
    return 0;
}

int elosTestElosClientManagerStopCloseConnectionsTeardown(void **state) {
    MOCK_FUNC_NEVER(pthread_join);
    elosClientManagerStopUtestDeleteParameters(state);
    return 0;
}

void elosTestElosClientManagerStopCloseConnections(void **state) {
    elosUteststateT_t *testState = *state;
    int retval = 0;

    TEST("elosClientManagerStop");
    SHOULD("%s", "close opened connections");
    SKIP("%s", "broken, needs to be fixed by #16382");

    // mock active connections
    for (int i = 0; i < CLIENT_MANAGER_MAX_CONNECTIONS; i += 1) {
        testState->context->connection[i].status = 1;
    }

    MOCK_FUNC_ALWAYS(pthread_join);
    expect_any_count(__wrap_pthread_join, __th, CLIENT_MANAGER_MAX_CONNECTIONS + LISTENING_THREAD_COUNT);
    expect_any_count(__wrap_pthread_join, __thread_return, CLIENT_MANAGER_MAX_CONNECTIONS + LISTENING_THREAD_COUNT);
    will_return_count(__wrap_pthread_join, 0, CLIENT_MANAGER_MAX_CONNECTIONS + LISTENING_THREAD_COUNT);

    MOCK_FUNC_AFTER_CALL(elosClientAuthorizationDelete, 0);
    expect_value(elosClientAuthorizationDelete, clientAuth, &(testState->context->clientAuth));
    will_return(elosClientAuthorizationDelete, SAFU_RESULT_OK);

    retval = elosClientManagerStop(testState->context);
    assert_int_equal(retval, 0);
}

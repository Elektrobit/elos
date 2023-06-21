// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <cmocka_mocks/mock_libc.h>
#include <samconf/mock_samconf.h>
#include <samconf/samconf.h>

#include "elos/eventfilter/eventfilter.h"
#include "elosClientManagerStart_utest.h"
#include "mock_client_authorization.h"
#include "mock_client_authorized_processes.h"
#include "mock_client_manager.h"
#include "mock_eventfilter.h"
#include "safu/common.h"
#include "safu/log.h"
#include "safu/mock_log.h"
#include "safu/mock_safu.h"

#define MOCK_FD 42

int elosTestElosClientManagerStartErrBlacklistInitializeSetup(void **state) {
    elosClientManagerStartUtestInitParameters(state);
    return 0;
}

int elosTestElosClientManagerStartErrBlacklistInitializeTeardown(void **state) {
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

void elosTestElosClientManagerStartErrBlacklistInitialize(void **state) {
    elosUteststateT_t *testState = *state;
    int retval = 0;

    TEST("elosClientManagerStart");
    SHOULD("%s", "start the client manager");

    MOCK_FUNC_AFTER_CALL(elosAuthorizedProcessInitialize, 0);
    expect_any(elosAuthorizedProcessInitialize, authorizedprocesses);
    expect_any(elosAuthorizedProcessInitialize, config);
    will_set_parameter(elosAuthorizedProcessInitialize, authorizedprocesses,
                       &testState->context->clientAuth.authorizedProcessFilters);
    will_return(elosAuthorizedProcessInitialize, SAFU_RESULT_OK);

    MOCK_FUNC_AFTER_CALL(safuLogFunc, 0);
    expect_value(__wrap_safuLogFunc, level, SAFU_LOG_LEVEL_DEBUG);
    expect_any(__wrap_safuLogFunc, file);
    expect_any(__wrap_safuLogFunc, func);
    expect_any(__wrap_safuLogFunc, line);
    expect_string(__wrap_safuLogFunc, message, "authorized process filter initialization succeeded");
    will_return(__wrap_safuLogFunc, SAFU_LOG_STATUS_SUCCESS);

    MOCK_FUNC_ALWAYS(elosBlacklistInitialize);

    for (int i = 0; i < CLIENT_MANAGER_MAX_CONNECTIONS; i++) {
        expect_any(elosBlacklistInitialize, blacklist);
        expect_any(elosBlacklistInitialize, config);
        will_return(elosBlacklistInitialize, SAFU_RESULT_FAILED);
    }

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

    retval = elosClientManagerStart(testState->context, testState->parameters);
    assert_int_equal(retval, 0);

    MOCK_FUNC_NEVER(elosBlacklistInitialize);
}

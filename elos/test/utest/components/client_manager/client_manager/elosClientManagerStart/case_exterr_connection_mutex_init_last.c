// SPDX-License-Identifier: MIT

#include <cmocka_mocks/mock_libc.h>

#include "elosClientManagerStart_utest.h"
#include "mock_client_authorization.h"

#define TEST_NUM (CLIENT_MANAGER_MAX_CONNECTIONS - 1)

int elosTestElosClientManagerStartExtErrConnectionMutexInitLastSetup(void **state) {
    elosClientManagerStartUtestInitParameters(state);
    return 0;
}

int elosTestElosClientManagerStartExtErrConnectionMutexInitLastTeardown(void **state) {
    elosUteststateT_t *testState = *state;

    MOCK_FUNC_AFTER_CALL(elosClientAuthorizationDelete, 0);
    expect_value(elosClientAuthorizationDelete, clientAuth, &(testState->context->clientAuth));
    will_return(elosClientAuthorizationDelete, SAFU_RESULT_OK);

    elosClientManagerStop(testState->context);
    elosClientManagerStartUtestDeleteParameters(state);
    return 0;
}

void elosTestElosClientManagerStartExtErrConnectionMutexInitLast(void **state) {
    elosUteststateT_t *testState = *state;
    int retval = 0;

    TEST("elosClientManagerStart");
    SHOULD("%s", "return -1 when pthread_mutex_init for the last connection locks fails");

    MOCK_FUNC_AFTER_CALL(pthread_mutex_init, TEST_NUM + 1);
    expect_value(__wrap_pthread_mutex_init, __mutex, &(testState->context->connection[TEST_NUM].lock));
    expect_value(__wrap_pthread_mutex_init, __mutexattr, NULL);
    will_return(__wrap_pthread_mutex_init, -1);

    retval = elosClientManagerStart(testState->context, testState->parameters);

    assert_int_equal(retval, -1);
}

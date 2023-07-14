// SPDX-License-Identifier: MIT

#include <cmocka_mocks/mock_libc.h>

#include "elosClientManagerStart_utest.h"
#include "mock_client_authorization.h"

int elosTestElosClientManagerStartExtErrConnectionMutexInitFirstSetup(void **state) {
    elosClientManagerStartUtestInitParameters(state);
    return 0;
}

int elosTestElosClientManagerStartExtErrConnectionMutexInitFirstTeardown(void **state) {
    elosUteststateT_t *testState = *state;

    MOCK_FUNC_AFTER_CALL(elosClientAuthorizationDelete, 0);
    expect_value(elosClientAuthorizationDelete, clientAuth, &(testState->context->clientAuth));
    will_return(elosClientAuthorizationDelete, SAFU_RESULT_OK);

    elosClientManagerStop(testState->context);
    elosClientManagerStartUtestDeleteParameters(state);
    return 0;
}

void elosTestElosClientManagerStartExtErrConnectionMutexInitFirst(void **state) {
    elosUteststateT_t *testState = *state;
    int retval = 0;

    TEST("elosClientManagerStart");
    SHOULD("%s", "return -1 when pthread_mutex_init for the first connection lock fails");

    MOCK_FUNC_AFTER_CALL(pthread_mutex_init, 1);
    expect_value(__wrap_pthread_mutex_init, __mutex, &(testState->context->connection->lock));
    expect_value(__wrap_pthread_mutex_init, __mutexattr, NULL);
    will_return(__wrap_pthread_mutex_init, -1);

    retval = elosClientManagerStart(testState->context, testState->parameters);

    assert_int_equal(retval, -1);
}

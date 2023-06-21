// SPDX-License-Identifier: MIT

#include <cmocka_mocks/mock_libc.h>

#include "elosClientManagerStart_utest.h"

int elosTestElosClientManagerStartExtErrContextMutexInitSetup(void **state) {
    elosClientManagerStartUtestInitParameters(state);
    return 0;
}

int elosTestElosClientManagerStartExtErrContextMutexInitTeardown(void **state) {
    elosClientManagerStartUtestDeleteParameters(state);
    return 0;
}

void elosTestElosClientManagerStartExtErrContextMutexInit(void **state) {
    elosUteststateT_t *testState = *state;
    int retval = 0;

    TEST("elosClientManagerStart");
    SHOULD("%s", "return -1 when pthread_mutex_init for the context lock fails");

    MOCK_FUNC_AFTER_CALL(pthread_mutex_init, 0);
    expect_value(__wrap_pthread_mutex_init, __mutex, &(testState->context->lock));
    expect_value(__wrap_pthread_mutex_init, __mutexattr, NULL);
    will_return(__wrap_pthread_mutex_init, -1);

    retval = elosClientManagerStart(testState->context, testState->parameters);

    assert_int_equal(retval, -1);
}

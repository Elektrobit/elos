// SPDX-License-Identifier: MIT

#include <cmocka_mocks/mock_libc.h>

#include "elosClientManagerStart_utest.h"

int elosTestElosClientManagerStartExtErrSemInitSetup(void **state) {
    elosClientManagerStartUtestInitParameters(state);
    return 0;
}

int elosTestElosClientManagerStartExtErrSemInitTeardown(void **state) {
    elosUteststateT_t *testState = *state;
    elosClientManagerStop(testState->context);
    elosClientManagerStartUtestDeleteParameters(state);
    return 0;
}

void elosTestElosClientManagerStartExtErrSemInit(void **state) {
    elosUteststateT_t *testState = *state;
    int retval = 0;

    TEST("elosClientManagerStart");
    SHOULD("%s", "return -1 when sem_init fails");

    MOCK_FUNC_AFTER_CALL(sem_init, 0);
    expect_value(__wrap_sem_init, __sem, &(testState->context->sharedData.connectionSemaphore));
    expect_value(__wrap_sem_init, __pshared, 0);
    expect_value(__wrap_sem_init, __value, CLIENT_MANAGER_MAX_CONNECTIONS);
    will_return(__wrap_sem_init, -1);

    retval = elosClientManagerStart(testState->context, testState->parameters);

    assert_int_equal(retval, -1);
}

// SPDX-License-Identifier: MIT

#include <cmocka_mocks/mock_libc.h>

#include "elosClientManagerStop_utest.h"
#include "mock_client_authorization.h"

int elosTestElosClientManagerStopExtErrSemDestroySetup(void **state) {
    elosUteststateT_t *testState = *state;
    elosClientManagerStopUtestInitParameters(state);

    MOCK_FUNC_AFTER_CALL(elosClientAuthorizationInitialize, 0);
    expect_value(elosClientAuthorizationInitialize, clientAuth, &(testState->context->clientAuth));
    will_return(elosClientAuthorizationInitialize, SAFU_RESULT_OK);

    elosClientManagerStart(testState->context, testState->parameters);
    return 0;
}

int elosTestElosClientManagerStopExtErrSemDestroyTeardown(void **state) {
    elosClientManagerStopUtestDeleteParameters(state);
    return 0;
}

void elosTestElosClientManagerStopExtErrSemDestroy(void **state) {
    elosUteststateT_t *testState = *state;
    int retval = 0;

    TEST("elosClientManagerStop");
    SHOULD("%s", "return warning when sem_destroy fails");

    MOCK_FUNC_AFTER_CALL(sem_destroy, 0);
    expect_value(__wrap_sem_destroy, __sem, &(testState->context->sharedData.connectionSemaphore));
    will_return(__wrap_sem_destroy, -1);

    MOCK_FUNC_AFTER_CALL(elosClientAuthorizationDelete, 0);
    expect_value(elosClientAuthorizationDelete, clientAuth, &(testState->context->clientAuth));
    will_return(elosClientAuthorizationDelete, SAFU_RESULT_OK);
    retval = elosClientManagerStop(testState->context);
    assert_int_equal(retval, 0);

    __real_sem_destroy(&(testState->context->sharedData.connectionSemaphore));
}

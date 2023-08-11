// SPDX-License-Identifier: MIT

#include "elosClientManagerGetStatus_utest.h"

int elosTestElosClientManagerGetStatusExtErrMutexUnlockSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosClientManagerGetStatusExtErrMutexUnlockTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientManagerGetStatusExtErrMutexUnlock(void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosClientManagerContext_t *context = *state;
    uint32_t flags = 0;

    TEST("elosClientManagerGetStatus");
    SHOULD("%s", "return SAFU_RESULT_FAILED when first pthread_mutex_unlock fails");

    MOCK_FUNC_AFTER_CALL(pthread_mutex_lock, 0);
    expect_value(__wrap_pthread_mutex_lock, __mutex, &context->lock);
    will_return(__wrap_pthread_mutex_lock, 0);

    MOCK_FUNC_AFTER_CALL(pthread_mutex_unlock, 0);
    expect_value(__wrap_pthread_mutex_unlock, __mutex, &context->lock);
    will_return(__wrap_pthread_mutex_unlock, -1);

    MOCK_FUNC_AFTER_CALL(raise, 0);
    expect_value(__wrap_raise, __sig, SIGTERM);
    will_return(__wrap_raise, 0);

    result = elosClientManagerGetStatus(context, &flags);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}

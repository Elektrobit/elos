// SPDX-License-Identifier: MIT

#include "elosClientManagerThreadGetFreeConnectionSlot_utest.h"

int elosTestElosClientManagerThreadGetFreeConnectionSlotExtErrMutexLockSetup(void **state) {
    elosClientManagerContext_t *context = *state;
    memset(context, '\0', sizeof(elosClientManagerContext_t));
    return 0;
}

int elosTestElosClientManagerThreadGetFreeConnectionSlotExtErrMutexLockTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientManagerThreadGetFreeConnectionSlotExtErrMutexLock(void **state) {
    elosClientManagerContext_t *context = *state;
    safuResultE_t result = SAFU_RESULT_OK;
    int slot = 0;

    TEST("elosClientManagerThreadGetFreeConnectionSlot");
    SHOULD("%s", "return SAFU_RESULT_FAILED when pthread_mutex_lock fails");

    MOCK_FUNC_AFTER_CALL(sem_timedwait, 0)
    expect_any(__wrap_sem_timedwait, __sem);
    expect_any(__wrap_sem_timedwait, __abstime);
    will_return(__wrap_sem_timedwait, 0);

    MOCK_FUNC_AFTER_CALL(pthread_mutex_lock, 0);
    expect_not_value(__wrap_pthread_mutex_lock, __mutex, NULL);
    will_return(__wrap_pthread_mutex_lock, -1);

    MOCK_FUNC_AFTER_CALL(raise, 0);
    expect_value(__wrap_raise, __sig, SIGTERM);
    will_return(__wrap_raise, 0);

    result = elosClientManagerThreadGetFreeConnectionSlot(context, &slot);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}

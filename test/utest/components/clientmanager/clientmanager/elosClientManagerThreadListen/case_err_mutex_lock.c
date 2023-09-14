// SPDX-License-Identifier: MIT

#include "elosClientManagerThreadListen_utest.h"

int elosTestElosClientManagerThreadErrMutexLockSetup(void **state) {
    elosClientManagerContext_t *context = *state;
    memset(context, '\0', sizeof(elosClientManagerContext_t));
    context->status |= CLIENT_MANAGER_LISTEN_ACTIVE;
    int retval = pthread_mutex_init(&context->lock, NULL);
    assert_int_equal(retval, 0);
    return 0;
}

int elosTestElosClientManagerThreadErrMutexLockTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientManagerThreadErrMutexLock(void **state) {
    elosClientManagerContext_t *context = *state;
    elosClientManagerConnection_t *connection = &context->connection[MOCK_SLOT];

    MOCK_FUNC_ALWAYS(pthread_mutex_lock);

    TEST("elosClientManagerThreadListen");
    SHOULD("%s", "return SAFU_RESULT_FAILED when pthread_mutex_lock failed");

    MOCK_FUNC_AFTER_CALL(elosClientManagerGetStatus, 0);
    expect_value(elosClientManagerGetStatus, context, context);
    expect_not_value(elosClientManagerGetStatus, status, NULL);
    will_set_parameter(elosClientManagerGetStatus, status, context->status);
    will_return(elosClientManagerGetStatus, SAFU_RESULT_OK);

    MOCK_FUNC_AFTER_CALL(elosClientManagerThreadGetFreeConnectionSlot, 0);
    expect_value(elosClientManagerThreadGetFreeConnectionSlot, ctx, context);
    expect_not_value(elosClientManagerThreadGetFreeConnectionSlot, slot, NULL);
    will_set_parameter(elosClientManagerThreadGetFreeConnectionSlot, slot, MOCK_SLOT);
    will_return(elosClientManagerThreadGetFreeConnectionSlot, SAFU_RESULT_OK);

    expect_value(__wrap_pthread_mutex_lock, __mutex, &connection->lock);
    will_return(__wrap_pthread_mutex_lock, -1);

    MOCK_FUNC_AFTER_CALL(raise, 0);
    expect_value(__wrap_raise, __sig, SIGTERM);
    will_return(__wrap_raise, 0);

    expect_value(__wrap_pthread_mutex_lock, __mutex, &context->lock);
    will_return(__wrap_pthread_mutex_lock, -1);

    MOCK_FUNC_AFTER_CALL(pthread_mutex_unlock, 0);
    expect_value(__wrap_pthread_mutex_unlock, __mutex, &context->lock);
    will_return(__wrap_pthread_mutex_unlock, -1);

    elosClientManagerThreadListen(context);

    MOCK_FUNC_NEVER(pthread_mutex_lock);
}

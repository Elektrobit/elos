// SPDX-License-Identifier: MIT

#include "elosClientManagerThreadListen_utest.h"

int elosTestElosClientManagerThreadErrWaitForConnectionSetup(void **state) {
    elosClientManagerContext_t *context = *state;
    memset(context, '\0', sizeof(elosClientManagerContext_t));
    context->status |= CLIENT_MANAGER_LISTEN_ACTIVE;
    int retval = pthread_mutex_init(&context->lock, NULL);
    assert_int_equal(retval, 0);
    return 0;
}

int elosTestElosClientManagerThreadErrWaitForConnectionTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientManagerThreadErrWaitForConnection(void **state) {
    elosClientManagerContext_t *context = *state;
    elosClientManagerConnection_t *connection = &context->connection[MOCK_SLOT];

    MOCK_FUNC_ALWAYS(elosClientManagerGetStatus);
    MOCK_FUNC_ALWAYS(pthread_mutex_lock);
    MOCK_FUNC_ALWAYS(pthread_mutex_unlock);

    TEST("elosClientManagerThreadListen");
    SHOULD("%s", "start again when elosClientManagerThreadWaitForIncomingConnection failed");

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
    will_return(__wrap_pthread_mutex_lock, 0);

    MOCK_FUNC_AFTER_CALL(elosClientManagerThreadWaitForIncomingConnection, 0);
    expect_value(elosClientManagerThreadWaitForIncomingConnection, ctx, context);
    expect_value(elosClientManagerThreadWaitForIncomingConnection, slot, MOCK_SLOT);
    will_return(elosClientManagerThreadWaitForIncomingConnection, SAFU_RESULT_FAILED);

    expect_value(__wrap_pthread_mutex_unlock, __mutex, &connection->lock);
    will_return(__wrap_pthread_mutex_unlock, 0);

    MOCK_FUNC_AFTER_CALL(sem_post, 0);
    expect_value(__wrap_sem_post, __sem, &context->sharedData.connectionSemaphore);
    will_return(__wrap_sem_post, 0);

    // break up to leave loop
    expect_value(elosClientManagerGetStatus, context, context);
    expect_not_value(elosClientManagerGetStatus, status, NULL);
    will_set_parameter(elosClientManagerGetStatus, status, context->status);
    will_return(elosClientManagerGetStatus, SAFU_RESULT_FAILED);

    expect_value(__wrap_pthread_mutex_lock, __mutex, &context->lock);
    will_return(__wrap_pthread_mutex_lock, 0);

    expect_value(__wrap_pthread_mutex_unlock, __mutex, &context->lock);
    will_return(__wrap_pthread_mutex_unlock, 0);

    elosClientManagerThreadListen(context);

    MOCK_FUNC_NEVER(elosClientManagerGetStatus);
    MOCK_FUNC_NEVER(pthread_mutex_lock);
    MOCK_FUNC_NEVER(pthread_mutex_unlock);
}

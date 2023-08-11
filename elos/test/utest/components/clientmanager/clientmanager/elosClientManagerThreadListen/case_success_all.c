// SPDX-License-Identifier: MIT

#include "elosClientManagerThreadListen_utest.h"

int elosTestElosClientManagerThreadSuccessAllSetup(void **state) {
    elosClientManagerContext_t *context = *state;
    memset(context, '\0', sizeof(elosClientManagerContext_t));
    context->status |= CLIENT_MANAGER_LISTEN_ACTIVE;
    int retval = pthread_mutex_init(&context->lock, NULL);
    assert_int_equal(retval, 0);
    return 0;
}

int elosTestElosClientManagerThreadSuccessAllTeardown(UNUSED void **state) {
    return 0;
}

static void _testCreateConnection(elosClientManagerContext_t *context, int idx) {
    elosClientManagerConnection_t *connection = &context->connection[idx];

    expect_value(elosClientManagerGetStatus, context, context);
    expect_not_value(elosClientManagerGetStatus, status, NULL);
    will_set_parameter(elosClientManagerGetStatus, status, context->status);
    will_return(elosClientManagerGetStatus, SAFU_RESULT_OK);

    expect_value(elosClientManagerThreadGetFreeConnectionSlot, ctx, context);
    expect_not_value(elosClientManagerThreadGetFreeConnectionSlot, slot, NULL);
    will_set_parameter(elosClientManagerThreadGetFreeConnectionSlot, slot, idx);
    will_return(elosClientManagerThreadGetFreeConnectionSlot, SAFU_RESULT_OK);

    expect_value(__wrap_pthread_mutex_lock, __mutex, &connection->lock);
    will_return(__wrap_pthread_mutex_lock, 0);

    expect_value(elosClientManagerThreadWaitForIncomingConnection, ctx, context);
    expect_value(elosClientManagerThreadWaitForIncomingConnection, slot, idx);
    will_return(elosClientManagerThreadWaitForIncomingConnection, SAFU_RESULT_OK);

    expect_value(__wrap_pthread_create, __newthread, &connection->thread);
    expect_value(__wrap_pthread_create, __attr, 0);
    expect_any(__wrap_pthread_create, __start_routine);
    expect_value(__wrap_pthread_create, __arg, (void *)connection);
    will_return(__wrap_pthread_create, 0);

    expect_value(__wrap_pthread_mutex_unlock, __mutex, &connection->lock);
    will_return(__wrap_pthread_mutex_unlock, 0);
}

void elosTestElosClientManagerThreadSuccessAll(void **state) {
    elosClientManagerContext_t *context = *state;

    MOCK_FUNC_ALWAYS(elosClientManagerGetStatus);
    MOCK_FUNC_ALWAYS(elosClientManagerThreadGetFreeConnectionSlot);
    MOCK_FUNC_ALWAYS(elosClientManagerThreadWaitForIncomingConnection);
    MOCK_FUNC_ALWAYS(pthread_create);
    MOCK_FUNC_ALWAYS(pthread_mutex_lock);
    MOCK_FUNC_ALWAYS(pthread_mutex_unlock);

    TEST("elosClientManagerThreadListen");
    SHOULD("%s", "successfully create max amount of connections");

    for (int i = 0; i < CLIENT_MANAGER_MAX_CONNECTIONS; i++) {
        _testCreateConnection(context, i);
    }

    // connections created, break up to leave loop

    expect_value(elosClientManagerGetStatus, context, context);
    expect_not_value(elosClientManagerGetStatus, status, NULL);
    will_set_parameter(elosClientManagerGetStatus, status, 0);
    will_return(elosClientManagerGetStatus, SAFU_RESULT_OK);

    expect_value(__wrap_pthread_mutex_lock, __mutex, &context->lock);
    will_return(__wrap_pthread_mutex_lock, 0);

    expect_value(__wrap_pthread_mutex_unlock, __mutex, &context->lock);
    will_return(__wrap_pthread_mutex_unlock, 0);

    elosClientManagerThreadListen(context);

    MOCK_FUNC_NEVER(elosClientManagerGetStatus);
    MOCK_FUNC_NEVER(elosClientManagerThreadGetFreeConnectionSlot);
    MOCK_FUNC_NEVER(elosClientManagerThreadWaitForIncomingConnection);
    MOCK_FUNC_NEVER(pthread_create);
    MOCK_FUNC_NEVER(pthread_mutex_lock);
    MOCK_FUNC_NEVER(pthread_mutex_unlock);
}

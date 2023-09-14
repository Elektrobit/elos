// SPDX-License-Identifier: MIT

#include "elosClientManagerThreadListen_utest.h"

int elosTestElosClientManagerThreadSlotLtZeroSetup(void **state) {
    elosClientManagerContext_t *context = *state;
    memset(context, '\0', sizeof(elosClientManagerContext_t));
    context->status |= CLIENT_MANAGER_LISTEN_ACTIVE;
    int retval = pthread_mutex_init(&context->lock, NULL);
    assert_int_equal(retval, 0);
    return 0;
}

int elosTestElosClientManagerThreadSlotLtZeroTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientManagerThreadSlotLtZero(void **state) {
    elosClientManagerContext_t *context = *state;

    MOCK_FUNC_ALWAYS(elosClientManagerGetStatus);

    TEST("elosClientManagerThreadListen");
    SHOULD("%s", "start again when the received slot is negative");

    expect_value(elosClientManagerGetStatus, context, context);
    expect_not_value(elosClientManagerGetStatus, status, NULL);
    will_set_parameter(elosClientManagerGetStatus, status, context->status);
    will_return(elosClientManagerGetStatus, SAFU_RESULT_OK);

    MOCK_FUNC_AFTER_CALL(elosClientManagerThreadGetFreeConnectionSlot, 0);
    expect_value(elosClientManagerThreadGetFreeConnectionSlot, ctx, context);
    expect_not_value(elosClientManagerThreadGetFreeConnectionSlot, slot, NULL);
    will_set_parameter(elosClientManagerThreadGetFreeConnectionSlot, slot, -1);
    will_return(elosClientManagerThreadGetFreeConnectionSlot, SAFU_RESULT_OK);

    // break up to leave loop

    expect_value(elosClientManagerGetStatus, context, context);
    expect_not_value(elosClientManagerGetStatus, status, NULL);
    will_set_parameter(elosClientManagerGetStatus, status, context->status);
    will_return(elosClientManagerGetStatus, SAFU_RESULT_FAILED);

    MOCK_FUNC_AFTER_CALL(pthread_mutex_lock, 0);
    expect_value(__wrap_pthread_mutex_lock, __mutex, &context->lock);
    will_return(__wrap_pthread_mutex_lock, 0);

    MOCK_FUNC_AFTER_CALL(pthread_mutex_unlock, 0);
    expect_value(__wrap_pthread_mutex_unlock, __mutex, &context->lock);
    will_return(__wrap_pthread_mutex_unlock, 0);

    elosClientManagerThreadListen(context);

    MOCK_FUNC_NEVER(elosClientManagerGetStatus);
}

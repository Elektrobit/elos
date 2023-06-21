// SPDX-License-Identifier: MIT

#include "elosClientManagerThreadListen_utest.h"

int elosTestElosClientManagerThreadListenErrGetStatusSetup(void **state) {
    elosClientManagerContext_t *context = *state;
    memset(context, '\0', sizeof(elosClientManagerContext_t));
    int retval = pthread_mutex_init(&context->lock, NULL);
    assert_int_equal(retval, 0);
    return 0;
}

int elosTestElosClientManagerThreadListenErrGetStatusTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientManagerThreadListenErrGetStatus(void **state) {
    elosClientManagerContext_t *context = *state;

    TEST("elosClientManagerThreadListen");
    SHOULD("%s", "return SAFU_RESULT_FAILED when elosClientManagerGetStatus fails");

    MOCK_FUNC_AFTER_CALL(elosClientManagerGetStatus, 0);
    expect_value(elosClientManagerGetStatus, context, context);
    expect_not_value(elosClientManagerGetStatus, status, NULL);
    will_set_parameter(elosClientManagerGetStatus, status, 0);
    will_return(elosClientManagerGetStatus, SAFU_RESULT_FAILED);

    MOCK_FUNC_AFTER_CALL(pthread_mutex_lock, 0);
    expect_value(__wrap_pthread_mutex_lock, __mutex, &context->lock);
    will_return(__wrap_pthread_mutex_lock, 0);

    MOCK_FUNC_AFTER_CALL(pthread_mutex_unlock, 0);
    expect_value(__wrap_pthread_mutex_unlock, __mutex, &context->lock);
    will_return(__wrap_pthread_mutex_unlock, 0);

    elosClientManagerThreadListen(context);
}

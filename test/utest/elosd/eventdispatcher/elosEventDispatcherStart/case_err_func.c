// SPDX-License-Identifier: MIT

#include <cmocka_mocks/mock_libc.h>
#include <signal.h>

#include "elosEventDispatcherStart_utest.h"

int testElosEventDispatcherStartErrFuncSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventDispatcherParam_t const param = {.eventProcessor = &test->eventProcessor};
    safuResultE_t result;

    result = elosEventDispatcherInitialize(&test->eventDispatcher, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int testElosEventDispatcherStartErrFuncTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventDispatcherDeleteMembers(&test->eventDispatcher);

    return 0;
}

void testElosEventDispatcherStartErrFunc(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosEventDispatcherStart");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "pthread_create fails");

    MOCK_FUNC_AFTER_CALL(pthread_create, 0);
    expect_not_value(__wrap_pthread_create, __newthread, NULL);
    expect_value(__wrap_pthread_create, __attr, 0);
    expect_not_value(__wrap_pthread_create, __start_routine, NULL);
    expect_not_value(__wrap_pthread_create, __arg, NULL);
    will_return(__wrap_pthread_create, -1);

    result = elosEventDispatcherStart(&test->eventDispatcher);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "eventfd_read fails");

    // The mock of pthread_create() is needed here to prevent a "mock race" of eventfd_read() with the worker thread
    MOCK_FUNC_AFTER_CALL(pthread_create, 0);
    expect_not_value(__wrap_pthread_create, __newthread, NULL);
    expect_value(__wrap_pthread_create, __attr, 0);
    expect_not_value(__wrap_pthread_create, __start_routine, NULL);
    expect_not_value(__wrap_pthread_create, __arg, NULL);
    will_return(__wrap_pthread_create, 0);

    MOCK_FUNC_ALWAYS(pthread_setname_np);
    expect_any_always(__wrap_pthread_setname_np, thread);
    expect_any_always(__wrap_pthread_setname_np, name);
    will_return_always(__wrap_pthread_setname_np, 0);

    MOCK_FUNC_AFTER_CALL(eventfd_read, 0);
    expect_any(__wrap_eventfd_read, __fd);
    expect_not_value(__wrap_eventfd_read, __value, NULL);
    will_return(__wrap_eventfd_read, -1);

    result = elosEventDispatcherStart(&test->eventDispatcher);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    pthread_mutex_unlock(&test->eventDispatcher.lock);  // Prevent dead locks further down the road
}

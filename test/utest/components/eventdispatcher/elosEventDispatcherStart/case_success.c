// SPDX-License-Identifier: MIT

#include <cmocka_mocks/mock_libc.h>

#include "elosEventDispatcherStart_utest.h"

int testElosEventDispatcherStartSuccessSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventDispatcherParam_t const param = {.eventProcessor = &test->eventProcessor};
    safuResultE_t result;

    result = elosEventDispatcherInitialize(&test->eventDispatcher, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int testElosEventDispatcherStartSuccessTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventDispatcherDeleteMembers(&test->eventDispatcher);

    return 0;
}

void testElosEventDispatcherStartSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosEventDispatcherStart");
    SHOULD("%s", "test correct behaviour of elosEventDispatcherStart");

    MOCK_FUNC_ALWAYS(pthread_setname_np);
    expect_any_always(__wrap_pthread_setname_np, thread);
    expect_any_always(__wrap_pthread_setname_np, name);
    will_return_always(__wrap_pthread_setname_np, 0);

    result = elosEventDispatcherStart(&test->eventDispatcher);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(ELOS_EVENTDISPATCHER_FLAG_HAS_ACTIVE_BIT(&test->eventDispatcher.flags), true);
}

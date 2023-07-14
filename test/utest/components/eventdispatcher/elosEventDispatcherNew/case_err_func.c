// SPDX-License-Identifier: MIT

#include "elosEventDispatcherNew_utest.h"
#include "mock_eventdispatcher.h"

int testElosEventDispatcherNewErrFuncSetup(UNUSED void **state) {
    return 0;
}

int testElosEventDispatcherNewErrFuncTeardown(UNUSED void **state) {
    return 0;
}

void testElosEventDispatcherNewErrFunc(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;
    elosEventDispatcherParam_t const param = {
        .eventProcessor = &test->eventProcessor,
    };

    TEST("elosEventDispatcherNew");
    SHOULD("%s", "test correct behaviour of elosEventDispatcherNew");

    TEST("elosEventDispatcherDelete");
    SHOULD("%s", "test correct behaviour of elosEventDispatcherDelete during errors");

    MOCK_FUNC_AFTER_CALL(elosEventDispatcherInitialize, 0);
    expect_not_value(elosEventDispatcherInitialize, eventDispatcher, NULL);
    expect_not_value(elosEventDispatcherInitialize, param, NULL);
    will_return(elosEventDispatcherInitialize, SAFU_RESULT_FAILED);

    result = elosEventDispatcherNew(&test->eventDispatcher, &param);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}

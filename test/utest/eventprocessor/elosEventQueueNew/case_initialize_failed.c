// SPDX-License-Identifier: MIT

#include "elosEventQueueNew_utest.h"
#include "mock_eventqueue.h"
#include "safu/mock_log.h"

int elosTestEloEventQueueNewInitializeFailedSetup(UNUSED void **state) {
    elosUnitTestState_t *test = safuAllocMem(NULL, sizeof(elosUnitTestState_t));
    *state = test;
    return 0;
}

int elosTestEloEventQueueNewInitializeFailedTeardown(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    free(test);
    return 0;
}

void elosTestEloEventQueueNewInitializeFailed(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventQueueParam_t const param = _EVENTQUEUE_PARAM;
    safuResultE_t result;

    TEST("elosEventQueueNew");
    SHOULD("%s", "fail, since elosEventQueueInitialize failed");

    MOCK_FUNC_ENABLE(safuLogFunc);
    MOCK_FUNC_ENABLE(elosEventQueueInitialize);

    expect_any(elosEventQueueInitialize, eventQueue);
    expect_value(elosEventQueueInitialize, param, &param);
    will_return(elosEventQueueInitialize, SAFU_RESULT_FAILED);

    expect_value(__wrap_safuLogFunc, level, SAFU_LOG_LEVEL_ERR);
    expect_any(__wrap_safuLogFunc, file);
    expect_any(__wrap_safuLogFunc, func);
    expect_any(__wrap_safuLogFunc, line);
    expect_string(__wrap_safuLogFunc, message, "elosEventQueueInitialize failed");
    will_return(__wrap_safuLogFunc, SAFU_RESULT_OK);

    result = elosEventQueueNew(&test->eventQueue, &param);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}

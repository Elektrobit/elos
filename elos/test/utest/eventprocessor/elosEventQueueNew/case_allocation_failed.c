// SPDX-License-Identifier: MIT

#include "elosEventQueueNew_utest.h"
#include "safu/mock_log.h"
#include "safu/mock_safu.h"

int elosTestEloEventQueueNewAllocationFailedSetup(UNUSED void **state) {
    elosUnitTestState_t *test = safuAllocMem(NULL, sizeof(elosUnitTestState_t));
    *state = test;
    return 0;
}

int elosTestEloEventQueueNewAllocationFailedTeardown(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    free(test);
    return 0;
}

void elosTestEloEventQueueNewAllocationFailed(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventQueueParam_t const param = _EVENTQUEUE_PARAM;
    safuResultE_t result;

    TEST("elosEventQueueNew");
    SHOULD("%s", "fail, since safuAllocMem failed");

    MOCK_FUNC_ENABLE(safuLogFunc);
    MOCK_FUNC_ENABLE(safuAllocMem);

    expect_value(__wrap_safuAllocMem, oldptr, NULL);
    expect_value(__wrap_safuAllocMem, newlen, sizeof(elosEventQueue_t));
    will_return(__wrap_safuAllocMem, NULL);

    expect_value(__wrap_safuLogFunc, level, SAFU_LOG_LEVEL_ERR);
    expect_any(__wrap_safuLogFunc, file);
    expect_any(__wrap_safuLogFunc, func);
    expect_any(__wrap_safuLogFunc, line);
    expect_string(__wrap_safuLogFunc, message, "safuAllocMem failed");
    will_return(__wrap_safuLogFunc, SAFU_RESULT_OK);

    result = elosEventQueueNew(&test->eventQueue, &param);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}

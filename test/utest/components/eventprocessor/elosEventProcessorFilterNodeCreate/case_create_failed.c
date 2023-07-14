// SPDX-License-Identifier: MIT

#include "elosEventProcessorFilterNodeCreate_utest.h"
#include "safu/mock_log.h"

int elosTestEloEventProcessorFilterNodeCreateCreateFailedSetup(void **state) {
    elosTestState_t *test = safuAllocMem(NULL, sizeof(elosTestState_t));
    elosEventProcessorParam_t const param = {.config = &test->config};
    safuResultE_t result;

    memset(test, 0, sizeof(elosTestState_t));

    result = elosEventProcessorInitialize(&test->eventProcessor, &param);
    assert_true(result == SAFU_RESULT_OK);

    *state = test;
    return 0;
}

int elosTestEloEventProcessorFilterNodeCreateCreateFailedTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventProcessorDeleteMembers(&test->eventProcessor);
    free(test);
    return 0;
}

void elosTestEloEventProcessorFilterNodeCreateCreateFailed(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    char const *filterStrings[] = {"1 1 AND", "2 1 OR"};
    size_t const filterStringCount = ARRAY_SIZE(filterStrings);
    elosEventFilterNodeId_t eventFilterNodeId = ELOS_ID_INVALID;
    elosEventQueueId_t eventQueueId = ELOS_ID_INVALID;
    safuResultE_t result;

    TEST("elosEventProcessorFilterNodeCreate");
    SHOULD("%s", "fail since elosEventFilterManagerNodeCreate returns with error");

    MOCK_FUNC_AFTER_CALL(safuLogFunc, 1);

    expect_value(__wrap_safuLogFunc, level, SAFU_LOG_LEVEL_ERR);
    expect_any(__wrap_safuLogFunc, file);
    expect_any(__wrap_safuLogFunc, func);
    expect_any(__wrap_safuLogFunc, line);
    expect_string(__wrap_safuLogFunc, message, "elosEventFilterManagerNodeCreate failed");
    will_return(__wrap_safuLogFunc, SAFU_RESULT_OK);

    result = elosEventProcessorFilterNodeCreate(&test->eventProcessor, filterStrings, filterStringCount, eventQueueId,
                                                &eventFilterNodeId);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}

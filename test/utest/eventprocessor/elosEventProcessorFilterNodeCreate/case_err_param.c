// SPDX-License-Identifier: MIT

#include "elosEventProcessorFilterNodeCreate_utest.h"
#include "safu/mock_log.h"

int elosTestEloEventProcessorFilterNodeCreateErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestEloEventProcessorFilterNodeCreateErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestEloEventProcessorFilterNodeCreateErrParam(UNUSED void **state) {
    char const *filterStrings[] = {"1 1 AND", "2 1 OR"};
    size_t const filterStringCount = ARRAY_SIZE(filterStrings);
    elosEventFilterNodeId_t eventFilterNodeId = ELOS_ID_INVALID;
    elosEventQueueId_t eventQueueId = ELOS_ID_INVALID;
    safuResultE_t result;

    TEST("elosEventProcessorFilterNodeCreate");
    SHOULD("%s", "fail due to a null event processor pointer");

    MOCK_FUNC_ENABLE(safuLogFunc);

    expect_value(__wrap_safuLogFunc, level, SAFU_LOG_LEVEL_ERR);
    expect_any(__wrap_safuLogFunc, file);
    expect_any(__wrap_safuLogFunc, func);
    expect_any(__wrap_safuLogFunc, line);
    expect_string(__wrap_safuLogFunc, message, "Invalid parameter");
    will_return(__wrap_safuLogFunc, SAFU_RESULT_OK);

    result =
        elosEventProcessorFilterNodeCreate(NULL, filterStrings, filterStringCount, eventQueueId, &eventFilterNodeId);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}

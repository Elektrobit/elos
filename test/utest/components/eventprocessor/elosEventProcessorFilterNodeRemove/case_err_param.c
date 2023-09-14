// SPDX-License-Identifier: MIT

#include "elosEventProcessorFilterNodeRemove_utest.h"
#include "safu/mock_log.h"

int elosTestEloEventProcessorFilterNodeRemoveErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestEloEventProcessorFilterNodeRemoveErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestEloEventProcessorFilterNodeRemoveErrParam(UNUSED void **state) {
    safuResultE_t result;
    elosEventFilterNodeId_t eventFilterNodeId = ELOS_ID_INVALID;

    TEST("elosEventProcessorFilterNodeRemove");
    SHOULD("%s", "fail due to a null event processor pointer");

    MOCK_FUNC_ENABLE(safuLogFunc);

    expect_value(__wrap_safuLogFunc, level, SAFU_LOG_LEVEL_ERR);
    expect_any(__wrap_safuLogFunc, file);
    expect_any(__wrap_safuLogFunc, func);
    expect_any(__wrap_safuLogFunc, line);
    expect_string(__wrap_safuLogFunc, message, "Invalid parameter");
    will_return(__wrap_safuLogFunc, SAFU_RESULT_OK);

    result = elosEventProcessorFilterNodeRemove(NULL, eventFilterNodeId);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}

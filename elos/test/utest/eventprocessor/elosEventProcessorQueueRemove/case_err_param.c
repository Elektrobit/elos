// SPDX-License-Identifier: MIT

#include "elosEventProcessorQueueRemove_utest.h"
#include "safu/mock_log.h"

int elosTestEloEventProcessorQueueRemoveErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestEloEventProcessorQueueRemoveErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestEloEventProcessorQueueRemoveErrParam(UNUSED void **state) {
    safuResultE_t result;

    TEST("elosEventProcessorQueueRemove");
    SHOULD("%s", "fail as the given event processor pointer is NULL");

    MOCK_FUNC_ENABLE(safuLogFunc);

    expect_value(__wrap_safuLogFunc, level, SAFU_LOG_LEVEL_ERR);
    expect_any(__wrap_safuLogFunc, file);
    expect_any(__wrap_safuLogFunc, func);
    expect_any(__wrap_safuLogFunc, line);
    expect_string(__wrap_safuLogFunc, message, "Invalid parameter");
    will_return(__wrap_safuLogFunc, SAFU_RESULT_OK);

    result = elosEventProcessorQueueRemove(NULL, 0);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}

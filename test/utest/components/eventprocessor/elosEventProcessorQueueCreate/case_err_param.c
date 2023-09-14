// SPDX-License-Identifier: MIT

#include "elosEventProcessorQueueCreate_utest.h"
#include "safu/mock_log.h"

int elosTestEloEventProcessorQueueCreateErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestEloEventProcessorQueueCreateErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestEloEventProcessorQueueCreateErrParam(UNUSED void **state) {
    safuResultE_t result;

    TEST("elosEventProcessorQueueCreate");
    SHOULD("%s", "fail as the given event processor pointer is NULL");

    MOCK_FUNC_ENABLE(safuLogFunc);

    expect_value(__wrap_safuLogFunc, level, SAFU_LOG_LEVEL_ERR);
    expect_any(__wrap_safuLogFunc, file);
    expect_any(__wrap_safuLogFunc, func);
    expect_any(__wrap_safuLogFunc, line);
    expect_string(__wrap_safuLogFunc, message, "Invalid parameter");
    will_return(__wrap_safuLogFunc, SAFU_RESULT_OK);

    result = elosEventProcessorQueueCreate(NULL, 0);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}

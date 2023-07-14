// SPDX-License-Identifier: MIT

#include "elosEventProcessorPublish_utest.h"
#include "safu/mock_log.h"

int elosTestEloEventProcessorPublishErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestEloEventProcessorPublishErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestEloEventProcessorPublishErrParam(UNUSED void **state) {
    safuResultE_t result;

    TEST("elosEventProcessorPublish");
    SHOULD("%s", "fail as the given event processor pointer is NULL");

    MOCK_FUNC_ENABLE(safuLogFunc);

    expect_value(__wrap_safuLogFunc, level, SAFU_LOG_LEVEL_ERR);
    expect_any(__wrap_safuLogFunc, file);
    expect_any(__wrap_safuLogFunc, func);
    expect_any(__wrap_safuLogFunc, line);
    expect_string(__wrap_safuLogFunc, message, "Invalid parameter");
    will_return(__wrap_safuLogFunc, SAFU_RESULT_OK);

    result = elosEventProcessorPublish(NULL, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}

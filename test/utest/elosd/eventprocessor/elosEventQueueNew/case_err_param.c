// SPDX-License-Identifier: MIT

#include "elosEventQueueNew_utest.h"
#include "safu/mock_log.h"

int elosTestEloEventQueueNewErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestEloEventQueueNewErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestEloEventQueueNewErrParam(UNUSED void **state) {
    elosEventQueueParam_t const param = _EVENTQUEUE_PARAM;
    safuResultE_t result;

    TEST("elosEventQueueNew");
    SHOULD("%s", "fail, as the function is given an null pointer for the queue");

    MOCK_FUNC_ENABLE(safuLogFunc);

    expect_value(__wrap_safuLogFunc, level, SAFU_LOG_LEVEL_ERR);
    expect_any(__wrap_safuLogFunc, file);
    expect_any(__wrap_safuLogFunc, func);
    expect_any(__wrap_safuLogFunc, line);
    expect_string(__wrap_safuLogFunc, message, "Invalid parameter");
    will_return(__wrap_safuLogFunc, SAFU_RESULT_OK);

    result = elosEventQueueNew(NULL, &param);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}

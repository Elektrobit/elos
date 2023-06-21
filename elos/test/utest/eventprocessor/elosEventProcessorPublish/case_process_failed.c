// SPDX-License-Identifier: MIT

#include "elosEventProcessorPublish_utest.h"
#include "safu/mock_log.h"

int elosTestEloEventProcessorPublishProcessFailedSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventProcessorParam_t const param = {.config = &test->config};
    safuResultE_t result;

    memset(test, 0, sizeof(elosUnitTestState_t));

    result = elosEventProcessorInitialize(&test->eventProcessor, &param);
    assert_true(result == SAFU_RESULT_OK);

    return 0;
}

int elosTestEloEventProcessorPublishProcessFailedTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventProcessorDeleteMembers(&test->eventProcessor);
    return 0;
}

void elosTestEloEventProcessorPublishProcessFailed(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosEventProcessorPublish");
    SHOULD("%s", "fail as elosEventFilterManagerProcess failes");

    MOCK_FUNC_AFTER_CALL(safuLogFunc, 1);

    expect_value(__wrap_safuLogFunc, level, SAFU_LOG_LEVEL_ERR);
    expect_any(__wrap_safuLogFunc, file);
    expect_any(__wrap_safuLogFunc, func);
    expect_any(__wrap_safuLogFunc, line);
    expect_string(__wrap_safuLogFunc, message, "elosEventFilterManagerProcess failed");
    will_return(__wrap_safuLogFunc, SAFU_RESULT_OK);

    result = elosEventProcessorPublish(&test->eventProcessor, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}

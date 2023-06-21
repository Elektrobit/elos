// SPDX-License-Identifier: MIT

#include "elosEventPublish_utest.h"
#include "mock_event.h"

int elosTestElosEventPublishErrConvertSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventPublishErrConvertTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventPublishErrConvert(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosUnitTestSet_t *tSet;
    safuResultE_t result;

    TEST("elosEventPublish (event conversion error)");
    SHOULD("%s", "return SAFU_RESULT_FAILED after failing during event to json conversion");

    MOCK_FUNC_AFTER_CALL(elosEventToJsonObject, 0);
    expect_not_value(elosEventToJsonObject, to, NULL);
    expect_not_value(elosEventToJsonObject, from, NULL);
    will_return(elosEventToJsonObject, SAFU_RESULT_FAILED);

    tSet = &test->normal;
    result = elosEventPublish(&test->session, &tSet->event);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}

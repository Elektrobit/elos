// SPDX-License-Identifier: MIT

#include "elosEventQueueRead_utest.h"
#include "mock_event_vector.h"

int elosTestElosEventQueueReadErrConversionSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventQueueReadErrConversionTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventQueueReadErrConversion(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosEventQueueRead");
    SHOULD("%s", "return SAFU_RESULT_FAILED after failing event array conversion");

    PARAM("%s", "safuJsonGetArray fails");

    elosMockSendAndReceiveJsonMessage(test, test->responseStrNoArray, SAFU_RESULT_OK);
    result = elosEventQueueRead(&test->session, _MOCK_EVENTQUEUE_ID, &test->eventVector);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "elosEventVectorFromJsonArray fails");

    MOCK_FUNC_AFTER_CALL(elosEventVectorFromJsonArray, 0);
    expect_not_value(elosEventVectorFromJsonArray, jEventArray, NULL);
    expect_not_value(elosEventVectorFromJsonArray, eventVector, NULL);
    will_set_parameter(elosEventVectorFromJsonArray, jEventArray, NULL);
    will_return(elosEventVectorFromJsonArray, SAFU_RESULT_FAILED);

    elosMockSendAndReceiveJsonMessage(test, test->responseStrNormal, SAFU_RESULT_OK);
    result = elosEventQueueRead(&test->session, _MOCK_EVENTQUEUE_ID, &test->eventVector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_null(test->eventVector);
}

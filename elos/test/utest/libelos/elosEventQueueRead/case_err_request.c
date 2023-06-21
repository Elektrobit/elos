// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_jsonc.h"
#include "elosEventQueueRead_utest.h"

int elosTestElosEventQueueReadErrRequestSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventQueueReadErrRequestTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventQueueReadErrRequest(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventVector_t *eventVector = NULL;
    safuResultE_t result;

    TEST("elosEventQueueRead (elosd error)");
    SHOULD("%s", "return SAFU_RESULT_FAILED after a receiving an error message from elosd");

    PARAM("%s", "json object creation fails (json_object_new_object returns NULL)");

    MOCK_FUNC_AFTER_CALL(json_object_new_object, 0);
    will_return(__wrap_json_object_new_object, NULL);

    result = elosEventQueueRead(&test->session, _MOCK_EVENTQUEUE_ID, &eventVector);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "adding integer to json object fails (safuJsonAddNewUint64 returns NULL)");

    MOCK_FUNC_AFTER_CALL(safuJsonAddNewUint64, 0);
    expect_not_value(__wrap_safuJsonAddNewUint64, jobj, NULL);
    expect_string(__wrap_safuJsonAddNewUint64, name, "eventQueueId");
    expect_value(__wrap_safuJsonAddNewUint64, val, _MOCK_EVENTQUEUE_ID);
    will_return(__wrap_safuJsonAddNewUint64, NULL);

    result = elosEventQueueRead(&test->session, _MOCK_EVENTQUEUE_ID, &eventVector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}

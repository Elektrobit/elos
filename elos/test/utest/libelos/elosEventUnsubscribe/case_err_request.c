// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_jsonc.h"
#include "elosEventUnsubscribe_utest.h"

void elosTestElosEventUnsubscribeErrRequest(void **state) {
    elosTestState_t *test = *state;
    safuResultE_t result;

    TEST("elosEventUnsubscribe (elosd error)");
    SHOULD("%s", "return SAFU_RESULT_FAILED after a receiving an error message from elosd");

    PARAM("%s", "json object creation fails (json_object_new_object returns NULL)");

    MOCK_FUNC_AFTER_CALL(json_object_new_object, 0);
    will_return(__wrap_json_object_new_object, NULL);

    result = elosEventUnsubscribe(&test->session, _MOCK_EVENTQUEUE_ID);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "adding integer to json object fails (safuJsonAddNewUint64 returns NULL)");

    MOCK_FUNC_AFTER_CALL(safuJsonAddNewUint64, 0);
    expect_not_value(__wrap_safuJsonAddNewUint64, jobj, NULL);
    expect_string(__wrap_safuJsonAddNewUint64, name, "eventQueueId");
    expect_value(__wrap_safuJsonAddNewUint64, val, _MOCK_EVENTQUEUE_ID);
    will_return(__wrap_safuJsonAddNewUint64, NULL);

    result = elosEventUnsubscribe(&test->session, _MOCK_EVENTQUEUE_ID);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}

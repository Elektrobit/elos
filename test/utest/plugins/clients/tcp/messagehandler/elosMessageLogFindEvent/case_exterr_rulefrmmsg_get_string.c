// SPDX-License-Identifier: MIT

#include "elosMessageLogFindEvent_utest.h"
#include "safu/mock_safu.h"

extern safuResultE_t elosMessageLogFindEvent(elosClientConnection_t *conn, elosMessage_t const *const msg);

int elosTestElosMessageLogFindEventExtErrRuleFrmMsgGetStringSetup(void **state) {
    static elosUteststateT_t testState = {0};

    *state = &testState;
    elosMessageLogFindEventUtestCreateConnection(state);
    elosMessageLogFindEventUtestCreateMessage(state);

    return 0;
}

int elosTestElosMessageLogFindEventExtErrRuleFrmMsgGetStringTeardown(void **state) {
    elosMessageLogFindEventUtestFreeConnection(state);
    elosMessageLogFindEventUtestFreeMessage(state);

    return 0;
}

void elosTestElosMessageLogFindEventExtErrRuleFrmMsgGetString(void **state) {
    elosUteststateT_t *testState = *state;
    safuResultE_t result = SAFU_RESULT_OK;
    elosClientConnection_t *conn = testState->connection;
    elosMessage_t *msg = testState->msg;

    TEST("elosMessageLogFindEvent");
    SHOULD("%s", "return SAFU_RESULT_FAILED when safuJsonGetString in _getFilterRuleFromJsonMessage fails");

    json_object *mockJobj = __real_json_tokener_parse(msg->json);
    assert_non_null(mockJobj);

    MOCK_FUNC_AFTER_CALL(json_tokener_parse, 0);
    expect_string(__wrap_json_tokener_parse, str, msg->json);
    will_return(__wrap_json_tokener_parse, mockJobj);

    MOCK_FUNC_AFTER_CALL(safuJsonGetString, 0);
    expect_value(__wrap_safuJsonGetString, jobj, mockJobj);
    expect_string(__wrap_safuJsonGetString, name, "filter");
    expect_value(__wrap_safuJsonGetString, idx, 0);
    expect_any(__wrap_safuJsonGetString, val);
    will_set_parameter(__wrap_safuJsonGetString, val, NULL);
    will_return(__wrap_safuJsonGetString, 1);

    result = elosMessageLogFindEvent(conn, msg);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}

// SPDX-License-Identifier: MIT

#include "elosMessageLogFindEvent_utest.h"

extern safuResultE_t elosMessageLogFindEvent(elosClientManagerConnection_t *conn, elosMessage_t const *const msg);

int elosTestElosMessageLogFindEventExtErrRuleFrmMsgTokenerParseSetup(void **state) {
    static elosUteststateT_t testState = {0};

    *state = &testState;
    elosMessageLogFindEventUtestCreateConnection(state);
    elosMessageLogFindEventUtestCreateMessage(state);

    return 0;
}

int elosTestElosMessageLogFindEventExtErrRuleFrmMsgTokenerParseTeardown(void **state) {
    elosMessageLogFindEventUtestFreeConnection(state);
    elosMessageLogFindEventUtestFreeMessage(state);

    return 0;
}

void elosTestElosMessageLogFindEventExtErrRuleFrmMsgTokenerParse(void **state) {
    elosUteststateT_t *testState = *state;
    safuResultE_t result = SAFU_RESULT_OK;
    elosClientManagerConnection_t *conn = testState->connection;
    elosMessage_t *msg = testState->msg;

    TEST("elosMessageLogFindEvent");
    SHOULD("%s", "return SAFU_RESULT_FAILED when json_tokener_parse in _getFilterRuleFromJsonMessage fails");

    MOCK_FUNC_AFTER_CALL(json_tokener_parse, 0);
    expect_string(__wrap_json_tokener_parse, str, msg->json);
    will_return(__wrap_json_tokener_parse, NULL);

    result = elosMessageLogFindEvent(conn, msg);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
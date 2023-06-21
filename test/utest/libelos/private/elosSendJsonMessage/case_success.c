// SPDX-License-Identifier: MIT

#include "elosSendJsonMessage_utest.h"
#include "safu/mock_safu.h"

int elosTestElosSendJsonMessageSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosSendJsonMessageSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosSendJsonMessageSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosSendJsonMessage");
    SHOULD("%s", "test correct behaviour of elosSendJsonMessage");

    PARAM("%s", "send message");
    MOCK_FUNC_AFTER_CALL(elosSendMessage, 0);
    expect_not_value(elosSendMessage, session, NULL);
    expect_memory(elosSendMessage, message, test->normal.message, test->normal.messageSize);
    will_return(elosSendMessage, SAFU_RESULT_OK);

    result = elosSendJsonMessage(&test->session, test->normal.message->message, test->normal.jsonObject);
    assert_int_equal(result, SAFU_RESULT_OK);

    PARAM("%s", "send message with empty jsonObject");
    MOCK_FUNC_AFTER_CALL(elosSendMessage, 0);
    expect_not_value(elosSendMessage, session, NULL);
    expect_memory(elosSendMessage, message, test->empty.message, test->empty.messageSize);
    will_return(elosSendMessage, SAFU_RESULT_OK);

    result = elosSendJsonMessage(&test->session, test->empty.message->message, test->empty.jsonObject);
    assert_int_equal(result, SAFU_RESULT_OK);
}

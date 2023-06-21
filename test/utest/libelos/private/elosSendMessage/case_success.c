// SPDX-License-Identifier: MIT

#include "elosSendMessage_utest.h"
#include "safu/mock_safu.h"

int elosTestElosSendMessageSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosSendMessageSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosSendMessageSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosSendMessage");
    SHOULD("%s", "test correct behaviour of elosSendMessage");

    PARAM("%s", "send message with jsonStr");

    MOCK_FUNC_AFTER_CALL(safuSendExactly, 0);
    expect_value(__wrap_safuSendExactly, fd, 0);
    expect_value(__wrap_safuSendExactly, buf, test->message);
    expect_value(__wrap_safuSendExactly, len, sizeof(elosMessage_t) + test->message->length);
    will_return(__wrap_safuSendExactly, sizeof(elosMessage_t) + test->message->length);

    result = elosSendMessage(&test->session, test->message);
    assert_int_equal(result, SAFU_RESULT_OK);

    PARAM("%s", "send message without jsonStr");
    MOCK_FUNC_AFTER_CALL(safuSendExactly, 0);
    expect_value(__wrap_safuSendExactly, fd, 0);
    expect_value(__wrap_safuSendExactly, buf, test->messageNoJson);
    expect_value(__wrap_safuSendExactly, len, sizeof(elosMessage_t));
    will_return(__wrap_safuSendExactly, sizeof(elosMessage_t));

    result = elosSendMessage(&test->session, test->messageNoJson);
    assert_int_equal(result, SAFU_RESULT_OK);
}

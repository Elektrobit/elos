// SPDX-License-Identifier: MIT

#include "elosSendMessage_utest.h"
#include "safu/mock_safu.h"

int elosTestElosSendMessageErrSendSetup(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    test->session.connected = true;

    return 0;
}

int elosTestElosSendMessageErrSendTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosSendMessageErrSend(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosSendMessage");
    SHOULD("%s", "test correct behaviour of elosSendMessage with sending errors");

    PARAM("%s", "safuSendExactly failed");

    MOCK_FUNC_AFTER_CALL(safuSendExactly, 0);
    expect_any(__wrap_safuSendExactly, fd);
    expect_any(__wrap_safuSendExactly, buf);
    expect_any(__wrap_safuSendExactly, len);
    will_set_parameter(__wrap_safuSendExactly, transferred, 0);
    will_return(__wrap_safuSendExactly, SAFU_RESULT_FAILED);

    result = elosSendMessage(&test->session, test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    // Each transmission error resets the connection state.
    test->session.connected = true;

    PARAM("%s", "safuSendExactly with unexpected connection close");

    MOCK_FUNC_AFTER_CALL(safuSendExactly, 0);
    expect_value(__wrap_safuSendExactly, fd, 0);
    expect_value(__wrap_safuSendExactly, buf, test->message);
    expect_value(__wrap_safuSendExactly, len, sizeof(elosMessage_t) + test->message->length);
    will_set_parameter(__wrap_safuSendExactly, transferred, 0);
    will_return(__wrap_safuSendExactly, SAFU_RESULT_CLOSED);

    result = elosSendMessage(&test->session, test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    // Each transmission error resets the connection state.
    test->session.connected = true;

    PARAM("%s", "safuSendExactly with zero bytes");

    MOCK_FUNC_AFTER_CALL(safuSendExactly, 0);
    expect_value(__wrap_safuSendExactly, fd, 0);
    expect_value(__wrap_safuSendExactly, buf, test->message);
    expect_value(__wrap_safuSendExactly, len, sizeof(elosMessage_t) + test->message->length);
    will_set_parameter(__wrap_safuSendExactly, transferred, 0);
    will_return(__wrap_safuSendExactly, SAFU_RESULT_CLOSED);

    result = elosSendMessage(&test->session, test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    // Each transmission error resets the connection state.
    test->session.connected = true;

    PARAM("%s", "safuSendExactly with too few bytes");
    MOCK_FUNC_AFTER_CALL(safuSendExactly, 0);
    expect_value(__wrap_safuSendExactly, fd, 0);
    expect_value(__wrap_safuSendExactly, buf, test->message);
    expect_value(__wrap_safuSendExactly, len, sizeof(elosMessage_t) + test->message->length);
    will_set_parameter(__wrap_safuSendExactly, transferred, sizeof(elosMessage_t));
    will_return(__wrap_safuSendExactly, SAFU_RESULT_FAILED);

    result = elosSendMessage(&test->session, test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    // Each transmission error resets the connection state.
    test->session.connected = true;

    PARAM("%s", "safuSendExactly with too many bytes");
    MOCK_FUNC_AFTER_CALL(safuSendExactly, 0);
    expect_value(__wrap_safuSendExactly, fd, 0);
    expect_value(__wrap_safuSendExactly, buf, test->message);
    expect_value(__wrap_safuSendExactly, len, sizeof(elosMessage_t) + test->message->length);
    will_set_parameter(__wrap_safuSendExactly, transferred, sizeof(elosMessage_t) + (test->message->length * 2));
    will_return(__wrap_safuSendExactly, SAFU_RESULT_FAILED);

    result = elosSendMessage(&test->session, test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}

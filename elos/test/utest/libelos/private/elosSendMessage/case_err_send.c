// SPDX-License-Identifier: MIT

#include <errno.h>

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

    errno = ENETDOWN;
    MOCK_FUNC_AFTER_CALL(safuSendExactly, 0);
    expect_any(__wrap_safuSendExactly, fd);
    expect_any(__wrap_safuSendExactly, buf);
    expect_any(__wrap_safuSendExactly, len);
    will_return(__wrap_safuSendExactly, -1);

    result = elosSendMessage(&test->session, test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    // Each call of unsubscribe will reset session.
    test->session.connected = true;

    PARAM("%s", "safuSendExactly with unexpected connection close");

    errno = 0;
    MOCK_FUNC_AFTER_CALL(safuSendExactly, 0);
    expect_value(__wrap_safuSendExactly, fd, 0);
    expect_value(__wrap_safuSendExactly, buf, test->message);
    expect_value(__wrap_safuSendExactly, len, sizeof(elosMessage_t) + test->message->length);
    will_return(__wrap_safuSendExactly, 0);

    result = elosSendMessage(&test->session, test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    // Each call of unsubscribe will reset session.
    test->session.connected = true;

    PARAM("%s", "safuSendExactly with zero bytes and errno set");

    errno = ENETDOWN;
    MOCK_FUNC_AFTER_CALL(safuSendExactly, 0);
    expect_value(__wrap_safuSendExactly, fd, 0);
    expect_value(__wrap_safuSendExactly, buf, test->message);
    expect_value(__wrap_safuSendExactly, len, sizeof(elosMessage_t) + test->message->length);
    will_return(__wrap_safuSendExactly, 0);

    result = elosSendMessage(&test->session, test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    // Each call of unsubscribe will reset session.
    test->session.connected = true;

    PARAM("%s", "safuSendExactly with too few bytes");
    MOCK_FUNC_AFTER_CALL(safuSendExactly, 0);
    expect_value(__wrap_safuSendExactly, fd, 0);
    expect_value(__wrap_safuSendExactly, buf, test->message);
    expect_value(__wrap_safuSendExactly, len, sizeof(elosMessage_t) + test->message->length);
    will_return(__wrap_safuSendExactly, sizeof(elosMessage_t));

    result = elosSendMessage(&test->session, test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    // Each call of unsubscribe will reset session.
    test->session.connected = true;

    PARAM("%s", "safuSendExactly with too many bytes");
    MOCK_FUNC_AFTER_CALL(safuSendExactly, 0);
    expect_value(__wrap_safuSendExactly, fd, 0);
    expect_value(__wrap_safuSendExactly, buf, test->message);
    expect_value(__wrap_safuSendExactly, len, sizeof(elosMessage_t) + test->message->length);
    will_return(__wrap_safuSendExactly, sizeof(elosMessage_t) + (test->message->length * 2));

    result = elosSendMessage(&test->session, test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}

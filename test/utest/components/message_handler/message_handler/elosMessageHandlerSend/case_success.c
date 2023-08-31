// SPDX-License-Identifier: MIT

#include <stdio.h>

#include "elosMessageHandlerSend_utest.h"
#include "safu/mock_safu.h"

void elosTestElosMessageHandlerSendSuccess(UNUSED void **state) {
    safuResultE_t ret;
    const int mockFd = 666;
    const elosClientConnection_t conn = {.fd = mockFd};

    const uint8_t messageId = ELOS_MESSAGE_EVENT_PUBLISH;
    const char *jsonStr = "{\"payload\":\"this is payload\"}";
    const size_t msgLen = sizeof(elosMessage_t) + strlen(jsonStr) + 1;

    TEST("elosMessageHandlerSend");
    SHOULD("%s", "return 0 and send successfully the json string");

    expect_value(__wrap_safuSendExactly, fd, mockFd);
    expect_any(__wrap_safuSendExactly, buf);
    expect_value(__wrap_safuSendExactly, len, msgLen);
    will_set_parameter(__wrap_safuSendExactly, transferred, msgLen);
    will_return(__wrap_safuSendExactly, SAFU_RESULT_OK);
    MOCK_FUNC_AFTER_CALL(safuSendExactly, 0);

    ret = elosMessageHandlerSend(&conn, messageId, jsonStr);
    assert_int_equal(ret, SAFU_RESULT_OK);
}

// SPDX-License-Identifier: MIT

#include <stdio.h>

#include "elosMessageHandlerSend_utest.h"
#include "safu/mock_safu.h"

void elosTestElosMessageHandlerSendExterrAllocMem(UNUSED void **state) {
    safuResultE_t ret;
    const int mockFd = 666;
    const elosClientConnection_t conn = {.fd = mockFd};

    const uint8_t messageId = ELOS_MESSAGE_EVENT_PUBLISH;
    const char *jsonStr = "{\"payload\":\"this is payload\"}";
    const size_t msgLen = sizeof(elosMessage_t) + strlen(jsonStr) + 1;

    TEST("elosMessageHandlerSend");
    SHOULD("%s", "return -1 when allocMem fails");

    expect_value(__wrap_safuAllocMem, oldptr, NULL);
    expect_value(__wrap_safuAllocMem, newlen, msgLen);
    will_return(__wrap_safuAllocMem, NULL);
    MOCK_FUNC_AFTER_CALL(safuAllocMem, 0);

    ret = elosMessageHandlerSend(&conn, messageId, jsonStr);
    assert_int_equal(ret, SAFU_RESULT_FAILED);
}

// SPDX-License-Identifier: MIT

#include "elosMessageHandlerSendJson_utest.h"

int elosTestElosMessageHandlerSendJsonExterrSendSetup(void **state) {
    elosUnitTestState_t *data =
        elosMessageHandlerSendJsonCreateTestData("{\"topicId\":1,\"payload\":\"this is payload\"}");
    *state = data;
    return 0;
}
int elosTestElosMessageHandlerSendJsonExterrSendTeardown(void **state) {
    return elosMessageHandlerSendJsonFreeTestData(*(elosUnitTestState_t **)state);
}

void elosTestElosMessageHandlerSendJsonExterrSend(UNUSED void **state) {
    elosUnitTestState_t *data = *(elosUnitTestState_t **)state;
    safuResultE_t ret;
    const int mockFd = 666;
    const elosClientManagerConnection_t conn = {.fd = mockFd};
    const uint8_t messageId = ELOS_MESSAGE_EVENT_PUBLISH;

    TEST("elosMessageHandlerSendJson");
    SHOULD("%s", "return -1 if return value of elosMessageHandlerSend is lower than 0");

    assert_non_null(data);
    assert_non_null(data->jsonObject);
    assert_non_null(data->jsonString);

    expect_value(elosMessageHandlerSend, conn, &conn);
    expect_value(elosMessageHandlerSend, messageId, messageId);
    expect_string(elosMessageHandlerSend, jsonStr, data->jsonString);
    will_return(elosMessageHandlerSend, SAFU_RESULT_FAILED);
    MOCK_FUNC_AFTER_CALL(elosMessageHandlerSend, 0);

    ret = elosMessageHandlerSendJson(&conn, messageId, data->jsonObject);
    assert_int_equal(ret, SAFU_RESULT_FAILED);
}

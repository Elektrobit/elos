// SPDX-License-Identifier: MIT

#include "elosMessageHandlerSendJson_utest.h"

int elosTestElosMessageHandlerSendJsonExterrJsonObjectSetup(void **state) {
    json_object *jobj = json_tokener_parse("{\"topicId\":1,\"payload\":\"this is payload\"}");
    *state = jobj;
    return 0;
}
int elosTestElosMessageHandlerSendJsonExterrJsonObjectTeardown(void **state) {
    json_object *jobj = *(json_object **)state;
    json_object_put(jobj);
    return 0;
}

void elosTestElosMessageHandlerSendJsonExterrJsonObject(void **state) {
    json_object *jobj = *(json_object **)state;
    safuResultE_t ret;
    const int mockFd = 666;
    const elosClientConnection_t conn = {.fd = mockFd};
    const uint8_t messageId = ELOS_MESSAGE_EVENT_PUBLISH;

    TEST("elosMessageHandlerSendJson");
    SHOULD("%s", "return -1 when json_object_to_json_string_ext fails");

    assert_non_null(jobj);

    expect_value(__wrap_json_object_to_json_string_ext, obj, jobj);
    expect_value(__wrap_json_object_to_json_string_ext, flags, JSON_C_TO_STRING_PLAIN);
    will_return(__wrap_json_object_to_json_string_ext, NULL);
    MOCK_FUNC_AFTER_CALL(json_object_to_json_string_ext, 0);

    ret = elosMessageHandlerSendJson(&conn, messageId, jobj);
    assert_int_equal(ret, SAFU_RESULT_FAILED);
}

// SPDX-License-Identifier: MIT
#include "elosReceiveMessage_utest.h"

#include "safu/mock_safu.h"

TEST_SUITE_FUNC_PROTOTYPES(_testSuite)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosReceiveMessageErrAllocation),  TEST_CASE(elosTestElosReceiveMessageErrParam),
        TEST_CASE(elosTestElosReceiveMessageErrReceiveBody), TEST_CASE(elosTestElosReceiveMessageErrReceiveHeader),
        TEST_CASE(elosTestElosReceiveMessageSuccess),
    };

    return RUN_TEST_SUITE(tests, _testSuite);
}

static int _testSuiteSetup(void **state) {
    elosUnitTestState_t *test;
    safuResultE_t result;
    char const *jsonStr = "{\"payload\":\"test123\"}";

    test = safuAllocMem(NULL, sizeof(elosUnitTestState_t));
    assert_non_null(test);

    memset(test, 0x00, sizeof(elosUnitTestState_t));

    test->session.connected = true;

    result = elosCreateMessage(ELOS_MESSAGE_GET_VERSION, NULL, &test->receiveNoJson.message);
    assert_int_equal(result, SAFU_RESULT_OK);
    test->receiveNoJson.header.len = sizeof(elosMessage_t);
    test->receiveNoJson.header.buf = test->receiveNoJson.message;
    test->receiveNoJson.header.result = test->receiveNoJson.header.len;
    test->receiveNoJson.messageSize = test->receiveJson.header.len;

    result = elosCreateMessage(ELOS_MESSAGE_EVENT_PUBLISH, jsonStr, &test->receiveJson.message);
    assert_int_equal(result, SAFU_RESULT_OK);
    test->receiveJson.header.len = sizeof(elosMessage_t);
    test->receiveJson.header.buf = test->receiveJson.message;
    test->receiveJson.header.result = test->receiveJson.header.len;
    test->receiveJson.body.len = strlen(jsonStr) + 1;
    test->receiveJson.body.buf = test->receiveJson.message->json;
    test->receiveJson.body.result = test->receiveJson.body.len;
    test->receiveJson.messageSize = test->receiveJson.header.len + test->receiveJson.body.len;

    *state = test;

    return 0;
}

static int _testSuiteTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    free(test->receiveJson.message);
    free(test->receiveNoJson.message);
    free(*state);

    return 0;
}

void elosMockReceiveExactlySetup(elosReceiveValues_t const *header, elosReceiveValues_t const *body) {
    assert_non_null(header);

    if (body == NULL) {
        MOCK_FUNC_AFTER_CALL(safuRecvExactly, 0);
    } else {
        MOCK_FUNC_ALWAYS(safuRecvExactly);
    }

    expect_value(__wrap_safuRecvExactly, fd, header->fd);
    expect_not_value(__wrap_safuRecvExactly, buf, NULL);
    expect_value(__wrap_safuRecvExactly, len, header->len);
    will_set_parameter(__wrap_safuRecvExactly, buf, header->buf);
    will_return(__wrap_safuRecvExactly, header->result);

    if (body != NULL) {
        expect_value(__wrap_safuRecvExactly, fd, body->fd);
        expect_not_value(__wrap_safuRecvExactly, buf, NULL);
        expect_value(__wrap_safuRecvExactly, len, body->len);
        will_set_parameter(__wrap_safuRecvExactly, buf, body->buf);
        will_return(__wrap_safuRecvExactly, body->result);
    }
}

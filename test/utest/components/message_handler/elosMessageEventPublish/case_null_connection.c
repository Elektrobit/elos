// SPDX-License-Identifier: MIT

#include "elos/event/event.h"
#include "elos/eventfilter/eventfilter.h"
#include "elosMessageEventPublish_utest.h"
#include "json-c/json.h"
#include "mock_LogAggregator.h"
#include "mock_event.h"
#include "mock_eventprocessor.h"
#include "mock_message_handler.h"
#include "safu/common.h"
#include "safu/log.h"
#include "safu/mock_log.h"
#include "safu/mock_safu.h"

extern int elosMessageEventPublish(elosClientManagerConnection_t const *const conn, elosMessage_t const *const msg);

int elosTestElosMessageEventPublishConnectionNullErrorSetup(void **state UNUSED) {
    elosUtestState_t *data = safuAllocMem(NULL, sizeof(elosUtestState_t));
    assert_non_null(data);

    data->conn = NULL;

    const char *msg =
        "{\"topicID\": 0, \"topic\": \"test\", \"date\": [2000, 100], \"source\": \"Test\", \"severity\": 0, "
        "\"hardwareid\":\"0000\", \"classification\": 0, \"messageCode\": 0, \"payload\": \"testevent\"}";
    data->event = NULL;

    data->msg = safuAllocMem(NULL, sizeof(elosMessage_t *) + strlen(msg) + 1);
    assert_non_null(data->msg);
    memcpy(data->msg->json, msg, strlen(msg) + 1);

    *state = data;

    return 0;
}

int elosTestElosMessageEventPublishConnectionNullErrorTeardown(void **state UNUSED) {
    elosUtestState_t *data = *state;
    free(data->msg);
    free(data);

    return 0;
}

void elosTestElosMessageEventPublishConnectionNullError(void **state) {
    safuResultE_t ret;
    elosUtestState_t *data = *state;
    const char *errstr = "elosMessageEventPublish called with null parameters";

    TEST("elosMessageEventPublish");
    SHOULD("%s", "Return SAFU_RESULT_FAILED when elosEventProcessorPublish called with null connection parameter");

    MOCK_FUNC_AFTER_CALL(safuLogFunc, 0);
    expect_value(__wrap_safuLogFunc, level, SAFU_LOG_LEVEL_ERR);
    expect_any(__wrap_safuLogFunc, file);
    expect_any(__wrap_safuLogFunc, func);
    expect_any(__wrap_safuLogFunc, line);
    expect_string(__wrap_safuLogFunc, message, errstr);
    will_return(__wrap_safuLogFunc, SAFU_LOG_STATUS_SUCCESS);

    ret = elosMessageEventPublish(data->conn, data->msg);

    assert_int_equal(ret, SAFU_RESULT_FAILED);
}

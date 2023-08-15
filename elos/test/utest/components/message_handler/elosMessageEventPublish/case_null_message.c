// SPDX-License-Identifier: MIT

#include <samconf/mock_samconf.h>

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

extern int elosMessageEventPublish(elosClientConnection_t const *const conn, elosMessage_t const *const msg);

int elosTestElosMessageEventPublishMessageNullErrorSetup(void **state) {
    elosUtestState_t *data = safuAllocMem(NULL, sizeof(elosUtestState_t));
    assert_non_null(data);

    data->conn = safuAllocMem(NULL, sizeof(elosClientConnection_t));
    assert_non_null(data->conn);

    *state = data;

    return 0;
}

int elosTestElosMessageEventPublishMessageNullErrorTeardown(void **state) {
    elosUtestState_t *data = *state;
    free(data->conn);
    free(data);

    return 0;
}

void elosTestElosMessageEventPublishMessageNullError(void **state) {
    safuResultE_t ret;
    elosUtestState_t *data = *state;
    const char *errstr = "elosMessageEventPublish called with null parameters";

    TEST("elosMessageEventPublish");
    SHOULD("%s", "Return SAFU_RESULT_FAILED when elosEventProcessorPublish called with null message parameter");

    MOCK_FUNC_AFTER_CALL(safuLogFunc, 0);
    expect_value(__wrap_safuLogFunc, level, SAFU_LOG_LEVEL_ERR);
    expect_any(__wrap_safuLogFunc, file);
    expect_any(__wrap_safuLogFunc, func);
    expect_any(__wrap_safuLogFunc, line);
    expect_string(__wrap_safuLogFunc, message, errstr);
    will_return(__wrap_safuLogFunc, SAFU_LOG_STATUS_SUCCESS);

    ret = elosMessageEventPublish(data->conn, NULL);

    assert_int_equal(ret, SAFU_RESULT_FAILED);
}

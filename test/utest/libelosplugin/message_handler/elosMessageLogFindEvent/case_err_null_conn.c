// SPDX-License-Identifier: MIT

#include <json-c/json_types.h>
#include <safu/result.h>

#include "elosMessageLogFindEvent_utest.h"
#include "mock_message_handler.h"

extern int elosMessageLogFindEvent(elosClientConnection_t const *const conn, elosMessage_t const *const msg);

int elosTestElosMessageLogFindEventNullConnectionErrorSetup(void **state) {
    elosUtestState_t *data = safuAllocMem(NULL, sizeof(elosUtestState_t));
    assert_non_null(data);
    data->conn = NULL;
    *state = data;
    return 0;
}

int elosTestElosMessageLogFindEventNullConnectionErrorTeardown(void **state) {
    elosUtestState_t *data = *state;
    free(data->msg);
    free(data);
    return 0;
}

void elosTestElosMessageLogFindEventNullConnectionError(void **state) {
    safuResultE_t ret;
    elosUtestState_t *data = *state;

    TEST("elosMessageLogFindEvent");
    SHOULD("%s", "return SAFU_RESULT_FAILED as conn is NULL");

    const char findEventsRequest[] =
        "{\
    \"filter\": \".event.name 'sshd' STRCMP\",\
    \"newest\": [0,0],\
    \"oldest\": [0,0]\
    }";
    data->msg = safuAllocMem(NULL, sizeof(*(data->msg)) + sizeof(findEventsRequest));
    assert_non_null_msg(data->msg, "safuAllocMem failed");
    strcpy(data->msg->json, findEventsRequest);
    data->msg->length = 1;

    ret = elosMessageLogFindEvent(data->conn, data->msg);
    assert_int_equal(ret, SAFU_RESULT_FAILED);
}

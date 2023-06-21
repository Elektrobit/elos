// SPDX-License-Identifier: MIT
#include <stdlib.h>

#include "cmocka_mocks/mock_jsonc.h"
#include "elosMessageEventSubscribe_utest.h"
#include "mock_event.h"
#include "mock_message_handler.h"
#include "safu/common.h"
#include "safu/mock_log.h"
#include "safu/mock_safu.h"

int elosTestElosMessageEventSubscribeNoStringSetup(void **state) {
    elosUtestState_t *data = safuAllocMem(NULL, sizeof(elosUtestState_t));
    memset(data, 0, sizeof(elosUtestState_t));
    data->response = __real_json_object_new_object();
    data->conn = safuAllocMem(NULL, sizeof(elosClientManagerConnection_t));
    data->conn->sharedData = safuAllocMem(NULL, sizeof(elosClientManagerSharedData_t));
    data->conn->sharedData->eventProcessor = safuAllocMem(NULL, sizeof(elosEventProcessor_t));
    *state = (void *)data;
    return 0;
}

int elosTestElosMessageEventSubscribeNoStringTeardown(void **state) {
    elosUtestState_t *data = (elosUtestState_t *)*state;
    __real_json_object_put(data->jobj);
    __real_json_object_put(data->response);
    free(data->msg);
    free(data->conn->sharedData->eventProcessor);
    free(data->conn->sharedData);
    free(data->conn);
    free(data);
    return 0;
}

void elosTestElosMessageEventSubscribeNoString(void **state) {
    safuResultE_t result;
    const char *msg =
        "{\"filter\":["
        "\"event.source.appName,'myApp',eq\","
        "\"event.source.appName,'linux',eq\","
        "\"event.source.appName,'openssh',eq\""
        "]}";
    elosUtestState_t *data;
    json_object *filter;

    TEST("elosMessageEventSubscribe");
    SHOULD("%s", "fail, as safuJsonGetString does return with error");

    data = (elosUtestState_t *)*state;
    data->msg = safuAllocMem(NULL, sizeof(elosMessage_t *) + strlen(msg) + 1);
    memcpy(data->msg->json, msg, strlen(msg) + 1);
    data->jobj = __real_json_tokener_parse(msg);
    filter = __real_json_object_object_get(data->jobj, "filter");

    assert_non_null(data->jobj);

    MOCK_FUNC_ALWAYS(json_tokener_parse);
    MOCK_FUNC_ALWAYS(json_object_put);
    MOCK_FUNC_ALWAYS(safuLogFuncF);
    MOCK_FUNC_ALWAYS(safuJsonGetArray);
    MOCK_FUNC_ALWAYS(safuJsonGetString);
    MOCK_FUNC_ALWAYS(elosMessageHandlerResponseCreate);
    MOCK_FUNC_ALWAYS(elosMessageHandlerSendJson);

    expect_string(__wrap_json_tokener_parse, str, msg);
    will_return(__wrap_json_tokener_parse, data->jobj);

    expect_value(__wrap_safuJsonGetArray, jobj, data->jobj);
    expect_string(__wrap_safuJsonGetArray, name, "filter");
    expect_value(__wrap_safuJsonGetArray, idx, 0);
    expect_any(__wrap_safuJsonGetArray, len);
    will_set_parameter(__wrap_safuJsonGetArray, len, 3);
    will_return(__wrap_safuJsonGetArray, filter);

    expect_value(__wrap_safuJsonGetString, jobj, filter);
    expect_value(__wrap_safuJsonGetString, name, NULL);
    expect_value(__wrap_safuJsonGetString, idx, 0);
    expect_any(__wrap_safuJsonGetString, val);
    will_set_parameter(__wrap_safuJsonGetString, val, "");
    will_return(__wrap_safuJsonGetString, -1);

    expect_value(__wrap_safuLogFuncF, level, SAFU_LOG_LEVEL_ERR);
    expect_any(__wrap_safuLogFuncF, file);
    expect_any(__wrap_safuLogFuncF, func);
    expect_any(__wrap_safuLogFuncF, line);
    expect_string(__wrap_safuLogFuncF, message, "safuJsonGetString failed");
    will_return(__wrap_safuLogFuncF, SAFU_LOG_STATUS_SUCCESS);

    expect_value(__wrap_safuLogFuncF, level, SAFU_LOG_LEVEL_DEBUG);
    expect_any(__wrap_safuLogFuncF, file);
    expect_any(__wrap_safuLogFuncF, func);
    expect_any(__wrap_safuLogFuncF, line);
    expect_string(__wrap_safuLogFuncF, message,
                  "failed to extract string from json at id: '{\"filter\":["
                  "\"event.source.appName,'myApp',eq\","
                  "\"event.source.appName,'linux',eq\","
                  "\"event.source.appName,'openssh',eq\""
                  "]}', 0");
    will_return(__wrap_safuLogFuncF, SAFU_LOG_STATUS_SUCCESS);

    expect_value(__wrap_json_object_put, obj, data->jobj);
    will_return(__wrap_json_object_put, 0);

    expect_string(elosMessageHandlerResponseCreate, errstr, "failed to extract string from json");
    will_return(elosMessageHandlerResponseCreate, NULL);

    expect_value(__wrap_safuLogFuncF, level, SAFU_LOG_LEVEL_ERR);
    expect_any(__wrap_safuLogFuncF, file);
    expect_any(__wrap_safuLogFuncF, func);
    expect_any(__wrap_safuLogFuncF, line);
    expect_string(__wrap_safuLogFuncF, message, "elosMessageHandlerResponseCreate");
    will_return(__wrap_safuLogFuncF, SAFU_LOG_STATUS_SUCCESS);

    expect_value(__wrap_safuLogFuncF, level, SAFU_LOG_LEVEL_DEBUG);
    expect_any(__wrap_safuLogFuncF, file);
    expect_any(__wrap_safuLogFuncF, func);
    expect_any(__wrap_safuLogFuncF, line);
    expect_string(__wrap_safuLogFuncF, message, "failed to create response message");
    will_return(__wrap_safuLogFuncF, SAFU_LOG_STATUS_SUCCESS);

    result = elosMessageEventSubscribe(data->conn, data->msg);

    MOCK_FUNC_NEVER(json_tokener_parse);
    MOCK_FUNC_NEVER(json_object_put);
    MOCK_FUNC_NEVER(safuLogFuncF);
    MOCK_FUNC_NEVER(safuJsonGetArray);
    MOCK_FUNC_NEVER(safuJsonGetString);
    MOCK_FUNC_NEVER(elosMessageHandlerResponseCreate);
    MOCK_FUNC_NEVER(elosMessageHandlerSendJson);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}

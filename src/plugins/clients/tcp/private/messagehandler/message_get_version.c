// SPDX-License-Identifier: MIT
#include <elos/common/message.h>
#include <safu/json.h>
#include <version.h>

#include "messagehandler/message_handler.h"

safuResultE_t elosMessageGetVersion(elosClientConnection_t *conn, UNUSED elosMessage_t const *const msg) {
    struct json_object *jobj;

    jobj = elosMessageHandlerResponseCreate(NULL);
    if (jobj == NULL) {
        return SAFU_RESULT_FAILED;
    }

    if (safuJsonAddNewString(jobj, "version", elosGetVersionString()) == NULL) {
        json_object_put(jobj);
        return SAFU_RESULT_FAILED;
    }
    safuResultE_t result = elosMessageHandlerSendJson(conn, ELOS_MESSAGE_RESPONSE_GET_VERSION, jobj);
    json_object_put(jobj);

    return result;
}

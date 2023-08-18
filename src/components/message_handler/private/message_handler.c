// SPDX-License-Identifier: MIT
#include "elos/messages/message_handler.h"

#include <elos/logger/logger.h>
#include <safu/common.h>
#include <safu/json.h>
#include <safu/log.h>
#include <stdlib.h>

#include "elos/clientmanager/clientconnection_types.h"

typedef safuResultE_t elosMessageHandler_t(elosClientConnection_t const *const conn, elosMessage_t const *const msg);

extern elosMessageHandler_t elosMessageGetVersion;
extern elosMessageHandler_t elosMessageEventPublish;
extern elosMessageHandler_t elosMessageEventSubscribe;
extern elosMessageHandler_t elosMessageEventUnsubscribe;
extern elosMessageHandler_t elosMessageLogFindEvent;
extern elosMessageHandler_t elosMessageEventQueueRead;

// clang-format off
static elosMessageHandler_t * const elosMessageHandler[] = {
    [ELOS_MESSAGE_GET_VERSION] = elosMessageGetVersion,
    [ELOS_MESSAGE_EVENT_PUBLISH] = elosMessageEventPublish,
    [ELOS_MESSAGE_EVENT_SUBSCRIBE] = elosMessageEventSubscribe,
    [ELOS_MESSAGE_EVENT_UNSUBSCRIBE] = elosMessageEventUnsubscribe,
    [ELOS_MESSAGE_LOG_FIND_EVENT] = elosMessageLogFindEvent,
    [ELOS_MESSAGE_EVENTQUEUE_READ] = elosMessageEventQueueRead,
};
// clang-format on

safuResultE_t elosMessageHandlerSend(elosClientConnection_t const *const conn, uint8_t messageId, const char *jsonStr) {
    safuResultE_t result = SAFU_RESULT_OK;
    const int jsonLen = strlen(jsonStr) + 1;
    const int msgLen = sizeof(elosMessage_t) + jsonLen;
    elosMessage_t *msg;

    msg = safuAllocMem(NULL, msgLen);
    if (msg == NULL) {
        safuLogErr("safuAllocMem failed");
        result = SAFU_RESULT_FAILED;
    } else {
        memset(msg, 0, msgLen);
        msg->version = ELOS_PROTOCOL_VERSION;
        msg->message = messageId;
        msg->length = jsonLen;
        memcpy(msg->json, jsonStr, jsonLen);
    }

    if (result == SAFU_RESULT_OK) {
        size_t bytes;
        result = safuSendExactly(conn->fd, msg, msgLen, &bytes);
        if (result != SAFU_RESULT_OK) {
            if (result != SAFU_RESULT_CLOSED) {
                safuLogErrF("safuSendExactly failed with %d, %ld of %d bytes sent.", result, bytes, msgLen);
            } else {
                safuLogDebug("Connection has been closed");
            }
        }
    }

    free(msg);

    return result;
}

safuResultE_t elosMessageHandlerSendJson(elosClientConnection_t const *const conn, uint8_t messageId,
                                         json_object *jobj) {
    const char *jobjStr;
    int retval;

    jobjStr = json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_PLAIN);
    if (jobjStr == NULL) {
        safuLogErr("json_object_to_json_string_ext failed");
        return SAFU_RESULT_FAILED;
    }

    retval = elosMessageHandlerSend(conn, messageId, jobjStr);
    if (retval < 0) {
        safuLogErr("send_message failed");
    }

    return retval;
}

struct json_object *elosMessageHandlerResponseCreate(const char *errstr) {
    struct json_object *jresponse;

    jresponse = json_object_new_object();
    if (jresponse == NULL) {
        safuLogErr("json_object_new_object 'jresponse' failed");
        return NULL;
    }

    if (errstr == NULL) {
        if (safuJsonAddObject(jresponse, "error", NULL) < 0) {
            safuLogErr("safuJsonAddObject 'error' failed");
            json_object_put(jresponse);
            return NULL;
        }
    } else {
        if (safuJsonAddNewString(jresponse, "error", errstr) == NULL) {
            safuLogErr("safuJsonAddNewString 'error' failed");
            json_object_put(jresponse);
            return NULL;
        }
    }

    return jresponse;
}

static safuResultE_t elosMessageHandlerReadHeader(elosClientConnection_t const *const conn, elosMessage_t *const msg) {
    safuResultE_t result;
    size_t const msgHdrLen = sizeof(elosMessage_t);
    size_t bytes;

    result = safuRecvExactly(conn->fd, (void *)msg, msgHdrLen, &bytes);
    if (result != SAFU_RESULT_OK) {
        if (result != SAFU_RESULT_CLOSED) {
            safuLogErrF("safuRecvExactly failed with %d, %ld of %ld bytes received.", result, bytes, msgHdrLen);
        } else {
            safuLogDebug("Connection has been closed");
        }
    }

    return result;
}

static safuResultE_t elosMessageHandlerReadPayload(elosClientConnection_t const *const conn, elosMessage_t **msg) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    size_t const jsonLength = (*msg)->length;

    if (jsonLength == 0) {
        result = SAFU_RESULT_OK;
    } else {
        size_t const msgHdrLen = sizeof(elosMessage_t);

        *msg = safuAllocMem(*msg, msgHdrLen + jsonLength + 1);
        if (*msg == NULL) {
            safuLogErr("safuAllocMem failed");
        } else {
            char *jsonStr = &((*msg)->json[0]);
            size_t bytes;

            result = safuRecvExactly(conn->fd, jsonStr, jsonLength, &bytes);
            if (result != SAFU_RESULT_OK) {
                if (result != SAFU_RESULT_CLOSED) {
                    safuLogErrF("safuRecvExactly failed with %d, %ld of %ld bytes received.", result, bytes,
                                jsonLength);
                } else {
                    safuLogDebug("Connection has been closed");
                }
            } else {
                jsonStr[jsonLength] = 0;
            }
        }
    }

    return result;
}

safuResultE_t elosMessageHandlerHandleMessage(elosClientConnection_t const *const conn) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosMessage_t *msg = NULL;

    if (conn == NULL) {
        safuLogErr("Invalid parameter");
    } else {
        // Note: Inefficient, we should move it into elosClientManagerConnection_t at some point and reuse the memory
        msg = safuAllocMem(NULL, sizeof(elosMessage_t));
        if (msg == NULL) {
            safuLogErr("safuAllocMem failed");
        } else {
            result = SAFU_RESULT_OK;
        }
    }

    if (result == SAFU_RESULT_OK) {
        result = elosMessageHandlerReadHeader(conn, msg);
        if (result == SAFU_RESULT_OK) {
            result = elosMessageHandlerReadPayload(conn, &msg);
        }
    }

    if (result == SAFU_RESULT_OK) {
        if ((msg->message == 0) || (msg->message > ARRAY_SIZE(elosMessageHandler))) {
            safuLogErrF("msg->message has invalid code 0x%02x", msg->message);
            result = SAFU_RESULT_FAILED;
        } else if (elosMessageHandler[msg->message] == NULL) {
            safuLogErrF("msg->message 0x%02x has no messageHandler function!", msg->message);
            result = SAFU_RESULT_FAILED;
        } else {
            result = elosMessageHandler[msg->message](conn, msg);
            if (result != SAFU_RESULT_OK) {
                safuLogErrF("elosMessageHandler failed (msg->message: 0x%02x)", msg->message);
            }
        }
    }

    free(msg);
    return result;
}

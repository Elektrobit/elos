// SPDX-License-Identifier: MIT

#include <safu/json.h>
#include <safu/log.h>

#include "elos/common/types.h"
#include "elos/eventprocessor/eventprocessor.h"
#include "elos/messages/message_handler.h"

safuResultE_t elosMessageEventQueueRead(elosClientManagerConnection_t *conn, elosMessage_t const *const msg) {
    struct json_object *jRequest = NULL;
    struct json_object *jResponse = NULL;
    struct json_object *jEventArray = NULL;
    elosEventVector_t *eventVector = NULL;
    uint32_t eventQueueId = ELOS_ID_INVALID;
    safuResultE_t result;
    const char *errStr = NULL;
    int retVal;

    jRequest = json_tokener_parse(msg->json);
    if (jRequest == NULL) {
        safuLogErr("json_tokener_parse failed");
        errStr = "Parsing the message's json string failed";
    } else {
        retVal = safuJsonGetUint32(jRequest, "eventQueueId", 0, &eventQueueId);
        if (retVal < 0) {
            safuLogErr("safuJsonGetUint32 failed");
            errStr = "Could not find field 'eventQueueId' in the message's json string";
        } else if (eventQueueId == ELOS_ID_INVALID) {
            safuLogErrF("eventQueueId '%d' is invalid", eventQueueId);
            errStr = "Invalid value for 'eventQueueId'";
        }
        json_object_put(jRequest);
    }

    if (errStr == NULL) {
        result = elosEventProcessorQueueRead(conn->sharedData->eventProcessor, eventQueueId, &eventVector);
        if (result != SAFU_RESULT_OK) {
            safuLogErrF("elosEventProcessorQueueRead failed with eventQueueId '%d'", eventQueueId);
            errStr = "Reading from the EventQueue failed";
        } else {
            retVal = elosEventVectorToJsonArray(eventVector, &jEventArray);
            if (retVal < 0) {
                safuLogErr("elosEventVectorToJsonArray failed (events were lost)");
                errStr = "Converting the eventVector into a json array failed (events were lost)";
            }

            elosEventVectorDelete(eventVector);
        }
    }

    result = SAFU_RESULT_OK;

    jResponse = elosMessageHandlerResponseCreate(errStr);
    if (jResponse == NULL) {
        safuLogErr("elosMessageHandlerResponseCreate failed");
        result = SAFU_RESULT_FAILED;
    } else if (errStr == NULL) {
        retVal = safuJsonAddObject(jResponse, "eventArray", jEventArray);
        if (retVal < 0) {
            safuLogErr("safuJsonAddObject failed");
            result = SAFU_RESULT_FAILED;
        }
    }

    if (result == SAFU_RESULT_OK) {
        result = elosMessageHandlerSendJson(conn, ELOS_MESSAGE_RESPONSE_EVENTQUEUE_READ, jResponse);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosMessageHandlerSendJson failed (events were lost)");
        }
    } else {
        json_object_put(jEventArray);
    }
    json_object_put(jResponse);

    if (errStr != NULL) {
        result = SAFU_RESULT_FAILED;
    }

    return result;
}

// SPDX-License-Identifier: MIT

#include <safu/json.h>
#include <safu/log.h>

#include "elos/common/types.h"
#include "elos/eventprocessor/eventfiltermanager.h"
#include "elos/eventprocessor/eventqueuemanager.h"
#include "elos/messages/message_handler.h"

safuResultE_t elosMessageEventUnsubscribe(elosClientConnection_t *conn, elosMessage_t const *const msg) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    json_object *request = NULL;
    json_object *response = NULL;
    uint32_t eventQueueId = ELOS_ID_INVALID;
    const char *errStr = NULL;
    int retVal;

    request = json_tokener_parse(msg->json);
    if (request != NULL) {
        retVal = safuJsonGetUint32(request, "eventQueueId", 0, &eventQueueId);
        if (retVal < 0) {
            safuLogErr("safuJsonGetUint32 failed");
            errStr = "Could not find field 'eventQueueId' in the message's json string";
        } else if (eventQueueId == ELOS_ID_INVALID) {
            safuLogErrF("eventQueueId '%d' is invalid", eventQueueId);
            errStr = "Invalid value for 'eventQueueId'";
        } else {
            safuLogDebugF("Removing filters from event queue %d.", eventQueueId);
            result = elosEventFilterManagerNodeRemoveByEventQueueId(
                &conn->sharedData->eventProcessor->eventFilterManager, eventQueueId);
            if (result == SAFU_RESULT_OK) {
                safuLogDebugF("Removing event queue %d.", eventQueueId);
                result = elosEventQueueManagerEntryDelete(&conn->sharedData->eventProcessor->eventQueueManager,
                                                          eventQueueId);
                if (result != SAFU_RESULT_OK) {
                    safuLogErrF("elosEventQueueManagerEntryDelete failed with eventQueueId '%d'", eventQueueId);
                    errStr = "Removing the EventQueue failed";
                }
            } else {
                safuLogErrF("elosEventFilterManagerNodeRemoveByEventQueueId failed with eventQueueId '%d'",
                            eventQueueId);
                errStr = "Removing the EventQueue filters failed";
            }
        }

        json_object_put(request);
    } else {
        safuLogErr("json_tokener_parse failed");
        errStr = "Parsing the message's json string failed";
    }

    response = elosMessageHandlerResponseCreate(errStr);
    if (response != NULL) {
        result = elosMessageHandlerSendJson(conn, ELOS_MESSAGE_RESPONSE_EVENT_UNSUBSCRIBE, response);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosMessageHandlerSendJson failed (events were lost)");
        }

        json_object_put(response);
    } else {
        safuLogErr("elosMessageHandlerResponseCreate failed");
        result = SAFU_RESULT_FAILED;
    }

    if (errStr != NULL) {
        result = SAFU_RESULT_FAILED;
    }

    return result;
}

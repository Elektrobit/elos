// SPDX-License-Identifier: MIT

#include <safu/json.h>
#include <safu/log.h>

#include "elos/common/types.h"
#include "elos/eventprocessor/eventfiltermanager.h"
#include "elos/eventprocessor/eventprocessor.h"
#include "elos/eventprocessor/eventqueuemanager.h"
#include "elos/messages/message_handler.h"

safuResultE_t elosMessageEventSubscribe(elosClientManagerConnection_t *conn, elosMessage_t const *const msg) {
    const uint8_t messageId = ELOS_MESSAGE_RESPONSE_EVENT_SUBSCRIBE;
    json_object *request = NULL;
    json_object *jResponse = NULL;
    json_object *jFilterArray = NULL;

    const char *errStr = NULL;
    char **filterStrings = NULL;
    size_t arrayLength;
    int retval;
    elosEventProcessor_t *eventProcessor = conn->sharedData->eventProcessor;
    elosEventFilterNodeId_t eventFilterNodeId = ELOS_ID_INVALID;
    elosEventQueueId_t eventQueueId = ELOS_ID_INVALID;
    safuResultE_t result = SAFU_RESULT_OK;

    request = json_tokener_parse(msg->json);
    if (request == NULL) {
        safuLogErrF("%s", "json_tokener_parse");
        safuLogDebugF("failed to parse json: '%s'", msg->json);
        errStr = "failed to parse json";
    }

    if (errStr == NULL) {
        jFilterArray = safuJsonGetArray(request, "filter", 0, &arrayLength);
        if (jFilterArray == NULL) {
            safuLogErrF("%s", "safuJsonGetArray failed");
            safuLogDebugF("failed to extract filter array: '%s'", msg->json);
            errStr = "failed to extract filter array";
        }
    }

    if (errStr == NULL) {
        filterStrings = safuAllocMem(NULL, arrayLength * sizeof(char *));
        if (filterStrings == NULL) {
            safuLogErr("safuAllocMem failed");
            errStr = "failed to allocate memory for filter array";
        } else {
            memset(filterStrings, 0, arrayLength * sizeof(char *));

            for (size_t i = 0; i < arrayLength; i++) {
                char const *filterArrayString;

                retval = safuJsonGetString(jFilterArray, NULL, i, &filterArrayString);
                if (retval == -1) {
                    safuLogErrF("%s", "safuJsonGetString failed");
                    safuLogDebugF("failed to extract string from json at id: '%s', %lu", msg->json, i);
                    errStr = "failed to extract string from json";
                    break;
                } else {
                    filterStrings[i] = strdup(filterArrayString);
                    if (filterStrings[i] == NULL) {
                        errStr = "failed to allocate memory for filter array string";
                        safuLogErrF("%s", "strdup failed");
                        safuLogDebugF("%s at i=%lu", errStr, i);
                        break;
                    }
                }
            }
        }
    }

    if (request != NULL) {
        json_object_put(request);
    }

    if (errStr == NULL) {
        result = elosEventProcessorFilterNodeCreateWithQueue(eventProcessor, (char const **)filterStrings, arrayLength,
                                                             &eventQueueId, &eventFilterNodeId);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErrF("%s", "elosEventProcessorFilterNodeCreateWithQueue failed");
            safuLogDebugF("%s", "failed to create eventQueue and/or eventFilterNode");
            errStr = "failed to initialize eventFilterNode/eventQueue";
        } else {
            // These will be made obsolete in the future with eventProcessor improvements
            retval = safuVecPush(&conn->data.eventFilterNodeIdVector, &eventFilterNodeId);
            if (retval < 0) {
                safuLogErrF("safuVecPush with filterNodeId '%u' failed", eventFilterNodeId);
                errStr = "failed to push eventFilterNodeId";
            } else {
                retval = safuVecPush(&conn->data.eventQueueIdVector, &eventQueueId);
                if (retval < 0) {
                    safuLogErrF("safuVecPush with eventQueueId '%u' failed", eventQueueId);
                    errStr = "failed to push eventQueueId";
                }
            }
        }
    }

    if (filterStrings != NULL) {
        for (size_t i = 0; i < arrayLength; i++) {
            free(filterStrings[i]);
        }
        free(filterStrings);
    }

    result = SAFU_RESULT_FAILED;

    jResponse = elosMessageHandlerResponseCreate(errStr);
    if (jResponse == NULL) {
        safuLogErrF("%s", "elosMessageHandlerResponseCreate");
        safuLogDebugF("%s", "failed to create response message");
    } else if (errStr != NULL) {
        result = SAFU_RESULT_OK;
    } else {
        json_object *jEqIdArray;

        jEqIdArray = safuJsonAddNewArray(jResponse, "eventQueueIds");
        if (jEqIdArray == NULL) {
            safuLogErrF("%s", "safuJsonAddNewArray failed");
            safuLogDebugF("%s", "failed to create response eventqueue array");
        } else {
            json_object *jRet;

            jRet = safuJsonAddNewInt64(jEqIdArray, NULL, eventQueueId);
            if (jRet == NULL) {
                safuLogErrF("%s", "safuJsonAddNewInt64 failed");
                safuLogDebugF("%s", "failed to add eventqueueid to array");
            } else {
                result = SAFU_RESULT_OK;
            }
        }
    }

    if (result == SAFU_RESULT_OK) {
        result = elosMessageHandlerSendJson(conn, messageId, jResponse);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErrF("%s", "elosMessageHandlerSendJson failed");
        } else if (errStr != NULL) {
            result = SAFU_RESULT_FAILED;
        }
        json_object_put(jResponse);
    }

    if (result == SAFU_RESULT_FAILED) {
        if (eventFilterNodeId != ELOS_ID_INVALID) {
            elosEventFilterManagerNodeRemove(&eventProcessor->eventFilterManager, eventFilterNodeId);
        }
        if (eventQueueId != ELOS_ID_INVALID) {
            elosEventQueueManagerEntryDelete(&eventProcessor->eventQueueManager, eventQueueId);
        }
    }

    return result;
}

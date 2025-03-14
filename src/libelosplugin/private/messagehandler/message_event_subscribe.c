// SPDX-License-Identifier: MIT

#include <elos/libelosplugin/libelosplugin.h>
#include <safu/common.h>
#include <safu/json.h>
#include <safu/log.h>

#include "elos/common/message.h"
#include "elos/common/types.h"
#include "elos/libelosplugin/message_handler.h"

safuResultE_t elosMessageEventSubscribe(elosClientConnection_t *conn, elosMessage_t const *const msg) {
    const uint8_t messageId = ELOS_MESSAGE_RESPONSE_EVENT_SUBSCRIBE;
    json_object *request = NULL;
    json_object *jResponse = NULL;
    json_object *jFilterArray = NULL;
    const elosSubscription_t *subscription = NULL;
    const char *errStr = NULL;
    char **filterStrings = NULL;
    size_t arrayLength;
    int retval;
    safuResultE_t result = SAFU_RESULT_OK;

    if (msg->length == 0) {
        safuLogErr("header sayes the JSON has lenth 0");
        result = SAFU_RESULT_FAILED;
        errStr = "invalid message: message length cannot be 0";
    }

    if (errStr == NULL) {
        request = json_tokener_parse(msg->json);
        if (request == NULL) {
            safuLogErrF("%s", "json_tokener_parse");
            safuLogDebugF("failed to parse json: '%s'", msg->json);
            errStr = "failed to parse json";
        }
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
        result = elosPluginSubscribe(conn->sharedData->plugin, conn->data.subscriber,
                                     (char const *const *)filterStrings, arrayLength, &subscription);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErr("failed to subscribe");
            errStr = "failed to subscribe";
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

            jRet = safuJsonAddNewInt64(jEqIdArray, NULL, subscription->eventQueueId);
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

    return result;
}

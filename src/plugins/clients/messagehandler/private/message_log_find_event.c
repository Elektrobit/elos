// SPDX-License-Identifier: MIT
#include <elos/libelosplugin/libelosplugin.h>
#include <safu/common.h>
#include <safu/json.h>
#include <safu/log.h>
#include <stdbool.h>
#include <stdio.h>

#include "elos/common/message.h"
#include "elos/event/event_vector.h"  // elosEventVector<Initialize, ToJsonObject, DeleteMembers>
#include "messagehandler/message_handler.h"

// The vector will grow using realloc, if more events than VEC_START_SIZE are pushed to it
#ifndef MESSAGE_HANDLER_LOG_FIND_EVENT_VEC_START_SIZE
#define MESSAGE_HANDLER_LOG_FIND_EVENT_VEC_START_SIZE 8
#endif

static safuResultE_t _getFilterRuleFromJsonMessage(char **filterRule, const char *jsonMessage) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    json_object *jobj = NULL;
    const char *tmp = NULL;

    jobj = json_tokener_parse(jsonMessage);
    if (jobj == NULL) {
        safuLogErr("json_tokener_parse failed!");
        safuLogDebugF("failed to parse json: %s", jsonMessage);
    } else if (safuJsonGetString(jobj, "filter", 0, &tmp) != 0) {
        safuLogErr("safuJsonGetString failed!");
        safuLogDebug("failed to get string from filter object!");
        json_object_put(jobj);
    } else {
        char *newFilterRule = strdup(tmp);
        if (newFilterRule) {
            *filterRule = newFilterRule;
            result = SAFU_RESULT_OK;
        }
        json_object_put(jobj);
    }
    return result;
}

safuResultE_t elosMessageLogFindEvent(elosClientConnection_t *conn, elosMessage_t const *const msg) {
    safuResultE_t result = SAFU_RESULT_OK;
    char *filterRule = NULL;
    const char *errStr = NULL;
    struct json_object *eventVecJarr = NULL;
    struct json_object *jResponse;
    elosEventVector_t eventVector = {0};

    if (conn == NULL) {
        safuLogErr("Called elosMessageLogFindEvent with NULL-connection");
        return SAFU_RESULT_FAILED;
    }

    if (msg == NULL) {
        errStr = "Called elosMessageLogFindEvent with NULL-string";
        safuLogErrF("%s", errStr);
        result = SAFU_RESULT_FAILED;
    }

    if (msg->length == 0) {
        safuLogErr("Called elosMessageLogFindEvent with json of lenth 0");
        errStr = "invalid message: message length cannot be 0";
        result = SAFU_RESULT_FAILED;
    }

    if (result == SAFU_RESULT_OK) {
        safuLogDebugF("received message: %s", msg->json);
        result = _getFilterRuleFromJsonMessage(&filterRule, msg->json);
        if (result != SAFU_RESULT_OK) {
            errStr = "Failed to fetch filter rules";
            safuLogErrF("%s", errStr);
        }
    }

    if (result == SAFU_RESULT_OK) {
        result = elosEventVectorInitialize(&eventVector, MESSAGE_HANDLER_LOG_FIND_EVENT_VEC_START_SIZE);
        if (result != SAFU_RESULT_OK) {
            errStr = "Failed to initialize vector";
            safuLogErrF("%s", errStr);
        }
    }

    if (result == SAFU_RESULT_OK) {
        result = elosPluginFindEvents(conn->sharedData->plugin, filterRule, &eventVector);
        if (result != SAFU_RESULT_OK) {
            errStr = "Failed to find events";
            safuLogErrF("%s", errStr);
        }
    }

    if (result == SAFU_RESULT_OK) {
        result = elosEventVectorToJsonArray(&eventVector, &eventVecJarr);
        if (result != SAFU_RESULT_OK) {
            errStr = "Failed to convert event vector to json";
            safuLogErrF("%s", errStr);
        }
    }

    result = SAFU_RESULT_OK;

    jResponse = elosMessageHandlerResponseCreate(errStr);
    if (jResponse == NULL) {
        safuLogErr("elosMessageHandlerResponseCreate failed");
        json_object_put(eventVecJarr);
        result = SAFU_RESULT_FAILED;
    } else if (errStr == NULL) {
        int retVal = safuJsonAddObject(jResponse, "eventArray", eventVecJarr);
        if (retVal < 0) {
            safuLogErr("safuJsonAddObject failed");
            result = SAFU_RESULT_FAILED;
        }
    }

    if (result == SAFU_RESULT_OK) {
        result = elosMessageHandlerSendJson(conn, ELOS_MESSAGE_RESPONSE_LOG_FIND_EVENT, jResponse);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("Failed to send json message");
        }
    }

    elosEventVectorDeleteMembers(&eventVector);
    free(filterRule);
    if (NULL != jResponse) {
        json_object_put(jResponse);
    }

    return result;
}

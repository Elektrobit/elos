// SPDX-License-Identifier: MIT
#include <elos/libelosplugin/libelosplugin.h>
#include <linux/if_link.h>
#include <safu/common.h>
#include <safu/json.h>
#include <safu/log.h>
#include <safu/result.h>
#include <safu/vector.h>
#include <safu/vector_types.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "elos/common/message.h"
#include "elos/event/event.h"
#include "elos/event/event_types.h"
#include "elos/event/event_vector.h"  // elosEventVector<Initialize, ToJsonObject, DeleteMembers>
#include "elos/libelosplugin/message_handler.h"

// The vector will grow using realloc, if more events than VEC_START_SIZE are pushed to it
#ifndef MESSAGE_HANDLER_LOG_FIND_EVENT_VEC_START_SIZE
#define MESSAGE_HANDLER_LOG_FIND_EVENT_VEC_START_SIZE 8
#endif

static safuResultE_t _getFilterRuleFromJsonMessage(json_object *jRequest, char **filterRule) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    const char *tmp = NULL;

    if (safuJsonGetString(jRequest, "filter", 0, &tmp) != 0) {
        safuLogErr("safuJsonGetString failed!");
        safuLogDebug("failed to get string from filter object!");
    } else {
        char *newFilterRule = strdup(tmp);
        if (newFilterRule != NULL) {
            *filterRule = newFilterRule;
            result = SAFU_RESULT_OK;
        }
    }
    return result;
}

static void _getTimespanFromJsonMessage(const json_object *jRequest, struct timespec *newest, struct timespec *oldest) {
    struct timespec time = {0};
    if (safuJsonGetTimestamp(jRequest, "newest", 0, &time) != 0) {
        safuLogDebug("failed to get newes time");
    }
    *newest = time;

    time = (struct timespec){0};
    if (safuJsonGetTimestamp(jRequest, "oldest", 0, &time) != 0) {
        safuLogDebug("failed to get oldest");
    }
    *oldest = time;
}

#define MAX_PAYLOAD_SIZE (UINT16_MAX - sizeof(elosMessage_t))
static size_t _countDigits(uint64_t number) {
    size_t digitCount = 1;

    for (uint64_t value = number; value >= 10; value = value / 10) {
        digitCount++;
    }
    return digitCount;
}

static size_t _predictResponseSize(elosEventVector_t const *events) {
    size_t eventCount = safuVecElements(events);
    size_t totalSize = 0;
    size_t maxEvents = 0;

    totalSize = (eventCount * sizeof("{\"date\":[,]}")) + sizeof("{\"eventArray\":[],\"isTruncated\": false}");
    for (maxEvents = 0; maxEvents < eventCount; maxEvents++) {
        elosEvent_t const *event = safuVecGet(events, maxEvents);

        totalSize += _countDigits(event->date.tv_sec) + _countDigits(event->date.tv_nsec);

        if (event->source.pid != 0) {
            totalSize += _countDigits(event->source.pid) + sizeof("\"pid\":,");
        }
        if (event->messageCode != 0) {
            totalSize += _countDigits(event->source.pid) + sizeof("\"messageCode\":,");
        }
        if (event->severity != 0) {
            totalSize += _countDigits(event->severity) + sizeof("\"severity\":,");
        }
        if (event->classification != 0) {
            totalSize += _countDigits(event->classification) + sizeof("\"classification\":,");
        }
        if (event->payload != NULL) {
            totalSize += strlen(event->payload) + sizeof("\"payload\":\"\",");
        }
        if (event->hardwareid != NULL) {
            totalSize += strlen(event->hardwareid) + sizeof("\"hardwareid\":\"\",");
        }
        if (event->source.appName != NULL) {
            totalSize += strlen(event->source.appName) + sizeof("\"appName\":\"\",");
        }
        if (event->source.fileName != NULL) {
            totalSize += strlen(event->source.fileName) + sizeof("\"fileName\":\"\",");
        }

        if (totalSize > MAX_PAYLOAD_SIZE) {
            break;
        }
    }

    safuLogDebugF("predict repsonse size: totalSize %zu maxEvents %zu", totalSize, maxEvents);
    return maxEvents;
}

void _truncateVec(elosEventVector_t *eventVector, size_t newSize) {
    while (safuVecElements(eventVector) > newSize) {
        elosEvent_t *event = safuVecGetLast(eventVector);
        elosEventDeleteMembers(event);
        safuVecPop(eventVector);
    }
}

json_object *_jsonAddBool(json_object *root, const char *key, bool value) {
    json_object *jdata = json_object_new_boolean(value);
    if (jdata == NULL) {
        safuLogErr("json_object_new_boolean failed!");
    } else {
        if (safuJsonAddObject(root, key, jdata) < 0) {
            json_object_put(jdata);
            jdata = NULL;
        }
    }
    return jdata;
}

safuResultE_t elosMessageLogFindEvent(elosClientConnection_t *conn, elosMessage_t const *const msg) {
    safuResultE_t result = SAFU_RESULT_OK;
    char *filterRule = NULL;
    struct timespec newest = {0};
    struct timespec oldest = {0};
    const char *errStr = NULL;
    struct json_object *eventVecJarr = NULL;
    struct json_object *jResponse;
    elosEventVector_t eventVector = {0};
    bool isTruncated = false;

    if (conn == NULL) {
        safuLogErr("Called elosMessageLogFindEvent with NULL-connection");
        return SAFU_RESULT_FAILED;
    }

    if (msg == NULL) {
        errStr = "Called elosMessageLogFindEvent with NULL-string";
        safuLogErrF("%s", errStr);
        result = SAFU_RESULT_FAILED;
    }

    if (result == SAFU_RESULT_OK && msg->length == 0) {
        result = SAFU_RESULT_FAILED;
        errStr = "invalid message: message length cannot be 0";
        safuLogErr("elosMessageEventPublish called with json of length 0");
    }

    if (result == SAFU_RESULT_OK) {
        safuLogDebugF("received message: %s", msg->json);
        json_object *jRequest = json_tokener_parse(msg->json);
        if (jRequest == NULL) {
            safuLogErr("json_tokener_parse failed!");
            safuLogDebugF("failed to parse json: %s", msg->json);
            result = SAFU_RESULT_FAILED;
        } else {
            _getTimespanFromJsonMessage(jRequest, &newest, &oldest);
            result = _getFilterRuleFromJsonMessage(jRequest, &filterRule);
            if (result != SAFU_RESULT_OK) {
                errStr = "Failed to fetch filter rules";
                safuLogErrF("%s", errStr);
            }
            json_object_put(jRequest);
        }

        if (result == SAFU_RESULT_OK) {
            result = elosEventVectorInitialize(&eventVector, MESSAGE_HANDLER_LOG_FIND_EVENT_VEC_START_SIZE);
            if (result != SAFU_RESULT_OK) {
                errStr = "Failed to initialize vector";
                safuLogErrF("%s", errStr);
            }
        }

        if (result == SAFU_RESULT_OK) {
            result = elosPluginFindEvents(conn->sharedData->plugin, filterRule, &newest, &oldest, &eventVector);
            if (result != SAFU_RESULT_OK) {
                errStr = "Failed to find events";
                safuLogErrF("%s", errStr);
            } else {
                size_t maxEvents = _predictResponseSize(&eventVector);
                if (maxEvents < safuVecElements(&eventVector)) {
                    _truncateVec(&eventVector, maxEvents);
                    isTruncated = true;
                }
            }
        }

        if (result == SAFU_RESULT_OK) {
            result = elosEventVectorToJsonArray(&eventVector, &eventVecJarr);
            if (result != SAFU_RESULT_OK) {
                errStr = "Failed to convert event vector to json";
                safuLogErrF("%s", errStr);
            }
        }
    }

    result = SAFU_RESULT_OK;

    jResponse = elosMessageHandlerResponseCreate(errStr);
    if (jResponse == NULL) {
        safuLogErr("elosMessageHandlerResponseCreate failed");
        json_object_put(eventVecJarr);
        result = SAFU_RESULT_FAILED;
    } else if (errStr == NULL) {
        json_object *jIsTruncated = _jsonAddBool(jResponse, "isTruncated", isTruncated);
        if (jIsTruncated == NULL) {
            safuLogErr("safuJsonAddBool failed");
            result = SAFU_RESULT_FAILED;
        } else {
            int retVal = safuJsonAddObject(jResponse, "eventArray", eventVecJarr);
            if (retVal < 0) {
                safuLogErr("safuJsonAddObject failed");
                result = SAFU_RESULT_FAILED;
            }
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

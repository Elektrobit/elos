// SPDX-License-Identifier: MIT

#include "dlt_hv/utils.h"

#include <elos/event/event.h>
#include <elos/libelosplugin/libelosplugin.h>
#include <safu/common.h>
#include <safu/log.h>
#include <safu/result.h>
#include <samconf/samconf_types.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "dlt_hv/types.h"

samconfConfigStatusE_t elosConfigGetGenericInt64(const samconfConfig_t *root, const char *path, int64_t *result) {
    const samconfConfig_t *node = NULL;
    samconfConfigStatusE_t status = SAMCONF_CONFIG_NOT_FOUND;

    if (!root || !path || !result) {
        return SAMCONF_CONFIG_ERROR;
    }

    status = samconfConfigGet(root, path, &node);
    if (SAMCONF_CONFIG_OK != status) {
        return status;
    }

    if (node->type == SAMCONF_CONFIG_VALUE_INT) {
        safuLogDebugF("%ld is an integer that can just be used!!!", node->value.integer);
        *result = node->value.integer;
    } else if (node->type == SAMCONF_CONFIG_VALUE_REAL) {
        int64_t v = (int64_t)node->value.real;
        safuLogDebugF("%f is a double that will be transfomred into %ld!!!", node->value.real, v);
        if (node->value.real - (double)v != 0.0) {
            safuLogWarnF("Precision lost when converting %f from double to int64_t %ld", node->value.real, v);
        }
        *result = v;
    } else if (node->type == SAMCONF_CONFIG_VALUE_STRING) {
        safuLogDebugF("\"%s\" is a string that will be parsed", node->value.string);
        char *endPtr = NULL;
        int64_t v = strtoll(node->value.string, &endPtr, 0);
        if (*endPtr == '\0') {
            *result = v;
        } else {
            status = SAMCONF_CONFIG_INVALID_TYPE;
        }
    } else {
        status = SAMCONF_CONFIG_INVALID_TYPE;
    }

    return status;
}

static const elosSeverityE_t elosLogLevelTranslate[] = {
    [ELOS_DLT_LOGLEVEL_OFF] = ELOS_SEVERITY_OFF,     [ELOS_DLT_LOGLEVEL_FATAL] = ELOS_SEVERITY_FATAL,
    [ELOS_DLT_LOGLEVEL_ERROR] = ELOS_SEVERITY_ERROR, [ELOS_DLT_LOGLEVEL_WARN] = ELOS_SEVERITY_WARN,
    [ELOS_DLT_LOGLEVEL_INFO] = ELOS_SEVERITY_INFO,   [ELOS_DLT_LOGLEVEL_DEBUG] = ELOS_SEVERITY_DEBUG,
};

static elosSeverityE_t _logLevelTranslate(elosDltLogLevelE_t level) {
    elosSeverityE_t log = ELOS_SEVERITY_OFF;
    if (level >= ELOS_DLT_LOGLEVEL_OFF && level <= ELOS_DLT_LOGLEVEL_DEBUG) {
        log = elosLogLevelTranslate[level];
    }
    return log;
}

static const char *elosUnicodeSub[] = {
    "⋄", "␁", "␂", "␃", "␄",  "␅", "␆", "␇", "␈", "␉", "␊", "␋", "␌",  "␍", "␎", "␏", "␐", "␑", "␒", "␓", "␔", "␕",
    "␖", "␗", "␘", "␙", "␚",  "␛", "␜", "␝", "␞", "␟", " ", "!", "\"", "#", "$", "%", "&", "'", "(", ")", "*", "+",
    ",", "-", ".", "/", "0",  "1", "2", "3", "4", "5", "6", "7", "8",  "9", ":", ";", "<", "=", ">", "?", "@", "A",
    "B", "C", "D", "E", "F",  "G", "H", "I", "J", "K", "L", "M", "N",  "O", "P", "Q", "R", "S", "T", "U", "V", "W",
    "X", "Y", "Z", "[", "\\", "]", "^", "_", "`", "a", "b", "c", "d",  "e", "f", "g", "h", "i", "j", "k", "l", "m",
    "n", "o", "p", "q", "r",  "s", "t", "u", "v", "w", "x", "y", "z",  "{", "|", "}", "~", "␡", "x", "x", "x", "x",
    "x", "x", "x", "x", "x",  "x", "x", "x", "x", "x", "x", "x", "x",  "x", "x", "x", "x", "x", "x", "x", "x", "x",
    "x", "x", "x", "x", "x",  "x", "x", "x", "x", "x", "x", "x", "x",  "x", "x", "x", "x", "x", "x", "x", "x", "x",
    "x", "x", "x", "x", "x",  "x", "x", "x", "x", "x", "x", "x", "x",  "x", "x", "x", "x", "x", "x", "x", "x", "x",
    "x", "x", "x", "x", "x",  "x", "x", "x", "x", "x", "x", "x", "x",  "x", "x", "x", "x", "x", "x", "x", "x", "x",
    "x", "x", "x", "x", "x",  "x", "x", "x", "x", "x", "x", "x", "x",  "x", "x", "x", "x", "x", "x", "x", "x", "x",
    "x", "x", "x", "x", "x",  "x", "x", "x", "x", "x", "x", "x", "x",  "ﬀ",
};

static size_t _neededPayloadSize(const uint8_t *buffer) {
    size_t res = 0;
    for (size_t i = 0; i < ELOS_EB_LOG_STRING_SIZE; ++i) {
        res += strlen(elosUnicodeSub[buffer[i]]);
    }
    return res;
}
static void _shmemBufferToStr(char **res, const uint8_t *buffer) {
    size_t idx = 0;
    size_t needed = _neededPayloadSize(buffer);
    char *buf = safuAllocMem(NULL, needed + 1);
    for (size_t i = 0; i < ELOS_EB_LOG_STRING_SIZE; ++i) {
        int off = snprintf(&buf[idx], ELOS_EB_LOG_STRING_SIZE, "%s", elosUnicodeSub[buffer[i]]);
        idx += off;
    }
    buf[idx] = 0;
    *res = buf;
}

safuResultE_t elosEventFromLogEntry(elosEbLogEntry_t *entry, elosEvent_t *event) {
    safuResultE_t result = SAFU_RESULT_OK;
    if (entry == NULL || event == NULL) {
        result = SAFU_RESULT_FAILED;
    } else {
        // NOTE:  parse entry if possible
        event->date.tv_sec = entry->creationTime;
        _shmemBufferToStr(&event->payload, (uint8_t *)entry->logString);
        event->severity = _logLevelTranslate(entry->logLevel);
        safuLogDebugF("Payload: %s", event->payload);
    }
    return result;
}

safuResultE_t elosPublishAndStore(elosPlugin_t *plugin, struct elosPublisher *publisher, elosEvent_t *event) {
    safuResultE_t result = elosPluginPublish(plugin, publisher, event);
    if (result != SAFU_RESULT_OK) {
        char *jsonStr = NULL;
        safuResultE_t errRes = elosEventSerialize(&jsonStr, event);
        if (errRes == SAFU_RESULT_OK) {
            safuLogErrF("failed to publish event: %s", jsonStr);
            free(jsonStr);
        } else {
            safuLogErr("failed to publish event");
        }
    } else {
        result = elosPluginStore(plugin, event);
        if (result != SAFU_RESULT_OK) {
            char *jsonStr = NULL;
            safuResultE_t errRes = elosEventSerialize(&jsonStr, event);
            if (errRes == SAFU_RESULT_OK) {
                safuLogErrF("failed to publish event: %s", jsonStr);
                free(jsonStr);
            } else {
                safuLogErr("failed to publish event");
            }
        }
    }
    return result;
}

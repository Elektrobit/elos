// SPDX-License-Identifier: MIT

#include <elos/event/event.h>
#include <elos/libelosplugin/libelosplugin.h>
#include <safu/log.h>
#include <safu/result.h>
#include <stdlib.h>
#include <time.h>

#include "dlt_hv/types.h"

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

safuResultE_t elosEventFromLogEntry(elosEbLogEntry_t *entry, elosEvent_t *event) {
    safuResultE_t result = SAFU_RESULT_OK;
    if (entry == NULL || event == NULL) {
        result = SAFU_RESULT_FAILED;
    } else {
        // NOTE:  parse entry if possible
        event->date.tv_sec = entry->creationTime;
        event->payload = entry->logString;
        event->payload[ELOS_EB_LOG_STRING_SIZE - 1] = 0;  // make sure its 0 terminated
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

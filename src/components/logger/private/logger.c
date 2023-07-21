// SPDX-License-Identifier: MIT
#include "elos/logger/logger.h"

#include "elos/eloslog/eloslog.h"
#include "elos/eloslog/types.h"
#include "elos/eventprocessor/eventprocessor.h"

struct elosLogContext {
    elosEventProcessor_t *logEventProcessor;
};

struct elosLogContext elosLogContext = {.logEventProcessor = NULL};

void elosLogSetEventProcessor(elosEventProcessor_t *lep) {
    elosLogContext.logEventProcessor = lep;
}

elosLogStatusE_t elosLogPublishServerLogEvent(elosEventProcessor_t *elosLogEventProcessor, elosEvent_t *elosLogEvent) {
    int ret = 0;

    if ((elosLogEventProcessor != NULL) && (elosLogEvent != NULL)) {
        ret = elosEventProcessorPublish(elosLogEventProcessor, elosLogEvent);
    }

    return (ret == 0) ? ELOS_LOG_STATUS_SUCCESS : ELOS_LOG_STATUS_ERROR;
}

void elosLog(elosEventMessageCodeE_t messageCode, elosSeverityE_t severity, uint64_t classification,
             const char *logMessage) {
    elosEvent_t *logEvent = NULL;
    elosLogData_t *logData = NULL;
    elosLogStatusE_t result = ELOS_LOG_STATUS_SUCCESS;

    elosLogCreateLogData(messageCode, severity, classification, logMessage, &logData);

    result = elosLogCreateElosEventFromLog(logData, &logEvent);
    if (result == ELOS_LOG_STATUS_SUCCESS && logEvent != NULL) {
        result = elosLogPublishServerLogEvent(elosLogContext.logEventProcessor, logEvent);
        if (result == ELOS_LOG_STATUS_ERROR) {
            elosLogSafuFallback(logData);
        }
        elosEventDelete(logEvent);
    } else {
        elosLogSafuFallback(logData);
    }
    elosLogDeleteLogData(logData);
}

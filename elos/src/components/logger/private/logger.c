// SPDX-License-Identifier: MIT
#include "elos/logger/logger.h"

#include <bits/pthreadtypes.h>
#include <stdio.h>

static elosLogger_t elosDefaultLogger;
static pthread_once_t elosDefaultLoggerInitialized = PTHREAD_ONCE_INIT;

safuResultE_t elosLoggerInitialize(elosLogger_t *logger) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    elosEventBufferParam_t bParam = {.limitEventCount = ELOS_EVENTBUFFER_DEFAULT_LIMIT};

    result = elosEventBufferNew(&logger->logEventBuffer, &bParam);

    if (result == SAFU_RESULT_OK) {
        atomic_store(&logger->flags, SAFU_FLAG_INITIALIZED_BIT);
    }

    return result;
}

safuResultE_t elosLoggerDeleteMembers(elosLogger_t *logger) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (logger != NULL) {
        if (SAFU_FLAG_HAS_INITIALIZED_BIT(&logger->flags) == true) {
            result = elosEventBufferDelete(&logger->logEventBuffer);

            if (result == SAFU_RESULT_OK) {
                atomic_store(&logger->flags, SAFU_FLAG_ERROR_BIT);
            }
        }
    }

    return result;
}

static void _initDefaultLogger() {
    elosLoggerInitialize(&elosDefaultLogger);
}

safuResultE_t elosLoggerGetDefaultLogger(elosLogger_t **logger) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    int retVal = 0;

    retVal = pthread_once(&elosDefaultLoggerInitialized, _initDefaultLogger);

    if (retVal == 0) {
        *logger = &elosDefaultLogger;
        result = SAFU_RESULT_OK;
    }

    return result;
}

static safuResultE_t elosLogPublishServerLogEvent(elosEventBuffer_t *elosLogEventBuffer, elosEvent_t *elosLogEvent) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((elosLogEventBuffer != NULL) && (elosLogEvent != NULL)) {
        result = elosEventBufferWrite(elosLogEventBuffer, elosLogEvent);
    }

    return result;
}

void elosLog(elosEventMessageCodeE_t messageCode, elosSeverityE_t severity, uint64_t classification,
             const char *logMessage) {
    elosEvent_t logEvent = {0};
    elosLogger_t *logger = NULL;
    safuResultE_t result = SAFU_RESULT_FAILED;

    elosLoggerGetDefaultLogger(&logger);

    elosLogCreateElosEventFromLog(messageCode, severity, classification, logMessage, &logEvent);

    if (SAFU_FLAG_HAS_INITIALIZED_BIT(&logger->flags) == true) {
        result = elosLogPublishServerLogEvent(logger->logEventBuffer, &logEvent);
        if (result == SAFU_RESULT_FAILED) {
            elosLogSafuFallback(&logEvent);
        }
    }

    elosEventDeleteMembers(&logEvent);
}

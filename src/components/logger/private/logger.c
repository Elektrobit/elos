// SPDX-License-Identifier: MIT
#include "elos/logger/logger.h"

#include <bits/pthreadtypes.h>
#include <stdio.h>

static elosLogger_t elosDefaultLogger = {0};

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
        if (SAFU_FLAG_EXCHANGE_SHUTDOWN_BIT(&logger->flags) == false) {
            result = elosEventBufferDelete(&logger->logEventBuffer);
            if (result == SAFU_RESULT_OK) {
                logger->logEventBuffer = NULL;
                SAFU_FLAG_CLEAR_ALL_BITS(&logger->flags);
            }
        }
    }

    return result;
}

safuResultE_t elosLoggerGetDefaultLogger(elosLogger_t **logger) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (SAFU_FLAG_EXCHANGE_INITIALIZE_BIT(&elosDefaultLogger.flags) == false) {
        result = elosLoggerInitialize(&elosDefaultLogger);
    }

    if (result == SAFU_RESULT_OK) {
        *logger = &elosDefaultLogger;
    }

    return result;
}

void elosLog(elosEventMessageCodeE_t messageCode, elosSeverityE_t severity, uint64_t classification,
             const char *logMessage) {
    elosEvent_t logEvent = {0};
    elosLogger_t *logger = NULL;
    safuResultE_t result = SAFU_RESULT_FAILED;

    elosLogCreateElosEventFromLog(messageCode, severity, classification, logMessage, &logEvent);

    elosLoggerGetDefaultLogger(&logger);
    if (SAFU_FLAG_HAS_INITIALIZED_BIT(&logger->flags) == true) {
        result = elosEventBufferWrite(logger->logEventBuffer, &logEvent);
        if (result == SAFU_RESULT_FAILED) {
            elosLogSafuFallback(&logEvent);
        }
    }

    elosEventDeleteMembers(&logEvent);
}

// SPDX-License-Identifier: MIT

#include <elos/libelos/libelos.h>
#include <stdlib.h>

#include "elos/eloslog/eloslog.h"
#include "elos/libelos/libeloslog.h"

static elosLogger_t *elosClientlogger = NULL;

safuResultE_t elosInitLogger(elosSession_t *session) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosLogger_t *newlogger = safuAllocMem(NULL, sizeof(elosLogger_t));

    if (session != NULL) {
        newlogger->session = session;

        elosClientlogger = newlogger;
        result = SAFU_RESULT_OK;
    }

    return result;
}

void elosDeleteLogger() {
    free(elosClientlogger);
}

static safuResultE_t elosLogPublishClientLogEvent(elosEvent_t *elosLogEvent) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((elosClientlogger != NULL) && (elosClientlogger->session != NULL)) {
        if (elosClientlogger->session->connected == true) {
            if (elosLogEvent != NULL) {
                result = elosEventPublish(elosClientlogger->session, elosLogEvent);
            }
        }
    }

    return result;
}

void elosLog(elosEventMessageCodeE_t messageCode, elosSeverityE_t severity, uint64_t classification,
             const char *logMessage) {
    elosEvent_t logEvent = {0};
    safuResultE_t result = SAFU_RESULT_FAILED;

    elosLogCreateElosEventFromLog(messageCode, severity, classification, logMessage, &logEvent);
    result = elosLogPublishClientLogEvent(&logEvent);
    if (result == SAFU_RESULT_FAILED) {
        elosLogSafuFallback(&logEvent);
    }
    elosEventDeleteMembers(&logEvent);
}

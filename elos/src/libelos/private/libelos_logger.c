// SPDX-License-Identifier: MIT

#include <elos/libelos/libelos.h>

#include "elos/eloslog/eloslog.h"
#include "elos/libelos/libeloslog.h"

static safuResultE_t elosLogPublishClientLogEvent(elosEvent_t *elosLogEvent) {
    elosSession_t *session;
    safuResultE_t result = SAFU_RESULT_FAILED;

    result = elosConnectTcpip("127.0.0.1", 54321, &session);

    if (result == SAFU_RESULT_OK) {
        if (elosLogEvent != NULL) {
            result = elosEventPublish(session, elosLogEvent);
            if (result == SAFU_RESULT_OK) {
                result = elosDisconnect(session);
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

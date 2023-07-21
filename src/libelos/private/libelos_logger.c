// SPDX-License-Identifier: MIT

#include <elos/libelos/libelos.h>

#include "elos/eloslog/eloslog.h"
#include "elos/eloslog/types.h"
#include "elos/libelos/libeloslog.h"

static elosLogStatusE_t elosLogPublishClientLogEvent(elosEvent_t *elosLogEvent) {
    elosSession_t *session;
    elosLogStatusE_t result = ELOS_LOG_STATUS_ERROR;
    int retVal = 0;

    retVal = elosConnectTcpip("127.0.0.1", 54321, &session);

    if (retVal == 0) {
        if (elosLogEvent != NULL) {
            retVal = elosEventPublish(session, elosLogEvent);
            if (retVal == 0) {
                retVal = elosDisconnect(session);
                if (retVal == 0) {
                    result = ELOS_LOG_STATUS_SUCCESS;
                }
            }
        }
    }

    return result;
}

void elosLog(elosEventMessageCodeE_t messageCode, elosSeverityE_t severity, uint64_t classification,
             const char *logMessage) {
    elosEvent_t *logEvent = NULL;
    elosLogData_t *logData = NULL;
    elosLogStatusE_t result = ELOS_LOG_STATUS_SUCCESS;

    elosLogCreateLogData(messageCode, severity, classification, logMessage, &logData);

    result = elosLogCreateElosEventFromLog(logData, &logEvent);
    if (result == ELOS_LOG_STATUS_SUCCESS && logEvent != NULL) {
        result = elosLogPublishClientLogEvent(logEvent);
        if (result == ELOS_LOG_STATUS_ERROR) {
            elosLogSafuFallback(logData);
        }
        elosEventDelete(logEvent);
    } else {
        elosLogSafuFallback(logData);
    }
    elosLogDeleteLogData(logData);
}

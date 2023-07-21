// SPDX-License-Identifier: MIT
#include "elos/eloslog/eloslog.h"

#include <linux/limits.h>
#include <safu/common.h>
#include <safu/log.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "elos/event/event.h"

void elosLogCreateLogData(elosEventMessageCodeE_t messageCode, elosSeverityE_t severity, uint64_t classification,
                          const char *logMessage, elosLogData_t **logdata) {
    elosLogData_t *datalog = NULL;

    datalog = safuAllocMem(NULL, sizeof(elosLogData_t));

    datalog->messageCode = messageCode;
    datalog->severity = severity;
    datalog->classification = classification;
    datalog->payload = strdup(logMessage);

    *logdata = datalog;
}

void elosLogDeleteLogData(elosLogData_t *logData) {
    free(logData->payload);
    free(logData);
}

elosLogStatusE_t elosLogCreateElosEventFromLog(elosLogData_t *logData, elosEvent_t **event) {
    elosEvent_t *logEvent = NULL;
    char *executableName = NULL;
    char *hostName = NULL;
    int retVal = 0;
    elosLogStatusE_t result = ELOS_LOG_STATUS_ERROR;

    if ((logData != NULL) && (event != NULL)) {
        retVal = elosEventNew(&logEvent);

        if (retVal == 0) {
            retVal = clock_gettime(CLOCK_REALTIME, &logEvent->date);

            logEvent->source.pid = getpid();

            executableName = safuAllocMem(NULL, PATH_MAX);

            retVal = readlink("/proc/self/exe", executableName, PATH_MAX);

            if (retVal < 0) {
                logEvent->source.appName = NULL;
                logEvent->source.fileName = NULL;
            } else {
                executableName[retVal] = 0;
                logEvent->source.appName = executableName;
                logEvent->source.fileName = strdup(strrchr(executableName, '/'));
            }

            logEvent->severity = logData->severity;

            hostName = safuAllocMem(NULL, PATH_MAX);
            retVal = gethostname(hostName, PATH_MAX);
            if (retVal < 0) {
                logEvent->hardwareid = NULL;
            } else {
                /* for the sake of simplicity we use the hardware id instead
                   of /etc/machineid */
                logEvent->hardwareid = hostName;
            }

            logEvent->classification = logData->classification;
            logEvent->messageCode = logData->messageCode;
            logEvent->payload = strdup(logData->payload);
            *event = logEvent;
            result = ELOS_LOG_STATUS_SUCCESS;
        }
    }

    return result;
}

void elosLogSafuFallback(elosLogData_t *logData) {
    switch (logData->severity) {
        case ELOS_SEVERITY_FATAL:
            safuLogCrit(logData->payload);
        case ELOS_SEVERITY_ERROR:
            safuLogErr(logData->payload);
        case ELOS_SEVERITY_WARN:
            safuLogWarn(logData->payload);
        case ELOS_SEVERITY_INFO:
            safuLogInfo(logData->payload);
        case ELOS_SEVERITY_DEBUG:
            safuLogDebug(logData->payload);
        default:
            safuLogDebug(logData->payload);
    }
}

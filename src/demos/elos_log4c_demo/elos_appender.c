// SPDX-License-Identifier: MIT

#include <linux/limits.h>
#include <log4c.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>

#include "elos/event/event.h"
#include "elos/libelos/libelos.h"
#include "safu/common.h"

#define UNUSED __attribute__((unused))

static int elosCreateLog4cEvent(const log4c_logging_event_t *aEvent, elosEvent_t **event) {
    elosEvent_t *log4cEvent = NULL;
    char *executableName = NULL;
    char *hostName = NULL;
    int retVal = 0;
    int result = 0;

    if ((aEvent == NULL) || (event == NULL)) {
        fprintf(stderr, "Invalid Parameter");
    } else {
        retVal = elosEventNew(&log4cEvent);

        if (retVal < 0) {
            fprintf(stderr, "event memory allocation failed");
        } else {
            retVal = clock_gettime(CLOCK_REALTIME, &log4cEvent->date);
            if (retVal < 0) {
                fprintf(stderr, "clock_gettime failed");
            }

            log4cEvent->source.pid = getpid();

            executableName = safuAllocMem(NULL, PATH_MAX);

            retVal = readlink("/proc/self/exe", executableName, PATH_MAX);

            if (retVal < 0) {
                fprintf(stderr, "process name not found");
                log4cEvent->source.appName = NULL;
                log4cEvent->source.fileName = NULL;
            } else {
                executableName[retVal] = 0;
                log4cEvent->source.appName = executableName;
                log4cEvent->source.fileName = strdup(strrchr(executableName, '/'));
            }

            log4cEvent->severity = aEvent->evt_priority;

            hostName = safuAllocMem(NULL, PATH_MAX);
            retVal = gethostname(hostName, PATH_MAX);
            if (retVal < 0) {
                fprintf(stderr, "host name not found");
                log4cEvent->hardwareid = NULL;
            } else {
                log4cEvent->hardwareid =
                    hostName;  // for the sake of simplicity we use the hardware id instead of /etc/machineid
            }

            log4cEvent->classification = ELOS_CLASSIFICATION_PROCESS_ERRORS;
            log4cEvent->messageCode = ELOS_MSG_CODE_DEBUG_LOG;
            log4cEvent->payload = strdup(aEvent->evt_msg);
            *event = log4cEvent;
        }
    }

    if (log4cEvent == NULL) {
        result = -1;
    }

    return result;
}

static int elosPublishLog4cEvent(elosEvent_t *elosLog4cEvent) {
    elosSession_t *session;
    int retVal = 0;

    fprintf(stdout, "connecting elos appender to event log scanner...\n");
    retVal = elosConnectTcpip("127.0.0.1", 54321, &session);

    if (retVal < 0) {
        fprintf(stderr, "elos error can not connect to elos and failed!\n");
    }

    if (retVal == 0) {
        if (elosLog4cEvent == NULL) {
            fprintf(stderr, "Invalid Parameter");
        } else {
            retVal = elosEventPublish(session, elosLog4cEvent);
            if (retVal < 0) {
                fprintf(stderr, "elos appender push event failed!\n");
            }

            if (retVal == 0) {
                fprintf(stdout, "disconnecting elos appender from event log scanner...\n");
                retVal = elosDisconnect(session);
                if (retVal < 0) {
                    fprintf(stderr, "elos appender disconnect failed!\n");
                }
            }

            fprintf(stdout, "elos appender done\n");
        }
    }

    return retVal;
}

static int elosAppend(UNUSED log4c_appender_t *this, const log4c_logging_event_t *aEvent) {
    elosEvent_t *logEvent = NULL;
    int result = 0;

    result = elosCreateLog4cEvent(aEvent, &logEvent);
    if (result == 0 && logEvent != NULL) {
        result = elosPublishLog4cEvent(logEvent);
        if (result < 0) {
            fprintf(stderr, "publishing log4c message to elos failed\n");
        } else {
            fprintf(stderr, "%s", aEvent->evt_rendered_msg);
        }
        elosEventDelete(logEvent);
    } else {
        fprintf(stderr, "%s", aEvent->evt_rendered_msg);
    }

    return result;
}

static int elosOpen(UNUSED log4c_appender_t *this) {
    return 0;
}

static int elosClose(UNUSED log4c_appender_t *this) {
    return 0;
}

const log4c_appender_type_t elosLog4cAppenderTypeElosAppender = {
    "elos_appender",
    elosOpen,
    elosAppend,
    elosClose,
};

static const log4c_appender_type_t *const elosAppenderTypes[] = {
    &elosLog4cAppenderTypeElosAppender,
};

int elosNappenderTypes = (int)(sizeof(elosAppenderTypes) / sizeof(elosAppenderTypes[0]));

int elosInitElosAppender() {
    int rc = 0;
    int i = 0;

    for (i = 0; i < elosNappenderTypes; i++) {
        log4c_appender_type_set(elosAppenderTypes[i]);
    }

    return (rc);
}

// SPDX-License-Identifier: MIT
#include "elos/eloslog/eloslog.h"

#include <linux/limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

static void _fillEventSource(elosEvent_t *event) {
    ssize_t fileNameSize = 0;

    event->source.pid = getpid();
    event->source.appName = NULL;
    event->source.fileName = safuAllocMem(NULL, PATH_MAX);

    if (event->source.fileName != NULL) {
        fileNameSize = readlink("/proc/self/exe", event->source.fileName, PATH_MAX);

        if (fileNameSize > 0) {
            event->source.fileName[fileNameSize] = 0;
            event->source.appName = strdup(strrchr(event->source.fileName, '/'));
            event->source.fileName = safuAllocMem(event->source.fileName, fileNameSize + sizeof('\0'));
        }
    }
}

static void _fillEventTime(elosEvent_t *event) {
    int retVal = 0;

    retVal = clock_gettime(CLOCK_REALTIME, &event->date);

    if (retVal != 0) {
        event->date.tv_sec = 0;
        event->date.tv_nsec = 0;
    }
}

void elosLogCreateElosEventFromLog(elosEventMessageCodeE_t messageCode, elosSeverityE_t severity,
                                   uint64_t classification, const char *logMessage, elosEvent_t *event) {
    if (event != NULL) {
        event->severity = severity;
        event->classification = classification;
        event->messageCode = messageCode;

        _fillEventTime(event);

        _fillEventSource(event);

        event->hardwareid = strdup(safuGetHardwareId());

        event->payload = NULL;
        if (logMessage != NULL) {
            event->payload = strdup(logMessage);
        }
    }
}

void elosLogSafuFallback(elosEvent_t *event) {
    switch (event->severity) {
        case ELOS_SEVERITY_FATAL:
            safuLogCrit(event->payload);
        case ELOS_SEVERITY_ERROR:
            safuLogErr(event->payload);
        case ELOS_SEVERITY_WARN:
            safuLogWarn(event->payload);
        case ELOS_SEVERITY_INFO:
            safuLogInfo(event->payload);
        case ELOS_SEVERITY_DEBUG:
            safuLogDebug(event->payload);
        default:
            safuLogDebug(event->payload);
    }
}

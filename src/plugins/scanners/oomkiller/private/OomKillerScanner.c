// SPDX-License-Identifier: MIT

#include "OomKillerScanner.h"

#include <errno.h>
#include <fcntl.h>
#include <safu/log.h>
#include <safu/time.h>
#include <safu/vector.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "elos/event/event.h"
#include "elos/event/event_types.h"
#include "elos/event/event_vector.h"

#define PID_OFFSET 14

typedef struct _matchedEventData {
    elosPlugin_t *plugin;
    struct elosPublisher *publisher;
    safuResultE_t result;
} _matchedEventData_t;

atomic_bool elosPluginIsRunning = ATOMIC_VAR_INIT(false);

static safuResultE_t _create_oom_subscriber(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosOomKillerScanner_t *scanner = (elosOomKillerScanner_t *)plugin->data;

    result = elosPluginCreateSubscriber(plugin, &scanner->oomEventSubscriber);

    if (result == SAFU_RESULT_FAILED) {
        safuLogErr("elosPluginCreateSubscriber failed");
    }

    return result;
}

static safuResultE_t _create_oom_publisher(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosOomKillerScanner_t *scanner = (elosOomKillerScanner_t *)plugin->data;

    result = elosPluginCreatePublisher(plugin, &scanner->oomEventPublisher);

    if (result == SAFU_RESULT_FAILED) {
        safuLogErr("elosPluginCreatePublisher failed");
    }

    return result;
}

static safuResultE_t _create_oom_subscription(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosOomKillerScanner_t *scanner = (elosOomKillerScanner_t *)plugin->data;
    const char *subscriptionFilters[] = {
        ".event.source.fileName '/dev/kmsg' STRCMP .event.messageCode 1111 EQ AND .event.payload r'.*Out of memory: "
        "Killed process*' REGEX AND"};

    result = elosPluginSubscribe(plugin, scanner->oomEventSubscriber, subscriptionFilters,
                                 ARRAY_SIZE(subscriptionFilters), &scanner->oomEventSubscription);

    if (result == SAFU_RESULT_FAILED) {
        safuLogErr("elosPluginSubscribe failed");
    }

    return result;
}

static void _parse_matched_event_payload(const char *payload, char **process, pid_t *pid) {
    char *killedProcessStr = NULL;
    char *processName = NULL;
    char *processStringEnd = NULL;

    killedProcessStr = strstr(payload, "Killed process ");

    if (killedProcessStr == NULL) {
        safuLogErr("payload does not contain string \"Killed process\"");
        *pid = 0;
        processName = "";
    } else {
        char *endPtr = NULL;
        errno = 0;
        *pid = strtol(killedProcessStr + PID_OFFSET, &endPtr, 10);
        if (errno != 0) {
            safuLogWarn("pid conversion error, pid set to 0");
            *pid = 0;
        }

        if (endPtr == killedProcessStr + PID_OFFSET) {
            safuLogWarn("pid not found, pid set to 0");
            *pid = 0;
        }

        processName = strstr(killedProcessStr, " (");
        if (processName == NULL) {
            safuLogWarn("Terminated process name not found, no process name set");
            processName = "";
        } else {
            processName += 2;
            processStringEnd = strstr(processName, ")");
            if (processStringEnd == NULL) {
                safuLogWarn("Kernel log format unknown, no process name set");
                processName = "";
            } else {
                *processStringEnd = '\0';
            }
        }
    }

    *process = strdup(processName);
}

static inline void _create_oom_killer_invoked_event(elosEvent_t *matchedEvent, elosEvent_t *oomkillerEvent) {
    char *processName = NULL;
    pid_t processPID = 0;

    oomkillerEvent->date.tv_sec = matchedEvent->date.tv_sec;
    oomkillerEvent->date.tv_nsec = matchedEvent->date.tv_nsec;

    _parse_matched_event_payload(matchedEvent->payload, &processName, &processPID);

    oomkillerEvent->source.appName = processName;
    oomkillerEvent->source.fileName = NULL;
    oomkillerEvent->source.pid = processPID;

    oomkillerEvent->severity = ELOS_SEVERITY_FATAL;
    oomkillerEvent->hardwareid = strdup(safuGetHardwareId());
    oomkillerEvent->classification =
        ELOS_CLASSIFICATION_KERNEL | ELOS_CLASSIFICATION_STORAGE | ELOS_CLASSIFICATION_PROCESS_ERRORS;
    oomkillerEvent->messageCode = ELOS_MSG_CODE_OOM_KILLER_INVOKED;
    oomkillerEvent->payload = strdup(matchedEvent->payload);
}

static int _foreachEvent(void const *element, void const *data) {
    elosEvent_t oomkillerEvent = {0};
    elosEvent_t *matchedEvent = (elosEvent_t *)element;
    _matchedEventData_t *helperData = (_matchedEventData_t *)data;

    _create_oom_killer_invoked_event(matchedEvent, &oomkillerEvent);

    helperData->result = elosPluginPublish(helperData->plugin, helperData->publisher, &oomkillerEvent);
    if (helperData->result != SAFU_RESULT_OK) {
        safuLogErr("elosPluginPublish failed");
    }

    safuResultE_t result = elosEventDeleteMembers(&oomkillerEvent);
    if (result != SAFU_RESULT_OK) {
        safuLogErr("elosEventDeleteMembers failed");
        helperData->result = result;
    }
    return 0;
}

safuResultE_t elosOomKillerScannerNew(elosOomKillerScanner_t **oomKillerScanner, elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosOomKillerScanner_t *newScanner = NULL;

    newScanner = safuAllocMem(NULL, sizeof(elosOomKillerScanner_t));

    if (newScanner == NULL) {
        safuLogErr("safuAllocMem failed");
    } else {
        *oomKillerScanner = newScanner;
        result = _create_oom_subscriber(plugin);
        if (result == SAFU_RESULT_OK) {
            result = _create_oom_publisher(plugin);
            if (result == SAFU_RESULT_OK) {
                result = _create_oom_subscription(plugin);
            }
        }
    }

    return result;
}

safuResultE_t elosOomKillerScannerStart(elosOomKillerScanner_t *oomKillerScanner, elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosEventVector_t *eventVector = NULL;
    atomic_store(&elosPluginIsRunning, true);
    _matchedEventData_t helperData = {
        .plugin = plugin, .publisher = oomKillerScanner->oomEventPublisher, .result = result};

    while (atomic_load(&elosPluginIsRunning)) {
        result = elosPluginReadQueue(plugin, oomKillerScanner->oomEventSubscriber,
                                     oomKillerScanner->oomEventSubscription, &eventVector);
        if (result == SAFU_RESULT_OK && eventVector != NULL) {
            safuVecIterate(eventVector, _foreachEvent, &helperData);
            if (helperData.result == SAFU_RESULT_OK) {
                result = helperData.result;
            } else {
                result = helperData.result;
                atomic_store(&elosPluginIsRunning, false);
            }
        } else if (result != SAFU_RESULT_OK) {
            safuLogErr("elosPluginReadQueue failed");
        }

        elosEventVectorDelete(eventVector);
        eventVector = NULL;
        usleep(10000);
    }

    return result;
}

safuResultE_t elosOomKillerScannerDelete(elosOomKillerScanner_t *oomKillerScanner, elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    bool failed = false;

    if (oomKillerScanner == NULL) {
        safuLogErr("Invalid parameters to OomKillerScannerDelete");
    } else {
        result = elosPluginDeletePublisher(plugin, oomKillerScanner->oomEventPublisher);
        if (result == SAFU_RESULT_FAILED) {
            failed = true;
            safuLogErr("elosPluginDeletePublisher failed");
        }
        result = elosPluginUnsubscribeAll(plugin, oomKillerScanner->oomEventSubscriber);
        if (result == SAFU_RESULT_FAILED) {
            failed = true;
            safuLogErr("Invalid parameters to OomKillerScannerShutdown");
        }

        result = elosPluginDeleteSubscriber(plugin, oomKillerScanner->oomEventSubscriber);
        if (result == SAFU_RESULT_FAILED) {
            failed = true;
            safuLogErr("Invalid parameters to OomKillerScannerShutdown");
        }

        free(oomKillerScanner);
    }

    return failed ? SAFU_RESULT_FAILED : SAFU_RESULT_OK;
}

safuResultE_t elosOomKillerScannerStop(elosOomKillerScanner_t *oomKillerScanner) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (oomKillerScanner == NULL) {
        safuLogErr("Invalid parameters to OomKillerScannerStop");
    } else {
        atomic_store(&elosPluginIsRunning, false);
        result = SAFU_RESULT_OK;
    }

    return result;
}

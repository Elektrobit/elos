// SPDX-License-Identifier: MIT
#include <fcntl.h>
#include <safu/vector.h>
#include <safu/time.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "elos/event/event.h"
#include "OomKillerScanner.h"

typedef struct _matchedEventData {
    elosPlugin_t *plugin;
    struct elosPublisher *publisher;
    safuResultE_t result;
} _matchedEventData_t;

atomic_bool isRunning = ATOMIC_VAR_INIT(false);

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
        ".event.messageCode 1111 EQ .event.payload r'.*Out of memory: Killed process*' REGEX AND"};

    result = elosPluginSubscribe(plugin, scanner->oomEventSubscriber, subscriptionFilters, ARRAY_SIZE(subscriptionFilters),
                                 &scanner->oomEventSubscription);

    if (result == SAFU_RESULT_FAILED) {
        safuLogErr("elosPluginSubscribe failed");
    }

    return result;
}

static void _parse_matched_event_payload(const char *payload, char **process, pid_t *pid) {
    char *killed_prcess_str = NULL;
    char *process_name = NULL;
    char *process_string_end = NULL;

    killed_prcess_str = strstr(payload, "Killed process ");

    if (killed_prcess_str == NULL) {
        safuLogErr("payload does not contain string \"Killed process\"");
        *pid = 0;
        process_name = "";
    } else {
        *pid = (pid_t)atoi(killed_prcess_str + 14);
        if (*pid == 0) {
            safuLogWarn("pid conversion error, pid set to 0");
        }

        process_name = strstr(killed_prcess_str, " (");
        if (process_name == NULL) {
            safuLogWarn("Terminated process name not found, no process name set");
            process_name = "";
        } else {
            process_name += 2;
            process_string_end = strstr(process_name, ")");
            if (process_string_end == NULL) {
                safuLogWarn("Kernel log format unknown, no process name set");
                process_name = "";
            } else {
                *process_string_end = '\0';
            }
        }
    }

    *process = strdup(process_name);
}

static inline void _create_oom_killer_invoked_event(elosEvent_t *matchedEvent, elosEvent_t *oomkillerEvent) {
    char *processName = NULL;
    pid_t processPID = 0;

    oomkillerEvent->date.tv_sec = matchedEvent->date.tv_sec;
    oomkillerEvent->date.tv_nsec = matchedEvent->date.tv_nsec;
   
    _parse_matched_event_payload(matchedEvent->payload, &processName, &processPID);

    oomkillerEvent->source.appName = processName;
    oomkillerEvent->source.fileName = "";
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

    safuResultE_t result = elosEventDeleteMembers(matchedEvent);
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
    safuVec_t *eventVector = NULL;
    atomic_store(&isRunning, true);
    _matchedEventData_t helperData = {
        .plugin = plugin, .publisher = oomKillerScanner->oomEventPublisher, .result = result};

    while (atomic_load(&isRunning)) {
        result = elosPluginReadQueue(plugin, oomKillerScanner->oomEventSubscriber,
                                     oomKillerScanner->oomEventSubscription, &eventVector);
        if (result == SAFU_RESULT_OK && eventVector != NULL) {
            safuVecIterate(eventVector, _foreachEvent, &helperData);
            if (helperData.result == SAFU_RESULT_OK) {
                result = helperData.result;
                usleep(10000);
            } else {
                result = helperData.result;
                atomic_store(&isRunning, false);
            }
            int ret = safuVecFree(eventVector);
            if (ret != 0) {
                safuLogWarn("safuVecFree failed, potential memory leak");
            }
            eventVector = NULL;
        } else if (result == SAFU_RESULT_OK && eventVector == NULL) {
            continue;
        } else {
            safuLogErr("elosPluginReadQueue failed, attempting again");
        }
    }
    return result;
}

safuResultE_t elosOomKillerScannerStop(elosOomKillerScanner_t *oomKillerScanner, elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    bool failed = false;
    if (oomKillerScanner == NULL || plugin == NULL) {
        safuLogErr("Invalid parameters to OomKillerScannerShutdown");
    } else {
        atomic_store(&isRunning, false);
        result = elosPluginDeletePublisher(plugin, oomKillerScanner->oomEventPublisher);
        if (result == SAFU_RESULT_FAILED) {
            failed = true;
        }
        result = elosPluginUnsubscribeAll(plugin, oomKillerScanner->oomEventSubscriber);
        if (result == SAFU_RESULT_FAILED) {
            failed = true;
        }
        free((elosSubscription_t *)oomKillerScanner->oomEventSubscription);
        result = elosPluginDeleteSubscriber(plugin, oomKillerScanner->oomEventSubscriber);
        if (result == SAFU_RESULT_FAILED) {
            failed = true;
        }
        if (!failed) {
            result = SAFU_RESULT_OK;
        }
    }

    free(oomKillerScanner);
    return result;
}

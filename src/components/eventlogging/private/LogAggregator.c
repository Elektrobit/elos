// SPDX-License-Identifier: MIT
#include "elos/eventlogging/LogAggregator.h"

#include <pthread.h>
#include <safu/common.h>
#include <safu/result.h>
#include <safu/vector.h>

#include "elos/config/config.h"
#include "elos/eventfilter/eventfilter.h"
#include "elos/eventfilter/vector.h"
#include "elos/libelosplugin/StorageBackend_types.h"
#include "elos/storagemanager/StorageBackend.h"
#include "safu/log.h"
#include "safu/mutex.h"

static pthread_mutex_t elosLogAggregatorMutex = PTHREAD_MUTEX_INITIALIZER;

#define FETCH_API_BACKEND_NAME "fetchapi"

static int _findFetchApiPlugin(void const *element, UNUSED void const *data) {
    int retVal = 0;
    if ((element == NULL) || (*(elosStorageBackend_t **)element == NULL)) {
        safuLogErr("StorageBackend is not configured correctly");
        retVal = -1;
    } else {
        elosStorageBackend_t *backend = *(elosStorageBackend_t **)element;
        if (strcmp(backend->name, FETCH_API_BACKEND_NAME) == 0) {
            retVal = 1;
        }
    }
    return retVal;
}

safuResultE_t elosLogAggregatorStart(elosLogAggregator_t *logAggregator, elosLogAggregatorParam_t const *param) {
    safuResultE_t result = SAFU_RESULT_OK;

    if ((logAggregator == NULL) || (param == NULL)) {
        safuLogErr("Called elosLogAggregatorStart with NULL-parameter");
        result = SAFU_RESULT_FAILED;
    } else if ((param->config == NULL) || (param->backends == NULL)) {
        safuLogErr("Parameter struct passed to elosLogAggregatorStart contains NULL-pointers");
        result = SAFU_RESULT_FAILED;
    } else {
        SAFU_PTHREAD_MUTEX_LOCK(&elosLogAggregatorMutex, result = SAFU_RESULT_FAILED);
        if (result == SAFU_RESULT_OK) {
            logAggregator->lock = &elosLogAggregatorMutex;
            logAggregator->backends = param->backends;
            logAggregator->fetchapiBackendPluginIndex = UINT32_MAX;
            int retVal = safuVecFind(logAggregator->backends, 0, _findFetchApiPlugin, NULL,
                                     &logAggregator->fetchapiBackendPluginIndex);
            switch (retVal) {
                case 0:
                    safuLogDebug("no fetchapi plugin configured");
                    break;
                case -1:
                    safuLogErr("Error while finding fetchapi plugin");
                    result = SAFU_RESULT_FAILED;
            }
            SAFU_PTHREAD_MUTEX_UNLOCK(&elosLogAggregatorMutex, result = SAFU_RESULT_FAILED);
        }
    }

    return result;
}

safuResultE_t elosLogAggregatorShutdown(elosLogAggregator_t *logAggregator) {
    safuResultE_t result = SAFU_RESULT_OK;

    if ((logAggregator != NULL) && (logAggregator->lock != NULL)) {
        SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT(logAggregator->lock, result);
        if (result == SAFU_RESULT_OK) {
            logAggregator->backends = NULL;
            logAggregator->fetchapiBackendPluginIndex = UINT32_MAX;
            SAFU_PTHREAD_MUTEX_UNLOCK(logAggregator->lock, result = SAFU_RESULT_FAILED);
        }
    }

    return result;
}

typedef struct _addHelperData {
    elosEvent_t const *event;
    safuResultE_t result;
} _addHelperData_t;

static int _logAggregatorAddHelper(void const *element, void const *data) {
    _addHelperData_t *helperData = (_addHelperData_t *)data;
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((element == NULL) || (*(elosStorageBackend_t **)element == NULL) || (data == NULL)) {
        safuLogErr("StorageBackend is not configured correctly");
    } else {
        elosStorageBackend_t *backend = *(elosStorageBackend_t **)element;
        result = elosStorageBackendAccepts(backend, helperData->event);
        if (result == SAFU_RESULT_OK) {
            result = backend->persist(backend, helperData->event);
            if (result != SAFU_RESULT_OK) {
                safuLogErrF("Persist event in backend: %s failed", backend->name);
            }
        } else if (result == SAFU_RESULT_NOT_FOUND) {
            result = SAFU_RESULT_OK;
        }
    }

    if (result != SAFU_RESULT_OK) {
        helperData->result = result;
    }

    return 0;
}

safuResultE_t elosLogAggregatorAdd(elosLogAggregator_t *logAggregator, const elosEvent_t *event) {
    safuResultE_t result = SAFU_RESULT_OK;

    if ((logAggregator == NULL) || (event == NULL)) {
        safuLogErr("Called elosLogAggregatorAdd with NULL-parameter");
        result = SAFU_RESULT_FAILED;
    } else {
        SAFU_PTHREAD_MUTEX_LOCK(logAggregator->lock, result = SAFU_RESULT_FAILED);
        if (result == SAFU_RESULT_OK) {
            _addHelperData_t helperData = {.event = event, result = SAFU_RESULT_OK};
            int retVal;

            retVal = safuVecIterate(logAggregator->backends, _logAggregatorAddHelper, &helperData);
            if (retVal < 0) {
                safuLogErr("Iterating through the backends failed");
                result = SAFU_RESULT_FAILED;
            } else if (helperData.result != SAFU_RESULT_OK) {
                safuLogWarn("Errors happened in at least one backend");
                result = SAFU_RESULT_FAILED;
            }

            SAFU_PTHREAD_MUTEX_UNLOCK(logAggregator->lock, result = SAFU_RESULT_FAILED);
        }
    }

    return result;
}

safuResultE_t elosLogAggregatorFindEvents(elosLogAggregator_t *logAggregator, const char *rule,
                                          struct timespec const *newest, struct timespec const *oldest,
                                          safuVec_t *events) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    safuLogDebugF("received filterRule: %s", rule);

    if (logAggregator == NULL || rule == NULL || events == NULL) {
        safuLogErr("Called elosLogAggregatorFindEvents with NULL-parameter");
    } else {
        uint32_t index = logAggregator->fetchapiBackendPluginIndex;
        if (index == UINT32_MAX) {
            result = SAFU_RESULT_OK;
            safuLogDebug("No fetchapi backend configured to get events from!");
        } else {
            elosEventFilterParam_t const param = {.filterString = rule};
            elosEventFilter_t filter = ELOS_EVENTFILTER_INIT;
            elosRpnFilterResultE_t errVal;

            errVal = elosEventFilterCreate(&filter, &param);
            if (errVal == RPNFILTER_RESULT_ERROR) {
                safuLogErrF("Failed to create filter for rule '%s'", rule);
            } else {
                result = SAFU_RESULT_OK;
                SAFU_PTHREAD_MUTEX_LOCK(logAggregator->lock, result = SAFU_RESULT_FAILED);
                if (result == SAFU_RESULT_OK) {
                    elosStorageBackend_t **fetch = safuVecGet(logAggregator->backends, index);
                    if (fetch == NULL || *fetch == NULL || (*fetch)->findEvent == NULL) {
                        safuLogErr("failed to get the fetchapi backend!");
                    } else {
                        elosStorageBackend_t *fetchapi = *fetch;
                        result = fetchapi->findEvent(fetchapi, &filter, newest, oldest, events);
                        if (result != SAFU_RESULT_OK) {
                            safuLogErr("Find events in fetchapi backend failed");
                        }
                    }
                    SAFU_PTHREAD_MUTEX_UNLOCK(logAggregator->lock, result = SAFU_RESULT_FAILED);
                }
            }
            elosEventFilterDeleteMembers(&filter);
        }
    }
    return result;
}

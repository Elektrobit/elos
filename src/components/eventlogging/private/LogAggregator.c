// SPDX-License-Identifier: MIT
#include "elos/eventlogging/LogAggregator.h"

#include <pthread.h>
#include <safu/common.h>
#include <safu/vector.h>

#include "elos/config/config.h"
#include "elos/eventfilter/eventfilter.h"
#include "elos/eventfilter/vector.h"
#include "elos/eventlogging/PluginFilterLoader.h"
#include "safu/log.h"
#include "safu/mutex.h"

static pthread_mutex_t elosLogAggregatorMutex = PTHREAD_MUTEX_INITIALIZER;

static int _loadFilter(void const *element, UNUSED void const *data) {
    elosPluginControl_t *plugin = *(elosPluginControl_t **)element;
    elosPluginFilterLoaderLoad(plugin);
    return 0;
}

safuResultE_t elosLogAggregatorStart(elosLogAggregator_t *logAggregator, elosLogAggregatorParam_t const *param) {
    safuResultE_t result = SAFU_RESULT_OK;

    if ((logAggregator == NULL) || (param == NULL)) {
        safuLogErr("Called elosLogAggregatorStart with NULL-parameter");
        result = SAFU_RESULT_FAILED;
    } else if ((param->config == NULL) || (param->pluginManager == NULL)) {
        safuLogErr("Parameter struct passed to elosLogAggregatorStart contains NULL-pointers");
        result = SAFU_RESULT_FAILED;
    } else {
        SAFU_PTHREAD_MUTEX_LOCK(&elosLogAggregatorMutex, result = SAFU_RESULT_FAILED);
        if (result == SAFU_RESULT_OK) {
            samconfConfigStatusE_t status;
            int retVal;

            logAggregator->lock = &elosLogAggregatorMutex;
            logAggregator->pluginManager = param->pluginManager;

            retVal = safuVecCreate(&logAggregator->pluginControlPtrVector, 1, sizeof(elosPluginControl_t *));
            if (retVal < 0) {
                safuLogErr("safuVecCreate failed");
            } else {
                status = samconfConfigGet(param->config, ELOS_CONFIG_EVENTLOGGING, &logAggregator->config);
                if (status != SAMCONF_CONFIG_OK) {
                    safuLogErr("Loading LogAggregator config failed");
                } else {
                    char const *searchPath;

                    searchPath = elosConfigGetElosdBackendPath(param->config);
                    if (searchPath == NULL) {
                        safuLogErr("elosConfigGetElosdBackendPath failed");
                        result = SAFU_RESULT_FAILED;
                    } else {
                        result = elosPluginManagerLoad(logAggregator->pluginManager, logAggregator->config, searchPath,
                                                       &logAggregator->pluginControlPtrVector);
                        if (result != SAFU_RESULT_OK) {
                            safuLogWarn("elosPluginManagerLoad executed with errors");
                        } else {
                            safuVecIterate(&logAggregator->pluginControlPtrVector, _loadFilter, NULL);
                        }
                    }
                }
            }

            SAFU_PTHREAD_MUTEX_UNLOCK(&elosLogAggregatorMutex, result = SAFU_RESULT_FAILED);
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
    elosPluginControl_t *plugin = *(elosPluginControl_t **)element;
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((plugin->context.state != PLUGIN_STATE_STARTED) || (plugin->context.data == NULL)) {
        safuLogErrF("Backend plugin->id: %d is not configured correctly", plugin->context.id);
    } else {
        elosStorageBackend_t *backend = (elosStorageBackend_t *)plugin->context.data;

        result = elosStorageBackendAccepts(backend, helperData->event);
        if (result == SAFU_RESULT_OK) {
            result = backend->persist(backend, helperData->event);
            if (result != SAFU_RESULT_OK) {
                safuLogErrF("Backend 'Persist' for plugin->id: %d failed", plugin->context.id);
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

            retVal = safuVecIterate(&logAggregator->pluginControlPtrVector, _logAggregatorAddHelper, &helperData);
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

static int _unloadFilter(void const *element, UNUSED void const *data) {
    elosPluginControl_t *plugin = *(elosPluginControl_t **)element;
    elosStorageBackend_t *backend = (elosStorageBackend_t *)plugin->context.data;
    elosEventFilterVectorDeleteMembers(&backend->filter);
    return 0;
}

safuResultE_t elosLogAggregatorShutdown(elosLogAggregator_t *logAggregator) {
    safuResultE_t result = SAFU_RESULT_OK;

    if ((logAggregator != NULL) && (logAggregator->lock != NULL)) {
        SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT(logAggregator->lock, result);
        if (result == SAFU_RESULT_OK) {
            int retVal;

            if (logAggregator->pluginManager != NULL) {
                safuResultE_t funcResult;

                safuVecIterate(&logAggregator->pluginControlPtrVector, _unloadFilter, NULL);

                funcResult =
                    elosPluginManagerUnload(logAggregator->pluginManager, &logAggregator->pluginControlPtrVector);
                if (funcResult != SAFU_RESULT_OK) {
                    result = funcResult;
                }

                retVal = safuVecFree(&logAggregator->pluginControlPtrVector);
                if (retVal < 0) {
                    result = SAFU_RESULT_FAILED;
                }
            }

            SAFU_PTHREAD_MUTEX_UNLOCK(logAggregator->lock, result = SAFU_RESULT_FAILED);
        }
    }

    return result;
}

typedef struct _findEventHelperData {
    elosEventFilter_t *filter;
    safuVec_t *eventVector;
    safuResultE_t result;
} _findEventHelperData_t;

static int _logAggregatorFindEventsHelper(void const *element, void const *data) {
    _findEventHelperData_t *helperData = (_findEventHelperData_t *)data;
    elosPluginControl_t *plugin = *(elosPluginControl_t **)element;
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((plugin->context.state != PLUGIN_STATE_STARTED) || (plugin->context.data == NULL)) {
        safuLogErrF("Backend plugin->id: %d is not configured correctly", plugin->context.id);
    } else {
        elosStorageBackend_t *backend = (elosStorageBackend_t *)plugin->context.data;

        result = backend->findEvent(backend, helperData->filter, helperData->eventVector);
        if (result != SAFU_RESULT_OK) {
            safuLogErrF("Backend 'FindEvent' for plugin->id: %d failed", plugin->context.id);
            helperData->result = result;
        }
    }

    if (result != SAFU_RESULT_OK) {
        helperData->result = result;
    }

    return 0;
}

safuResultE_t elosLogAggregatorFindEvents(elosLogAggregator_t *logAggregator, const char *rule, safuVec_t *events) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    safuLogDebugF("received filterRule: %s", rule);

    if (logAggregator == NULL || rule == NULL || events == NULL) {
        safuLogErr("Called elosLogAggregatorFindEvents with NULL-parameter");
    } else {
        elosEventFilterParam_t const param = {.filterString = rule};
        elosEventFilter_t filter = ELOS_EVENTFILTER_INIT;
        elosRpnFilterResultE_t errVal;

        errVal = elosEventFilterCreate(&filter, &param);
        if (errVal == RPNFILTER_RESULT_ERROR) {
            safuLogErrF("Failed to create filter for rule '%s'", rule);
        } else {
            int retVal;
            _findEventHelperData_t const helperData = {
                .filter = &filter,
                .eventVector = events,
                .result = SAFU_RESULT_OK,
            };

            result = SAFU_RESULT_OK;

            SAFU_PTHREAD_MUTEX_LOCK(logAggregator->lock, result = SAFU_RESULT_FAILED);
            if (result == SAFU_RESULT_OK) {
                retVal =
                    safuVecIterate(&logAggregator->pluginControlPtrVector, _logAggregatorFindEventsHelper, &helperData);
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

        elosEventFilterDeleteMembers(&filter);
    }

    return result;
}

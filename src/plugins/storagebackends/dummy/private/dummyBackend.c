// SPDX-License-Identifier: MIT

#include <safu/common.h>
#include <safu/log.h>
#include <safu/mutex.h>
#include <stdlib.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include "elos/eventfilter/eventfilter.h"
#include "elos/eventlogging/StorageBackend.h"
#include "elos/plugin/types.h"

static safuResultE_t _backendStart(elosStorageBackend_t *backend) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (backend == NULL) {
        safuLogErr("Null parameter given");
    } else {
        result = SAFU_RESULT_OK;
    }

    return result;
}

static safuResultE_t _backendPersist(elosStorageBackend_t *backend, const elosEvent_t *event) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((backend == NULL) || (event == NULL)) {
        safuLogErr("Null parameter given");
    } else {
        char *eventStr = NULL;

        result = elosEventSerialize(&eventStr, event);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("Event serialization failed");
        } else {
            safuLogDebugF("Backend '%s' is persisting Event: %s", backend->name, eventStr);
            safuLogDebug("I am just a dummy backend and can't persist anything.");
            free(eventStr);
        }
    }

    return result;
}

static safuResultE_t _backendFindEvent(elosStorageBackend_t *backend, elosRpnFilter_t *filter, safuVec_t *events) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((backend == NULL) || (filter == NULL) || (events == NULL)) {
        safuLogErr("Null parameter given");
    } else {
        safuLogDebug("I am just a dummy backend and can't remember anything.");
        elosRpnFilterResultE_t filterResult;

        elosEvent_t event = {0};
        filterResult = elosEventFilterExecute(filter, NULL, &event);
        if (filterResult == RPNFILTER_RESULT_MATCH || filterResult == RPNFILTER_RESULT_NO_MATCH) {
            result = SAFU_RESULT_OK;
        } else {
            safuLogErr("elosEventFilterExecute failed");
            safuLogDebugF("Free Event: RPN filter result is: %d", filterResult);
            result = SAFU_RESULT_FAILED;
        }
    }

    return result;
}

static safuResultE_t _backendShutdown(elosStorageBackend_t *backend) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (backend == NULL) {
        safuLogErr("Null parameter given");
    } else {
        result = SAFU_RESULT_OK;
    }

    return result;
}

safuResultE_t elosPluginLoad(elosPluginContext_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
    } else {
        elosStorageBackend_t *newBackend;

        newBackend = safuAllocMem(NULL, sizeof(elosStorageBackend_t));
        if (newBackend == NULL) {
            safuLogErr("Memory allocation failed");
        } else if ((plugin->config == NULL) || (plugin->config->key == NULL)) {
            safuLogErr("Given configuration is NULL or has .key set to NULL");
        } else {
            elosStorageBackend_t const backendValues = {
                .name = plugin->config->key,
                .backendData = NULL,
                .start = &_backendStart,
                .persist = &_backendPersist,
                .findEvent = &_backendFindEvent,
                .shutdown = &_backendShutdown,
            };

            memcpy(newBackend, &backendValues, sizeof(elosStorageBackend_t));
            plugin->data = newBackend;

            safuLogDebugF("Plugin '%s' has been loaded", plugin->config->key);
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

safuResultE_t elosPluginStart(elosPluginContext_t *plugin) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
        result = SAFU_RESULT_FAILED;
    } else {
        eventfd_t efdVal = 0;
        int retVal;

        safuLogDebugF("Plugin '%s' has been started", plugin->config->key);

        retVal = eventfd_write(plugin->sync, 1);
        if (retVal < 0) {
            safuLogErrErrno("eventfd_write (worker.sync) failed");
            result = SAFU_RESULT_FAILED;
        }

        retVal = eventfd_read(plugin->stop, &efdVal);
        if (retVal < 0) {
            safuLogErrErrno("eventfd_read (stop) failed");
            result = SAFU_RESULT_FAILED;
        }
    }

    return result;
}

safuResultE_t elosPluginStop(elosPluginContext_t *plugin) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
        result = SAFU_RESULT_FAILED;
    } else {
        int retVal;

        safuLogDebugF("Stopping Plugin '%s'", plugin->config->key);

        retVal = eventfd_write(plugin->stop, 1);
        if (retVal < 0) {
            safuLogErrErrno("eventfd_write (stop) failed");
            result = SAFU_RESULT_FAILED;
        }
    }

    return result;
}

safuResultE_t elosPluginUnload(elosPluginContext_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
    } else if ((plugin->state != PLUGIN_STATE_LOADED) && (plugin->state != PLUGIN_STATE_STOPPED)) {
        safuLogErrF("Plugin '%s' not in state 'LOADED' or 'STOPPED' (state=%d)", plugin->config->key, plugin->state);
    } else {
        safuLogDebugF("Unloading Plugin '%s'", plugin->config->key);
        free(plugin->data);
        result = SAFU_RESULT_OK;
    }

    return result;
}

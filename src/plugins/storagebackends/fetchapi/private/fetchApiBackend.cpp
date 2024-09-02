// SPDX-License-Identifier: MIT

#include <elos/event/event.h>
#include <elos/event/event_types.h>
#include <elos/eventfilter/eventfilter.h>
#include <elos/libelosplugin/StorageBackend_types.h>
#include <elos/libelosplugin/libelosplugin.h>
#include <safu/common.h>
#include <safu/log.h>
#include <safu/mutex.h>
#include <safu/result.h>
#include <safu/ringbuffer.h>
#include <safu/ringbuffer_types.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include <new>

#include "public/EventBuffer.h"

static safuResultE_t _backendStart(elosStorageBackend_t *backend) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (backend == nullptr) {
        safuLogErr("Null parameter given");
    } else {
        result = SAFU_RESULT_OK;
    }
    return result;
}

static safuResultE_t _backendPersist(elosStorageBackend_t *backend, const elosEvent_t *event) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((backend == nullptr) || (event == nullptr)) {
        safuLogErr("Null parameter given");
    } else {
        auto *eventBuffer = (elosEventBuffer *)backend->backendData;
        result = eventBuffer->elosPushEvent(*event);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("Event serialization failed");
        }
    }
    return result;
}

static safuResultE_t _backendFindEvent(elosStorageBackend_t *backend, elosRpnFilter_t *filter, struct timespec newest,
                                       struct timespec oldest, safuVec_t *events) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((backend == nullptr) || (filter == nullptr) || (events == nullptr)) {
        safuLogErr("Null parameter given");
    } else {
        auto *eventBuffer = (elosEventBuffer *)backend->backendData;
        result = eventBuffer->elosFindEvents(*filter, *events);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("Finding events failed!");
        }
    }
    return result;
}

static safuResultE_t _backendShutdown(elosStorageBackend_t *backend) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (backend == nullptr) {
        safuLogErr("Null parameter given");
    } else {
        result = SAFU_RESULT_OK;
    }
    return result;
}

static safuResultE_t _pluginLoad(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (plugin == nullptr) {
        safuLogErr("Null parameter given");
    } else {
        elosStorageBackend_t *newBackend = new (std::nothrow) elosStorageBackend_t;
        if (newBackend == nullptr) {
            safuLogErr("Memory allocation failed");
        } else if ((plugin->config == nullptr) || (plugin->config->key == nullptr)) {
            safuLogErr("Given configuration is nullptr or has .key set to nullptr");
        } else {
            newBackend->name = plugin->config->key;
            newBackend->filter = safuVec_t();
            newBackend->start = &_backendStart;
            newBackend->persist = &_backendPersist;
            newBackend->findEvent = &_backendFindEvent;
            newBackend->shutdown = &_backendShutdown;

            size_t elements = samconfConfigGetInt32Or(plugin->config, "Config/BufferSize", 1000);

            try {
                newBackend->backendData = new elosEventBuffer(elements);
            } catch (safuResultE_t err) {
                result = err;
                safuLogErr("event buffer initialization failed!");
                delete newBackend;
            } catch (std::bad_alloc &_) {
                result = SAFU_RESULT_FAILED;
                safuLogErr("allocating event buffer failed!");
                delete newBackend;
            }
            if (result == SAFU_RESULT_OK) {
                plugin->data = newBackend;

                safuLogDebugF("Plugin '%s' has been loaded", plugin->config->key);
            }
        }
    }
    return result;
}

static safuResultE_t _pluginStart(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == nullptr) {
        safuLogErr("Null parameter given");
    } else {
        safuLogDebugF("Plugin '%s' has been started", plugin->config->key);

        result = elosPluginReportAsStarted(plugin);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErr("elosPluginReportAsStarted failed");
        } else {
            result = elosPluginStopTriggerWait(plugin);
            if (result == SAFU_RESULT_FAILED) {
                safuLogErr("elosPluginStopTriggerWait failed");
            }
        }
    }

    return result;
}

static safuResultE_t _pluginStop(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == nullptr) {
        safuLogErr("Null parameter given");
    } else {
        safuLogDebugF("Stopping Plugin '%s'", plugin->config->key);

        result = elosPluginStopTriggerWrite(plugin);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErr("elosPluginStopTriggerWrite failed");
        }
    }

    return result;
}

static safuResultE_t _pluginUnload(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (plugin == nullptr) {
        safuLogErr("Null parameter given");
    } else {
        safuLogDebugF("Unloading Plugin '%s'", plugin->config->key);
        auto *backendValues = (elosStorageBackend_t *)plugin->data;
        if (backendValues != nullptr) {
            auto *eventBuffer = (elosEventBuffer *)backendValues->backendData;
            try {
                delete eventBuffer;
            } catch (safuResultE_t error) {
                result = error;
                safuLogErr("Failed to free the event buffer!");
            }
            delete backendValues;
        }
    }
    return result;
}

__BEGIN_DECLS

elosPluginConfig_t elosPluginConfig({
    PLUGIN_TYPE_STORAGEBACKEND,
    _pluginLoad,
    _pluginUnload,
    _pluginStart,
    _pluginStop,
});

__END_DECLS

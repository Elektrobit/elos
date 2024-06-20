// SPDX-License-Identifier: MIT

#include "elos/libelosplugin/libelosplugin.h"

#include <safu/log.h>
#include <sys/eventfd.h>

#define _STOP_TRIGGER_VALUE 1

safuResultE_t elosPluginReportAsStarted(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("Invalid parameter given");
    } else {
        int retVal;

        retVal = eventfd_write(plugin->sync, 1);
        if (retVal < 0) {
            safuLogErrErrno("eventfd_write (context.sync) failed");
        } else {
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

safuResultE_t elosPluginStopTriggerWait(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("Invalid parameter given");
    } else {
        eventfd_t efdVal = 0;
        int retVal;

        retVal = eventfd_read(plugin->stop, &efdVal);
        if (retVal < 0) {
            safuLogErrErrno("eventfd_read (stop) failed");
        } else {
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

safuResultE_t elosPluginStopTriggerWrite(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("Invalid parameter given");
    } else {
        int retVal;

        retVal = eventfd_write(plugin->stop, _STOP_TRIGGER_VALUE);
        if (retVal < 0) {
            safuLogErrErrno("eventfd_write (stop) failed");
        } else {
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

static const char *elosPluginTypeLookup[] = {
    [PLUGIN_TYPE_INVALID] = "Invalid",
    [PLUGIN_TYPE_SCANNER] = "Scanner",
    [PLUGIN_TYPE_STORAGEBACKEND] = "StorageBackends",
    [PLUGIN_TYPE_CLIENTCONNECTION] = "ClientConnections",
};
const char *elosPluginTypeToStr(elosPluginTypeE_t type) {
    if (type >= sizeof(elosPluginTypeLookup)) {
        type = PLUGIN_TYPE_INVALID;
    }
    return elosPluginTypeLookup[type];
}

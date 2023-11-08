// SPDX-License-Identifier: MIT

#include "elos/libelosplugin/libelosplugin.h"

#include <safu/log.h>
#include <sys/eventfd.h>

#define _STOP_TRIGGER_VALUE 1

safuResultE_t elosPluginReportAsStarted(elosPluginContext_t *plugin) {
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

safuResultE_t elosPluginStopTriggerWait(elosPluginContext_t *plugin) {
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

safuResultE_t elosPluginStopTriggerWrite(elosPluginContext_t *plugin) {
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

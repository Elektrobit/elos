// SPDX-License-Identifier: MIT

#include <pthread.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include "elos/plugincontrol/plugincontrol.h"
#include "elos/plugincontrol/thread.h"
#include "safu/log.h"
#include "safu/mutex.h"

void *elosPluginWorkerThread(void *data) {
    elosPlugin_t *plugin = (elosPlugin_t *)data;
    safuResultE_t result = SAFU_RESULT_OK;
    int retVal;

    result = plugin->func[ELOS_PLUGIN_FUNC_LOAD].ptr(data);
    if (result != SAFU_RESULT_OK) {
        plugin->state = PLUGIN_STATE_ERROR;
        safuLogErr("plugin load function call failed");
    } else {
        plugin->state = PLUGIN_STATE_LOADED;

        retVal = eventfd_write(plugin->worker.sync, 1);
        if (retVal < 0) {
            safuLogErrErrno("pthread_cond_wait failed");
            result = SAFU_RESULT_FAILED;
        }
    }

    if (result == SAFU_RESULT_OK) {
        eventfd_t efdVal = 0;

        retVal = eventfd_read(plugin->sync, &efdVal);
        if (retVal < 0) {
            safuLogErrErrno("eventfd_read (sync) failed");
            result = SAFU_RESULT_FAILED;
        } else {
            plugin->state = PLUGIN_STATE_STARTED;

            result = plugin->func[ELOS_PLUGIN_FUNC_START].ptr(data);
            if (result != SAFU_RESULT_OK) {
                plugin->state = PLUGIN_STATE_ERROR;
                safuLogErr("plugin load function call failed");
            } else {
                plugin->state = PLUGIN_STATE_STOPPED;

                retVal = eventfd_write(plugin->worker.sync, 1);
                if (retVal < 0) {
                    safuLogErrErrno("eventfd_write (worker.sync) failed");
                    result = SAFU_RESULT_FAILED;
                }
            }
        }
    }

    return NULL;
}

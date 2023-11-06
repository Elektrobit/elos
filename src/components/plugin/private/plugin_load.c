// SPDX-License-Identifier: MIT

#define _GNU_SOURCE

#include "plugin_load.h"

#include <dlfcn.h>
#include <safu/log.h>
#include <safu/mutex.h>
#include <samconf/samconf.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include "elos/plugin/plugin.h"
#include "elos/plugin/thread.h"

// Note: Helper functions are meant to be called only internally, due to that we don't need value validity checks.
//       These normally would be marked 'static', but are not to make unit testing easier.

safuResultE_t elosPluginLoadHelperSearchFile(elosPlugin_t const *plugin, char **file) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    char *newFile = NULL;

    if (plugin->path == NULL) {
        result = SAFU_RESULT_OK;

        if (plugin->file != NULL) {
            int retVal;

            retVal = access(plugin->file, F_OK | R_OK);
            if (retVal != 0) {
                safuLogErrF("access on file '%s' failed (returned: %d, %s)", newFile, retVal, strerror(errno));
                result = SAFU_RESULT_FAILED;
            }
        }
    } else {
        samconfConfigStatusE_t configResult;
        char const *fileName = NULL;

        configResult = samconfConfigGetString(plugin->context.config, "File", &fileName);
        if (configResult != SAMCONF_CONFIG_OK) {
            safuLogErrF("samconfConfigGet 'File' failed : %d", configResult);
        } else if (fileName == NULL) {
            *file = NULL;
        } else {
            int retVal;

            retVal = asprintf(&newFile, "%s/%s", plugin->path, fileName);
            if (retVal <= 0) {
                safuLogErr("asprintf failed");
            } else if (retVal <= 1) {
                safuLogErr("asprintf returned an empty string");
            } else {
                retVal = access(newFile, F_OK | R_OK);
                if (retVal < 0) {
                    safuLogErrF("access on file '%s' failed (returned: %d, %s)", newFile, retVal, strerror(errno));
                    free(newFile);
                } else {
                    *file = newFile;
                    result = SAFU_RESULT_OK;
                }
            }
        }
    }

    return result;
}

safuResultE_t elosPluginLoadHelperGetFunctions(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_OK;
    char *errStr = NULL;

    result = elosPluginLoadHelperSearchFile(plugin, &plugin->file);
    if (result == SAFU_RESULT_OK) {
        plugin->dlHandle = dlopen(plugin->file, RTLD_LAZY);
        errStr = dlerror();
        if ((plugin->dlHandle == NULL) || (errStr != NULL)) {
            safuLogErrF("dlopen failed with: '%s'", errStr);
            result = SAFU_RESULT_FAILED;
        } else {
            for (size_t i = 0; i < ELOS_PLUGIN_FUNC_COUNT; i += 1) {
                elosPluginFuncEntry_t *func = &plugin->func[i];
                void **ptr = (void **)&(func->ptr);

                *ptr = dlsym(plugin->dlHandle, func->name);
                errStr = dlerror();
                if ((*ptr == NULL) || (errStr != NULL)) {
                    safuLogErrF("dlsym failed with: '%s'", errStr);
                    result = SAFU_RESULT_FAILED;
                    break;
                }
            }
        }
    }

    return result;
}

safuResultE_t elosPluginLoad(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("Invalid parameter");
    } else {
        bool loadNeeded = false;

        switch (plugin->context.state) {
            case PLUGIN_STATE_INITIALIZED:
                loadNeeded = true;
                break;
            case PLUGIN_STATE_INVALID:
            case PLUGIN_STATE_STARTED:
            case PLUGIN_STATE_STOPPED:
            case PLUGIN_STATE_LOADED:
            case PLUGIN_STATE_UNLOADED:
            case PLUGIN_STATE_ERROR:
                safuLogErrF("Plugin is not in state 'INITIALIZED': state=%d'", plugin->context.state);
                break;
            default:
                safuLogErrF("Plugin is in unknown state '%d'", plugin->context.state);
                break;
        }

        if (loadNeeded == true) {
            int retVal;

            result = elosPluginLoadHelperGetFunctions(plugin);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("elosPluginLoadHelperGetFunctions failed");
            }

            if (result == SAFU_RESULT_OK) {
                retVal = pthread_create(&plugin->workerThread, 0, elosPluginWorkerThread, (void *)plugin);
                if (retVal < 0) {
                    safuLogErrErrno("pthread_create failed");
                    atomic_fetch_and(&plugin->flags, ~ELOS_PLUGIN_FLAG_WORKERRUNNING);
                    result = SAFU_RESULT_FAILED;
                } else {
                    atomic_fetch_or(&plugin->flags, ELOS_PLUGIN_FLAG_WORKERRUNNING);
                }
            }

            if (result == SAFU_RESULT_OK) {
                eventfd_t efdVal = 0;

                retVal = eventfd_read(plugin->context.sync, &efdVal);
                if (retVal < 0) {
                    safuLogErrErrno("eventfd_read failed");
                    result = SAFU_RESULT_FAILED;
                } else {
                    if (plugin->context.state != PLUGIN_STATE_LOADED) {
                        safuLogErr("Plugin not in state 'LOADED' after worker thread execution");
                        result = SAFU_RESULT_FAILED;
                    }
                }
            }
        }

        if (result != SAFU_RESULT_OK) {
            plugin->context.state = PLUGIN_STATE_ERROR;
        }
    }

    return result;
}

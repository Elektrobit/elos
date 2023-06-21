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
    samconfConfigStatusE_t configResult;
    char const *fileName = NULL;

    configResult = samconfConfigGetString(plugin->config, "File", &fileName);
    if (configResult != SAMCONF_CONFIG_OK) {
        safuLogErrF("samconfConfigGet 'File' failed : %d", configResult);
    } else {
        char *newFile = NULL;
        int retVal;

        retVal = asprintf(&newFile, "%s/%s", plugin->path, fileName);
        if (retVal <= 0) {
            safuLogErr("asprintf failed");
        } else if (retVal <= 1) {
            safuLogErr("asprintf returned an empty string");
        } else {
            retVal = access(newFile, F_OK | R_OK);
            if (retVal < 0) {
                safuLogErrF("access on file '%s' failed with: %s", newFile, strerror(errno));
                free(newFile);
            } else {
                *file = newFile;
                result = SAFU_RESULT_OK;
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
            elosFuncTableEntry_t funcTable[ELOS_FUNCTABLEENTRIES] = {
                {.name = "elosPluginLoad", .func = (void **)&plugin->func.load},
                {.name = "elosPluginStart", .func = (void **)&plugin->func.start},
                {.name = "elosPluginStop", .func = (void **)&plugin->func.stop},
                {.name = "elosPluginUnload", .func = (void **)&plugin->func.unload},
            };

            for (size_t i = 0; i < ELOS_FUNCTABLEENTRIES; i += 1) {
                *funcTable[i].func = dlsym(plugin->dlHandle, funcTable[i].name);
                errStr = dlerror();
                if ((funcTable[i].func == NULL) || (errStr != NULL)) {
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

        switch (plugin->state) {
            case PLUGIN_STATE_INITIALIZED:
                loadNeeded = true;
                break;
            case PLUGIN_STATE_INVALID:
            case PLUGIN_STATE_STARTED:
            case PLUGIN_STATE_STOPPED:
            case PLUGIN_STATE_LOADED:
            case PLUGIN_STATE_UNLOADED:
            case PLUGIN_STATE_ERROR:
                safuLogErrF("Plugin is not in state 'INITIALIZED': state=%d'", plugin->state);
                break;
            default:
                safuLogErrF("Plugin is in unknown state '%d'", plugin->state);
                break;
        }

        if (loadNeeded == true) {
            int retVal;

            result = elosPluginLoadHelperGetFunctions(plugin);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("elosPluginLoadHelperGetFunctions failed");
            }

            if (result == SAFU_RESULT_OK) {
                retVal = pthread_create(&plugin->worker.thread, 0, elosPluginWorkerThread, (void *)plugin);
                if (retVal < 0) {
                    safuLogErrErrno("pthread_create failed");
                    plugin->worker.isThreadRunning = false;
                    result = SAFU_RESULT_FAILED;
                } else {
                    plugin->worker.isThreadRunning = true;
                }
            }

            if (result == SAFU_RESULT_OK) {
                eventfd_t efdVal = 0;

                retVal = eventfd_read(plugin->worker.sync, &efdVal);
                if (retVal < 0) {
                    safuLogErrErrno("eventfd_read failed");
                    result = SAFU_RESULT_FAILED;
                } else {
                    if (plugin->state != PLUGIN_STATE_LOADED) {
                        safuLogErr("Plugin not in state 'LOADED' after worker thread execution");
                        result = SAFU_RESULT_FAILED;
                    }
                }
            }
        }

        if (result != SAFU_RESULT_OK) {
            plugin->state = PLUGIN_STATE_ERROR;
        }
    }

    return result;
}

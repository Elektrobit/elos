// SPDX-License-Identifier: MIT

#define _GNU_SOURCE

#include "plugincontrol_load.h"

#include <dlfcn.h>
#include <safu/log.h>
#include <safu/mutex.h>
#include <samconf/samconf.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include "elos/plugincontrol/thread.h"
#include "plugincontrol_private.h"

// Note: Helper functions are meant to be called only internally, due to that we don't need value validity checks.
//       These normally would be marked 'static', but are not to make unit testing easier.

safuResultE_t elosPluginControlLoadHelperSearchFile(elosPluginControl_t const *control, char **file) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    char *newFile = NULL;

    if (control->path == NULL) {
        result = SAFU_RESULT_OK;

        if (control->file != NULL) {
            int retVal;

            retVal = access(control->file, F_OK | R_OK);
            if (retVal != 0) {
                safuLogErrF("access on file '%s' failed (returned: %d, %s)", newFile, retVal, strerror(errno));
                result = SAFU_RESULT_FAILED;
            }
        }
    } else {
        samconfConfigStatusE_t configResult;
        char const *fileName = NULL;

        configResult = samconfConfigGetString(control->context.config, "File", &fileName);
        if (configResult != SAMCONF_CONFIG_OK) {
            safuLogErrF("samconfConfigGet 'File' failed : %d", configResult);
        } else if (fileName == NULL) {
            *file = NULL;
        } else {
            int retVal;

            retVal = asprintf(&newFile, "%s/%s", control->path, fileName);
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

safuResultE_t elosPluginControlLoadHelperGetConfig(elosPluginControl_t *control) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (control->pluginConfig != NULL) {
        result = SAFU_RESULT_OK;
    } else {
        char *errStr = NULL;

        result = elosPluginControlLoadHelperSearchFile(control, &control->file);
        if (result == SAFU_RESULT_OK) {
            control->dlHandle = dlopen(control->file, RTLD_LAZY);
            errStr = dlerror();
            if ((control->dlHandle == NULL) || (errStr != NULL)) {
                safuLogErrF("dlopen failed with: '%s'", errStr);
            } else {
                void const **ptr = (void const **)&(control->pluginConfig);

                *ptr = dlsym(control->dlHandle, ELOS_PLUGINCONTROL_CONFIG_NAME);
                errStr = dlerror();
                if ((*ptr == NULL) || (errStr != NULL)) {
                    safuLogErrF("dlsym failed with: '%s'", errStr);
                } else {
                    result = SAFU_RESULT_OK;
                }
            }
        }
    }

    if (result == SAFU_RESULT_OK) {
        elosPluginConfig_t const *pluginConfig = control->pluginConfig;

        if (control->pluginType != pluginConfig->type) {
            safuLogErrF("plugin id:%d has wrong type (expected: %d, found: %d)", control->context.id,
                        control->pluginType, pluginConfig->type);
            result = SAFU_RESULT_FAILED;
        } else if (pluginConfig->load == NULL) {
            safuLogErrF("plugin id:%d has no load function set (.load == NULL)", control->context.id);
            result = SAFU_RESULT_FAILED;
        } else if (pluginConfig->unload == NULL) {
            safuLogErrF("plugin id:%d has no unload function set (.unload == NULL)", control->context.id);
            result = SAFU_RESULT_FAILED;
        } else if (pluginConfig->start == NULL) {
            safuLogErrF("plugin id:%d has no start function set (.start == NULL)", control->context.id);
            result = SAFU_RESULT_FAILED;
        } else if (pluginConfig->stop == NULL) {
            safuLogErrF("plugin id:%d has no stop function set (.stop == NULL)", control->context.id);
            result = SAFU_RESULT_FAILED;
        }
    }

    return result;
}

safuResultE_t elosPluginControlLoad(elosPluginControl_t *control) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (control == NULL) {
        safuLogErr("Invalid parameter");
    } else {
        bool loadNeeded = false;

        switch (control->context.state) {
            case PLUGIN_STATE_INITIALIZED:
                loadNeeded = true;
                break;
            case PLUGIN_STATE_INVALID:
            case PLUGIN_STATE_STARTED:
            case PLUGIN_STATE_STOPPED:
            case PLUGIN_STATE_LOADED:
            case PLUGIN_STATE_UNLOADED:
            case PLUGIN_STATE_ERROR:
                safuLogErrF("Plugin is not in state 'INITIALIZED': state=%d'", control->context.state);
                break;
            default:
                safuLogErrF("Plugin is in unknown state '%d'", control->context.state);
                break;
        }

        if (loadNeeded == true) {
            int retVal;

            result = elosPluginControlLoadHelperGetConfig(control);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("elosPluginControlLoadHelperGetConfig failed");
            }

            if (result == SAFU_RESULT_OK) {
                retVal = pthread_create(&control->workerThread, 0, elosPluginControlWorkerThread, (void *)control);
                if (retVal < 0) {
                    safuLogErrErrno("pthread_create failed");
                    atomic_fetch_and(&control->flags, ~ELOS_PLUGIN_FLAG_WORKERRUNNING);
                    result = SAFU_RESULT_FAILED;
                } else {
                    atomic_fetch_or(&control->flags, ELOS_PLUGIN_FLAG_WORKERRUNNING);
                }
            }

            if (result == SAFU_RESULT_OK) {
                eventfd_t efdVal = 0;

                retVal = eventfd_read(control->context.sync, &efdVal);
                if (retVal < 0) {
                    safuLogErrErrno("eventfd_read failed");
                    result = SAFU_RESULT_FAILED;
                } else {
                    if (control->context.state != PLUGIN_STATE_LOADED) {
                        safuLogErr("Plugin not in state 'LOADED' after worker thread execution");
                        result = SAFU_RESULT_FAILED;
                    }
                }
            }
        }

        if (result != SAFU_RESULT_OK) {
            control->context.state = PLUGIN_STATE_ERROR;
        }
    }

    return result;
}

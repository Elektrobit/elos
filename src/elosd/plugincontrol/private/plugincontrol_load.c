// SPDX-License-Identifier: MIT

// clang-format off
#define _GNU_SOURCE
#include <pthread.h>
// clang-format on

#include "plugincontrol_load.h"

#include <dlfcn.h>
#include <safu/defines.h>
#include <safu/log.h>
#include <safu/mutex.h>
#include <safu/result.h>
#include <samconf/samconf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include "elos/eventbuffer/eventbuffer.h"
#include "elos/eventdispatcher/eventdispatcher.h"
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
                result = SAFU_RESULT_FAILED;
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
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&control->flags) == false) {
        safuLogErr("Component is not initialized");
    } else if (ELOS_PLUGINCONTROL_FLAG_HAS_PLUGINERROR_BIT(&control->flags) == true) {
        safuLogErr("PluginControl is in an invalid state due to Plugin errors");
    } else {
        result = SAFU_RESULT_OK;

        if (ELOS_PLUGINCONTROL_FLAG_PUSH_LOADED_BIT(&control->flags) == true) {
            int retVal;

            result = elosPluginControlLoadHelperGetConfig(control);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("elosPluginControlLoadHelperGetConfig failed");
            } else {
                control->context.instanceRef = control;
                if ((result == SAFU_RESULT_OK) && (ELOS_PLUGINCONTROL_FLAG_PUSH_WORKER_BIT(&control->flags) == true)) {
                    retVal = pthread_create(&control->workerThread, 0, elosPluginControlWorkerThread, (void *)control);
                    if (retVal < 0) {
                        safuLogErrErrno("pthread_create failed");
                        atomic_fetch_or(&control->flags, SAFU_FLAG_ERROR_BIT);
                        atomic_fetch_and(&control->flags, ~ELOS_PLUGINCONTROL_FLAG_WORKER_BIT);
                        result = SAFU_RESULT_FAILED;
                    } else {
                        char threadName[20] = {0};
                        strncpy(threadName, control->context.config->key, ARRAY_SIZE(threadName) - 1);
                        retVal = pthread_setname_np(control->workerThread, threadName);
                        if (retVal != 0) {
                            safuLogErr("Failed to set thread name for plugin control");
                        }
                    }

                    if (result == SAFU_RESULT_OK) {
                        eventfd_t efdVal = 0;

                        retVal = eventfd_read(control->context.sync, &efdVal);
                        if (retVal < 0) {
                            safuLogErrErrno("eventfd_read failed");
                            result = SAFU_RESULT_FAILED;
                        }
                    }
                }
            }
        }
    }

    return result;
}

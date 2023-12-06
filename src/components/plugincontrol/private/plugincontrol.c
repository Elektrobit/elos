// SPDX-License-Identifier: MIT

#include "elos/plugincontrol/plugincontrol.h"

#include <dlfcn.h>
#include <safu/common.h>
#include <safu/log.h>
#include <safu/mutex.h>
#include <stdlib.h>
#include <sys/eventfd.h>
#include <unistd.h>

#define _LOG_ERR_CONTROL_WRONG_STATE(__control, __state)                                          \
    safuLogErrF("Plugin id=%d is not in state '%s' (state=%d)", (__control)->context.id, __state, \
                (__control)->context.state);

#define _LOG_ERR_CONTROL_UNKNOWN_STATE(__control) \
    safuLogErrF("Plugin id=%d is in unknown state=%d", (__control)->context.id, (__control)->context.state);

safuResultE_t elosPluginControlInitialize(elosPluginControl_t *control, elosPluginControlParam_t const *param) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((control == NULL) || (param == NULL)) {
        safuLogErr("Invalid parameter");
    } else if ((param->path != NULL) && ((param->config == NULL) || (param->file != NULL))) {
        safuLogErr("Invalid value combination in parameter struct");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&control->flags) == true) {
        safuLogErr("Component is already initialized");
    } else {
        int eventFdWorker;
        int eventFdSync;
        int eventFdStop;

        memset(control, 0, sizeof(elosPluginControl_t));
        control->context.stop = -1;
        control->context.sync = -1;
        control->sync = -1;

        eventFdStop = eventfd(0, 0);
        if (eventFdStop == -1) {
            safuLogErrErrnoValue("eventfd failed", eventFdStop);
        } else {
            eventFdSync = eventfd(0, 0);
            if (eventFdSync == -1) {
                safuLogErrErrnoValue("eventfd (sync) failed", eventFdSync);
            } else {
                eventFdWorker = eventfd(0, 0);
                if (eventFdWorker == -1) {
                    safuLogErrErrnoValue("eventfd (worker) failed", eventFdWorker);
                } else {
                    result = SAFU_RESULT_OK;

                    if (param->file != NULL) {
                        control->file = strdup(param->file);
                        if (control->file == NULL) {
                            safuLogErr("String duplication failed");
                            result = SAFU_RESULT_FAILED;
                        }
                    }

                    if (result == SAFU_RESULT_OK) {
                        control->context.config = param->config;
                        control->context.data = param->data;
                        control->context.stop = eventFdStop;
                        control->context.sync = eventFdWorker;
                        control->context.id = param->id;
                        control->pluginType = param->pluginType;
                        control->path = param->path;
                        control->sync = eventFdSync;
                        atomic_store(&control->flags, SAFU_FLAG_INITIALIZED_BIT);
                    }
                }
            }
        }

        if (result != SAFU_RESULT_OK) {
            elosPluginControlDeleteMembers(control);
        }
    }

    return result;
}

safuResultE_t elosPluginControlNew(elosPluginControl_t **control, elosPluginControlParam_t const *param) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((control == NULL) || (param == NULL)) {
        safuLogErr("Invalid parameter");
    } else {
        void *newPlugin;
        newPlugin = safuAllocMem(NULL, sizeof(elosPluginControl_t));
        if (newPlugin == NULL) {
            safuLogErr("Memory allocation failed");
        } else {
            memset(newPlugin, 0, sizeof(elosPluginControl_t));

            result = elosPluginControlInitialize(newPlugin, param);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("elosPluginControlInitialize failed");
            } else {
                *control = newPlugin;
            }
        }
    }

    return result;
}

safuResultE_t elosPluginControlStart(elosPluginControl_t *control) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (control == NULL) {
        safuLogErr("Invalid parameter");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&control->flags) == false) {
        safuLogErr("Component is not initialized");
    } else if (ELOS_PLUGINCONTROL_FLAG_HAS_LOADED_BIT(&control->flags) == false) {
        safuLogErr("Plugin is not loaded");
    } else if (ELOS_PLUGINCONTROL_FLAG_HAS_PLUGINERROR_BIT(&control->flags) == true) {
        safuLogErr("PluginControl is in an invalid state due to Plugin errors");
    } else {
        result = SAFU_RESULT_OK;

        if (ELOS_PLUGINCONTROL_FLAG_PUSH_STARTED_BIT(&control->flags) == true) {
            int retVal;

            retVal = eventfd_write(control->sync, 1);
            if (retVal < 0) {
                safuLogErrErrno("eventfd_write (sync) failed");
                result = SAFU_RESULT_FAILED;
            } else {
                eventfd_t efdVal = 0;

                retVal = eventfd_read(control->context.sync, &efdVal);
                if (retVal < 0) {
                    safuLogErrErrno("eventfd_read (worker.sync) failed");
                    result = SAFU_RESULT_FAILED;
                }
            }
        }
    }

    return result;
}

safuResultE_t elosPluginControlStop(elosPluginControl_t *control) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (control == NULL) {
        safuLogErr("NULL parameter");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&control->flags) == false) {
        safuLogErr("Component is not initialized");
    } else if (ELOS_PLUGINCONTROL_FLAG_HAS_LOADED_BIT(&control->flags) == false) {
        safuLogErr("Plugin is not loaded");
    } else {
        result = SAFU_RESULT_OK;

        if (ELOS_PLUGINCONTROL_FLAG_PULL_STARTED_BIT(&control->flags) == true) {
            if ((control->pluginConfig != NULL) && (control->pluginConfig->stop != NULL)) {
                if (ELOS_PLUGINCONTROL_FLAG_HAS_PLUGINERROR_BIT(&control->flags) == true) {
                    safuLogWarn("PluginControl had errors during Plugin operation; Trying to stop Plugin.");
                }

                result = control->pluginConfig->stop(&control->context);
                if (result != SAFU_RESULT_OK) {
                    safuLogErrF("PluginWorker stop call failed for %s", control->context.config->key);
                } else {
                    eventfd_t efdVal = 0;
                    int retVal;

                    retVal = eventfd_read(control->context.sync, &efdVal);
                    if (retVal < 0) {
                        safuLogErrErrno("eventfd_read (worker.sync) failed");
                        result = SAFU_RESULT_FAILED;
                    }
                }
            }
        }
    }

    return result;
}

safuResultE_t elosPluginControlUnload(elosPluginControl_t *control) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (control == NULL) {
        safuLogErr("Invalid parameter");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&control->flags) == false) {
        safuLogErr("The given PluginControl is not initialized");
    } else {
        result = SAFU_RESULT_OK;

        if (ELOS_PLUGINCONTROL_FLAG_HAS_PLUGINERROR_BIT(&control->flags) == true) {
            safuLogWarn("PluginControl had errors during Plugin operation; Trying to unload Plugin.");
        }

        if (ELOS_PLUGINCONTROL_FLAG_HAS_STARTED_BIT(&control->flags) == true) {
            result = elosPluginControlStop(control);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("elosPluginControlStop failed");
            }
        }

        if (ELOS_PLUGINCONTROL_FLAG_PULL_LOADED_BIT(&control->flags) == true) {
            if ((ELOS_PLUGINCONTROL_FLAG_PULL_WORKER_BIT(&control->flags) == true)) {
                int retVal;

                retVal = pthread_join(control->workerThread, NULL);
                if (retVal < 0) {
                    safuLogErr("Plugin: pthread_join failed!");
                    result = SAFU_RESULT_FAILED;
                }
            }

            if ((control->pluginConfig != NULL) && (control->pluginConfig->unload != NULL)) {
                safuResultE_t subResult;

                subResult = control->pluginConfig->unload(&control->context);
                if (subResult != SAFU_RESULT_OK) {
                    safuLogWarn("PluginWorker: Unload function failed (might result in memory leaks)");
                    result = SAFU_RESULT_FAILED;
                }
            }
        }
    }

    return result;
}

safuResultE_t elosPluginControlDeleteMembers(elosPluginControl_t *control) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (control != NULL) {
        if (SAFU_FLAG_HAS_INITIALIZED_BIT(&control->flags) == true) {
            int retVal;

            if (ELOS_PLUGINCONTROL_FLAG_HAS_LOADED_BIT(&control->flags) == true) {
                result = elosPluginControlUnload(control);
                if (result != SAFU_RESULT_OK) {
                    safuLogErr("elosPluginControlStop failed");
                }
            }

            if (control->dlHandle != NULL) {
                retVal = dlclose(control->dlHandle);
                if (retVal < 0) {
                    safuLogWarnF("dlclose for plugin id:%d failed with: %s", control->context.id, strerror(errno));
                    result = SAFU_RESULT_FAILED;
                }
            }

            free(control->file);

            if (control->context.stop != -1) {
                retVal = close(control->context.stop);
                if (retVal < 0) {
                    safuLogWarnF("close (stop) for plugin id:%d failed with: %s", control->context.id, strerror(errno));
                    result = SAFU_RESULT_FAILED;
                }
            }

            if (control->sync != -1) {
                retVal = close(control->sync);
                if (retVal < 0) {
                    safuLogWarnF("close (sync) for plugin id:%d failed with: %s", control->context.id, strerror(errno));
                    result = SAFU_RESULT_FAILED;
                }
            }

            if (control->context.sync != -1) {
                retVal = close(control->context.sync);
                if (retVal < 0) {
                    safuLogWarnF("close (worker.sync) for plugin id:%d failed with: %s", control->context.id,
                                 strerror(errno));
                    result = SAFU_RESULT_FAILED;
                }
            }

            memset(control, 0, sizeof(elosPluginControl_t));
        }
    }

    return result;
}

safuResultE_t elosPluginControlDelete(elosPluginControl_t **control) {
    safuResultE_t result = SAFU_RESULT_OK;

    if ((control != NULL) && (*control != NULL)) {
        result = elosPluginControlDeleteMembers(*control);
        free(*control);
        *control = NULL;
    }

    return result;
}

safuResultE_t elosPluginControlGetName(const elosPluginControl_t *control, const char **name) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((control == NULL) || (name == NULL)) {
        safuLogErr("Invalid parameter");
    } else if (SAFU_FLAG_HAS_INITIALIZED_BIT(&control->flags) == false) {
        safuLogErr("Component is not initialized");
    } else if (control->context.config == NULL) {
        safuLogErr("Plugin has no configuration");
    } else {
        *name = control->context.config->key;
        result = SAFU_RESULT_OK;
    }

    return result;
}

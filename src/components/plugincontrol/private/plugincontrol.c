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
    } else if (param->pluginType == PLUGIN_TYPE_INVALID) {
        safuLogErr("pluginType is not specified (pluginType == PLUGIN_TYPE_INVALID)");
    } else if (control->context.state != PLUGIN_STATE_INVALID) {
        safuLogErr("The given plugin struct is not in state 'PLUGIN_STATE_INVALID'");
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
                        atomic_store(&control->flags, SAFU_FLAG_NONE);
                    }
                }
            }
        }

        if (result != SAFU_RESULT_OK) {
            control->context.state = PLUGIN_STATE_ERROR;
            elosPluginControlDeleteMembers(control);
        } else {
            control->context.state = PLUGIN_STATE_INITIALIZED;
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
        safuLogErr("NULL parameter");
    } else {
        bool startNeeded = false;

        switch (control->context.state) {
            case PLUGIN_STATE_LOADED:
                startNeeded = true;
                break;
            case PLUGIN_STATE_INVALID:
            case PLUGIN_STATE_INITIALIZED:
            case PLUGIN_STATE_STARTED:
            case PLUGIN_STATE_STOPPED:
            case PLUGIN_STATE_UNLOADED:
            case PLUGIN_STATE_ERROR:
                _LOG_ERR_CONTROL_WRONG_STATE(control, "LOADED");
                break;
            default:
                _LOG_ERR_CONTROL_UNKNOWN_STATE(control);
                break;
        }

        if (startNeeded == true) {
            int retVal;

            retVal = eventfd_write(control->sync, 1);
            if (retVal < 0) {
                safuLogErrErrno("eventfd_write (sync) failed");
            } else {
                eventfd_t efdVal = 0;

                retVal = eventfd_read(control->context.sync, &efdVal);
                if (retVal < 0) {
                    safuLogErrErrno("eventfd_read (worker.sync) failed");
                } else if (control->context.state != PLUGIN_STATE_STARTED) {
                    _LOG_ERR_CONTROL_WRONG_STATE(control, "STARTED");
                } else {
                    result = SAFU_RESULT_OK;
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
    } else {
        bool stopNeeded = false;

        switch (control->context.state) {
            case PLUGIN_STATE_STARTED:
                stopNeeded = true;
                break;
            case PLUGIN_STATE_INVALID:
            case PLUGIN_STATE_INITIALIZED:
            case PLUGIN_STATE_STOPPED:
            case PLUGIN_STATE_LOADED:
            case PLUGIN_STATE_UNLOADED:
            case PLUGIN_STATE_ERROR:
                _LOG_ERR_CONTROL_WRONG_STATE(control, "STARTED");
                break;
            default:
                _LOG_ERR_CONTROL_UNKNOWN_STATE(control);
                break;
        }

        if ((stopNeeded == true) && (control->pluginConfig != NULL) && (control->pluginConfig->stop != NULL)) {
            result = control->pluginConfig->stop(&control->context);
            if (result != SAFU_RESULT_OK) {
                safuLogErrF("PluginWorker stop call failed for %s", control->context.config->key);
            } else {
                eventfd_t efdVal = 0;
                int retVal;

                retVal = eventfd_read(control->context.sync, &efdVal);
                if (retVal < 0) {
                    safuLogErrErrno("eventfd_read (worker.sync) failed");
                } else if (control->context.state != PLUGIN_STATE_STOPPED) {
                    _LOG_ERR_CONTROL_WRONG_STATE(control, "STOPPED");
                } else {
                    result = SAFU_RESULT_OK;
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
    } else {
        bool unloadNeeded = true;

        switch (control->context.state) {
            case PLUGIN_STATE_LOADED:
            case PLUGIN_STATE_STOPPED:
            case PLUGIN_STATE_ERROR:
                break;
            case PLUGIN_STATE_STARTED:
                result = elosPluginControlStop(control);
                if (result != SAFU_RESULT_OK) {
                    safuLogErr("elosPluginControlStop failed");
                }
                break;
            case PLUGIN_STATE_INITIALIZED:
            case PLUGIN_STATE_UNLOADED:
                unloadNeeded = false;
                break;
            default:
                _LOG_ERR_CONTROL_UNKNOWN_STATE(control);
                result = SAFU_RESULT_FAILED;
                unloadNeeded = false;
                break;
        }

        if (unloadNeeded == true) {
            if (ELOS_PLUGIN_FLAG_HAS_WORKERRUNNING_BIT(&control->flags) == true) {
                int retVal = pthread_join(control->workerThread, NULL);
                if (retVal < 0) {
                    safuLogErr("Plugin: pthread_join failed!");
                    result = SAFU_RESULT_FAILED;
                } else {
                    atomic_fetch_and(&control->flags, ~ELOS_PLUGIN_FLAG_WORKERRUNNING);
                }
            }

            if ((control->pluginConfig != NULL) && (control->pluginConfig->unload != NULL)) {
                result = control->pluginConfig->unload(&control->context);
                if (result != SAFU_RESULT_OK) {
                    safuLogWarn("PluginWorker: Unload function failed (might result in memory leaks)");
                }
            }

            control->context.state = PLUGIN_STATE_UNLOADED;
        }
    }

    return result;
}

safuResultE_t elosPluginControlDeleteMembers(elosPluginControl_t *control) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (control != NULL) {
        bool cleanupNeeded = false;

        switch (control->context.state) {
            case PLUGIN_STATE_LOADED:
            case PLUGIN_STATE_STARTED:
            case PLUGIN_STATE_STOPPED:
            case PLUGIN_STATE_ERROR:
                result = elosPluginControlUnload(control);
                if (result != SAFU_RESULT_OK) {
                    safuLogWarn("elosPluginControlUnload failed (might result in memory leaks)");
                }
                cleanupNeeded = true;
                break;
            case PLUGIN_STATE_INITIALIZED:
            case PLUGIN_STATE_UNLOADED:
                cleanupNeeded = true;
                break;
            case PLUGIN_STATE_INVALID:
                break;
            default:
                _LOG_ERR_CONTROL_UNKNOWN_STATE(control);
                result = SAFU_RESULT_FAILED;
                break;
        }

        if (cleanupNeeded == true) {
            int retVal;

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

    if (control != NULL && control->context.config != NULL && name != NULL) {
        *name = control->context.config->key;
        result = SAFU_RESULT_OK;
    } else {
        safuLogErr("Invalid parameters");
    }

    return result;
}

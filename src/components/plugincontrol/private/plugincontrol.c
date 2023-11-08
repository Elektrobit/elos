// SPDX-License-Identifier: MIT

#include "elos/plugincontrol/plugincontrol.h"

#include <dlfcn.h>
#include <safu/common.h>
#include <safu/log.h>
#include <safu/mutex.h>
#include <stdlib.h>
#include <sys/eventfd.h>
#include <unistd.h>

#define _LOG_ERR_PLUGIN_WRONG_STATE(__plugin, __state) \
    safuLogErrF("Plugin id=%d is not in state '%s' (state=%d)", (__plugin)->id, __state, (__plugin)->state);

#define _LOG_ERR_PLUGIN_UNKNOWN_STATE(__plugin) \
    safuLogErrF("Plugin id=%d is in unknown state=%d", (__plugin)->id, (__plugin)->state);

static inline safuResultE_t _funcTableDeleteMembers(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_OK;

    for (size_t i = 0; i < ELOS_PLUGIN_FUNC_COUNT; i += 1) {
        free(plugin->func[i].name);
        plugin->func[i].name = NULL;
    }

    return result;
}

static inline safuResultE_t _funcTableInitialize(elosPlugin_t *plugin, elosPluginParam_t const *param) {
    safuResultE_t result = SAFU_RESULT_OK;

    char const *const funcDefaultName[ELOS_PLUGIN_FUNC_COUNT] = {
        [ELOS_PLUGIN_FUNC_LOAD] = "elosPluginLoad",
        [ELOS_PLUGIN_FUNC_START] = "elosPluginStart",
        [ELOS_PLUGIN_FUNC_STOP] = "elosPluginStop",
        [ELOS_PLUGIN_FUNC_UNLOAD] = "elosPluginUnload",
    };

    char const *const funcOverrideName[ELOS_PLUGIN_FUNC_COUNT] = {
        param->funcOverride.load,
        param->funcOverride.start,
        param->funcOverride.stop,
        param->funcOverride.unload,
    };

    for (size_t i = 0; i < ELOS_PLUGIN_FUNC_COUNT; i += 1) {
        elosPluginFuncEntry_t *func = &plugin->func[i];
        char const *funcName = funcDefaultName[i];

        if (funcOverrideName[i] != NULL) {
            funcName = funcOverrideName[i];
        }

        func->ptr = NULL;
        func->name = strdup(funcName);
        if (func->name == NULL) {
            safuLogErr("String duplication failed");
            result = SAFU_RESULT_FAILED;
            break;
        }
    }

    if (result != SAFU_RESULT_OK) {
        _funcTableDeleteMembers(plugin);
    }

    return result;
}

safuResultE_t elosPluginControlInitialize(elosPlugin_t *plugin, elosPluginParam_t const *param) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((plugin == NULL) || (param == NULL)) {
        safuLogErr("Invalid parameter");
    } else if ((param->path != NULL) && ((param->config == NULL) || (param->file != NULL))) {
        safuLogErr("Invalid value combination in parameter struct");
    } else if (plugin->state != PLUGIN_STATE_INVALID) {
        safuLogErr("The given plugin struct is not in state 'PLUGIN_STATE_INVALID'");
    } else {
        int eventFdWorker;
        int eventFdSync;
        int eventFdStop;

        memset(plugin, 0, sizeof(elosPlugin_t));
        plugin->stop = -1;
        plugin->sync = -1;
        plugin->worker.sync = -1;

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
                        plugin->file = strdup(param->file);
                        if (plugin->file == NULL) {
                            safuLogErr("String duplication failed");
                            result = SAFU_RESULT_FAILED;
                        }
                    }

                    if (result == SAFU_RESULT_OK) {
                        plugin->id = param->id;
                        plugin->path = param->path;
                        plugin->config = param->config;
                        plugin->data = param->data;
                        plugin->stop = eventFdStop;
                        plugin->sync = eventFdSync;
                        plugin->worker.sync = eventFdWorker;
                        plugin->worker.isThreadRunning = false;

                        result = _funcTableInitialize(plugin, param);
                    }
                }
            }
        }

        if (result != SAFU_RESULT_OK) {
            plugin->state = PLUGIN_STATE_ERROR;
            elosPluginControlDeleteMembers(plugin);
        } else {
            plugin->state = PLUGIN_STATE_INITIALIZED;
        }
    }

    return result;
}

safuResultE_t elosPluginControlNew(elosPlugin_t **plugin, elosPluginParam_t const *param) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((plugin == NULL) || (param == NULL)) {
        safuLogErr("Invalid parameter");
    } else {
        void *newPlugin;
        newPlugin = safuAllocMem(NULL, sizeof(elosPlugin_t));
        if (newPlugin == NULL) {
            safuLogErr("Memory allocation failed");
        } else {
            memset(newPlugin, 0, sizeof(elosPlugin_t));

            result = elosPluginControlInitialize(newPlugin, param);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("elosPluginControlInitialize failed");
            } else {
                *plugin = newPlugin;
            }
        }
    }

    return result;
}

safuResultE_t elosPluginControlStart(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("NULL parameter");
    } else {
        bool startNeeded = false;

        switch (plugin->state) {
            case PLUGIN_STATE_LOADED:
                startNeeded = true;
                break;
            case PLUGIN_STATE_INVALID:
            case PLUGIN_STATE_INITIALIZED:
            case PLUGIN_STATE_STARTED:
            case PLUGIN_STATE_STOPPED:
            case PLUGIN_STATE_UNLOADED:
            case PLUGIN_STATE_ERROR:
                _LOG_ERR_PLUGIN_WRONG_STATE(plugin, "LOADED");
                break;
            default:
                _LOG_ERR_PLUGIN_UNKNOWN_STATE(plugin);
                break;
        }

        if (startNeeded == true) {
            int retVal;

            retVal = eventfd_write(plugin->sync, 1);
            if (retVal < 0) {
                safuLogErrErrno("eventfd_write (sync) failed");
            } else {
                eventfd_t efdVal = 0;

                retVal = eventfd_read(plugin->worker.sync, &efdVal);
                if (retVal < 0) {
                    safuLogErrErrno("eventfd_read (worker.sync) failed");
                } else if (plugin->state != PLUGIN_STATE_STARTED) {
                    _LOG_ERR_PLUGIN_WRONG_STATE(plugin, "STARTED");
                } else {
                    result = SAFU_RESULT_OK;
                }
            }
        }
    }

    return result;
}

safuResultE_t elosPluginControlStop(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("NULL parameter");
    } else {
        bool stopNeeded = false;

        switch (plugin->state) {
            case PLUGIN_STATE_STARTED:
                stopNeeded = true;
                break;
            case PLUGIN_STATE_INVALID:
            case PLUGIN_STATE_INITIALIZED:
            case PLUGIN_STATE_STOPPED:
            case PLUGIN_STATE_LOADED:
            case PLUGIN_STATE_UNLOADED:
            case PLUGIN_STATE_ERROR:
                _LOG_ERR_PLUGIN_WRONG_STATE(plugin, "STARTED");
                break;
            default:
                _LOG_ERR_PLUGIN_UNKNOWN_STATE(plugin);
                break;
        }

        if (stopNeeded == true) {
            result = plugin->func[ELOS_PLUGIN_FUNC_STOP].ptr(plugin);
            if (result != SAFU_RESULT_OK) {
                safuLogErrF("PluginWorker stop call failed for %s", plugin->config->key);
            } else {
                eventfd_t efdVal = 0;
                int retVal;

                retVal = eventfd_read(plugin->worker.sync, &efdVal);
                if (retVal < 0) {
                    safuLogErrErrno("eventfd_read (worker.sync) failed");
                } else if (plugin->state != PLUGIN_STATE_STOPPED) {
                    _LOG_ERR_PLUGIN_WRONG_STATE(plugin, "STOPPED");
                } else {
                    result = SAFU_RESULT_OK;
                }
            }
        }
    }

    return result;
}

safuResultE_t elosPluginControlUnload(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("Invalid parameter");
    } else {
        bool unloadNeeded = true;

        switch (plugin->state) {
            case PLUGIN_STATE_LOADED:
            case PLUGIN_STATE_STOPPED:
            case PLUGIN_STATE_ERROR:
                break;
            case PLUGIN_STATE_STARTED:
                result = elosPluginControlStop(plugin);
                if (result != SAFU_RESULT_OK) {
                    safuLogErr("elosPluginControlStop failed");
                }
                break;
            case PLUGIN_STATE_INITIALIZED:
            case PLUGIN_STATE_UNLOADED:
                unloadNeeded = false;
                break;
            default:
                _LOG_ERR_PLUGIN_UNKNOWN_STATE(plugin);
                result = SAFU_RESULT_FAILED;
                unloadNeeded = false;
                break;
        }

        if (unloadNeeded == true) {
            if (plugin->worker.isThreadRunning) {
                int retVal = pthread_join(plugin->worker.thread, NULL);
                if (retVal < 0) {
                    safuLogErr("Plugin: pthread_join failed!");
                    result = SAFU_RESULT_FAILED;
                } else {
                    plugin->worker.isThreadRunning = false;
                }
            }

            if (plugin->func[ELOS_PLUGIN_FUNC_UNLOAD].ptr != NULL) {
                result = plugin->func[ELOS_PLUGIN_FUNC_UNLOAD].ptr(plugin);
                if (result != SAFU_RESULT_OK) {
                    safuLogWarn("PluginWorker: Unload function failed (might result in memory leaks)");
                }
            }

            plugin->state = PLUGIN_STATE_UNLOADED;
        }
    }

    return result;
}

safuResultE_t elosPluginControlDeleteMembers(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (plugin != NULL) {
        bool cleanupNeeded = false;

        switch (plugin->state) {
            case PLUGIN_STATE_LOADED:
            case PLUGIN_STATE_STARTED:
            case PLUGIN_STATE_STOPPED:
            case PLUGIN_STATE_ERROR:
                result = elosPluginControlUnload(plugin);
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
                _LOG_ERR_PLUGIN_UNKNOWN_STATE(plugin);
                result = SAFU_RESULT_FAILED;
                break;
        }

        if (cleanupNeeded == true) {
            int retVal;

            if (plugin->dlHandle != NULL) {
                retVal = dlclose(plugin->dlHandle);
                if (retVal < 0) {
                    safuLogWarnF("dlclose for plugin id:%d failed with: %s", plugin->id, strerror(errno));
                    result = SAFU_RESULT_FAILED;
                }
            }

            free(plugin->file);

            if (plugin->stop != -1) {
                retVal = close(plugin->stop);
                if (retVal < 0) {
                    safuLogWarnF("close (stop) for plugin id:%d failed with: %s", plugin->id, strerror(errno));
                    result = SAFU_RESULT_FAILED;
                }
            }

            if (plugin->sync != -1) {
                retVal = close(plugin->sync);
                if (retVal < 0) {
                    safuLogWarnF("close (sync) for plugin id:%d failed with: %s", plugin->id, strerror(errno));
                    result = SAFU_RESULT_FAILED;
                }
            }

            if (plugin->worker.sync != -1) {
                retVal = close(plugin->worker.sync);
                if (retVal < 0) {
                    safuLogWarnF("close (worker.sync) for plugin id:%d failed with: %s", plugin->id, strerror(errno));
                    result = SAFU_RESULT_FAILED;
                }
            }

            _funcTableDeleteMembers(plugin);

            memset(plugin, 0, sizeof(elosPlugin_t));
        }
    }

    return result;
}

safuResultE_t elosPluginControlDelete(elosPlugin_t **plugin) {
    safuResultE_t result = SAFU_RESULT_OK;

    if ((plugin != NULL) && (*plugin != NULL)) {
        result = elosPluginControlDeleteMembers(*plugin);
        free(*plugin);
        *plugin = NULL;
    }

    return result;
}

safuResultE_t elosPluginControlGetName(const elosPlugin_t *plugin, const char **name) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin != NULL && plugin->config != NULL && name != NULL) {
        *name = plugin->config->key;
        result = SAFU_RESULT_OK;
    } else {
        safuLogErr("Invalid parameters");
    }

    return result;
}

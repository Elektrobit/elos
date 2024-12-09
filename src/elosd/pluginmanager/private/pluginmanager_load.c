// SPDX-License-Identifier: MIT

#include <elos/libelosplugin/libelosplugin.h>
#include <safu/log.h>
#include <safu/result.h>
#include <safu/vector.h>
#include <samconf/samconf.h>
#include <samconf/samconf_types.h>
#include <sys/time.h>
#include <unistd.h>

#include "elos/plugincontrol/plugincontrol.h"
#include "elos/pluginmanager/pluginmanager.h"

static safuResultE_t _loadPluginList(elosPluginManager_t *pluginManager, samconfConfig_t const *pluginConfig,
                                     elosPluginControlParam_t pluginParam,
                                     elosPluginControlPtrVector_t *controlPtrVector);

safuResultE_t elosPluginManagerLoad(elosPluginManager_t *pluginManager, elosPluginTypeE_t type,
                                    samconfConfig_t const *moduleConfig, char const *pluginSearchPath,
                                    elosPluginControlPtrVector_t *controlPtrVector) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    samconfConfig_t const *pluginConfig = NULL;

    if ((pluginManager == NULL) || (pluginSearchPath == NULL) || (moduleConfig == NULL) || (controlPtrVector == NULL)) {
        safuLogErr("NULL-Pointer has been passed as parameter");
    } else if (pluginManager->state != PLUGINMANAGER_STATE_INITIALIZED) {
        safuLogErr("The given pluginManager struct is not in state 'INITIALIZED'");
    } else {
        samconfConfigStatusE_t status;
        result = SAFU_RESULT_OK;

        status = samconfConfigGet(moduleConfig, "Plugins", &pluginConfig);
        if (status == SAMCONF_CONFIG_NOT_FOUND) {
            safuLogDebugF("%s configuration is missing a 'Plugins' list", elosPluginTypeToStr(type));
        } else if (status != SAMCONF_CONFIG_OK) {
            safuLogErrF("%s configuration failed", elosPluginTypeToStr(type));
            result = SAFU_RESULT_FAILED;
        } else if (pluginConfig->type != SAMCONF_CONFIG_VALUE_OBJECT) {
            safuLogErrF("%s configuration is not in a valid format", elosPluginTypeToStr(type));
            result = SAFU_RESULT_FAILED;
        } else {
            elosPluginControlParam_t pluginParam = {
                .pluginType = type,
                .path = pluginSearchPath,
                .eventProcessor = pluginManager->eventProcessor,
                .eventDispatcher = pluginManager->eventDispatcher,
                .logAggregator = pluginManager->logAggregator,
                .useEnv = pluginManager->useEnv,
            };
            result = _loadPluginList(pluginManager, pluginConfig, pluginParam, controlPtrVector);
            if (result != SAFU_RESULT_OK) {
                safuLogDebug("error in loading plugin list");
            }
        }
    }
    return result;
}

static safuResultE_t _loadPluginList(elosPluginManager_t *pluginManager, samconfConfig_t const *pluginConfig,
                                     elosPluginControlParam_t pluginParam,
                                     elosPluginControlPtrVector_t *controlPtrVector) {
    safuResultE_t result = SAFU_RESULT_OK;
    for (size_t i = 0; i < pluginConfig->childCount; i += 1) {
        char const *key = pluginConfig->children[i]->key;
        safuResultE_t iterResult = SAFU_RESULT_FAILED;
        elosPluginId_t pluginId = ELOS_ID_INVALID;
        samconfConfigStatusE_t iterStatus;
        bool hasTrigger = false;

        iterStatus = samconfConfigGet(pluginConfig, key, &pluginParam.config);
        if (iterStatus != SAMCONF_CONFIG_OK) {
            safuLogErrF("samconfConfigGet '%s' failed : status=%d", key, iterStatus);
        } else {
            char const *runStr = NULL;
            iterStatus = samconfConfigGetString(pluginParam.config, "Run", &runStr);
            if (iterStatus != SAMCONF_CONFIG_OK) {
                safuLogErrF("samconfConfigGet '%s/Run' failed : status=%d", key, iterStatus);
            } else {
                if (strcmp("trigger", runStr) == 0) {
                    hasTrigger = true;
                } else if (strcmp("never", runStr) == 0) {
                    continue;
                } else if (strcmp("always", runStr) != 0) {
                    iterStatus = SAMCONF_CONFIG_ERROR;
                }
            }
        }

        if (iterStatus != SAMCONF_CONFIG_OK) {
            result = SAFU_RESULT_FAILED;
            continue;
        }

        iterResult = elosPluginManagerEntryAdd(pluginManager, &pluginParam, &pluginId);
        if (iterResult != SAFU_RESULT_OK) {
            safuLogErr("elosPluginManagerEntryAdd failed");
        } else {
            elosPluginControl_t *plugin = NULL;

            iterResult = elosPluginManagerEntryGet(pluginManager, pluginId, &plugin);
            if (iterResult != SAFU_RESULT_OK) {
                safuLogErr("elosPluginManagerEntryGet failed");
            } else {
                iterResult = elosPluginControlLoad(plugin);
                if (iterResult != SAFU_RESULT_OK) {
                    safuLogErr("elosPluginControlLoad failed");
                } else {
                    if (hasTrigger == false) {
                        iterResult = elosPluginControlStart(plugin);
                        if (iterResult != SAFU_RESULT_OK) {
                            safuLogErr("elosPluginControlStart failed");
                        }
                    }

                    if (iterResult == SAFU_RESULT_OK) {
                        int retVal;

                        retVal = safuVecPush(controlPtrVector, &plugin);
                        if (retVal < 0) {
                            safuLogErr("safuVecPush failed");
                            iterResult = SAFU_RESULT_FAILED;
                        }
                    }
                }
            }
        }

        if (iterResult != SAFU_RESULT_OK) {
            result = SAFU_RESULT_FAILED;
        }
    }
    return result;
}

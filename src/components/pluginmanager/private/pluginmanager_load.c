// SPDX-License-Identifier: MIT

#include <safu/log.h>
#include <safu/vector.h>
#include <samconf/samconf.h>
#include <sys/time.h>
#include <unistd.h>

#include "elos/plugincontrol/plugincontrol.h"
#include "elos/pluginmanager/pluginmanager.h"

safuResultE_t elosPluginManagerLoad(elosPluginManager_t *pluginManager, samconfConfig_t const *moduleConfig,
                                    char const *pluginSearchPath, elosPluginPtrVector_t *pluginPtrVector) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    samconfConfig_t const *pluginConfig = NULL;
    elosPluginParam_t pluginParam = {0};

    if ((pluginManager == NULL) || (pluginSearchPath == NULL) || (moduleConfig == NULL) || (pluginPtrVector == NULL)) {
        safuLogErr("NULL-Pointer has been passed as parameter");
    } else if (pluginManager->state != PLUGINMANAGER_STATE_INITIALIZED) {
        safuLogErr("The given pluginManager struct is not in state 'INITIALIZED'");
    } else {
        samconfConfigStatusE_t status;

        status = samconfConfigGet(moduleConfig, "Plugins", &pluginConfig);
        if (status != SAMCONF_CONFIG_OK) {
            safuLogErrF("samconfConfigGet 'Plugins' failed : status=%d", status);
        } else {
            pluginParam.path = pluginSearchPath;
            result = SAFU_RESULT_OK;
        }
    }

    if (result == SAFU_RESULT_OK) {
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
                elosPlugin_t *plugin = NULL;

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

                            retVal = safuVecPush(pluginPtrVector, &plugin);
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
    }

    return result;
}

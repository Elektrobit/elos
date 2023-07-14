// SPDX-License-Identifier: MIT

#include <safu/log.h>
#include <sys/time.h>
#include <unistd.h>

#include "elos/plugin/vector.h"
#include "elos/pluginmanager/pluginmanager.h"

safuResultE_t elosPluginManagerEntryAdd(elosPluginManager_t *pluginManager, elosPluginParam_t const *param,
                                        elosPluginId_t *id) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((pluginManager == NULL) || (param == NULL)) {
        safuLogErr("Invalid parameter");
    } else if (pluginManager->state != PLUGINMANAGER_STATE_INITIALIZED) {
        safuLogErr("The given pluginManager struct is not in state 'INITIALIZED'");
    } else {
        elosPluginParam_t pluginParam = *param;
        elosPlugin_t plugin = {0};

        pluginParam.id = pluginManager->nextId;

        result = elosPluginInitialize(&plugin, &pluginParam);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosPluginInitialize failed");
        } else {
            result = elosPluginVectorPush(&pluginManager->pluginVector, &plugin);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("elosPluginInitialize failed");
            } else {
                if (id != NULL) {
                    *id = plugin.id;
                }

                pluginManager->nextId += 1;
            }
        }
    }

    return result;
}

safuResultE_t elosPluginManagerEntryGet(elosPluginManager_t *pluginManager, elosPluginId_t id, elosPlugin_t **plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((pluginManager == NULL) || (id == ELOS_ID_INVALID) || (plugin == NULL)) {
        safuLogErr("Invalid parameter");
    } else if (pluginManager->state != PLUGINMANAGER_STATE_INITIALIZED) {
        safuLogErr("The given pluginManager struct is not in state 'INITIALIZED'");
    } else {
        result = elosPluginVectorGetById(&pluginManager->pluginVector, id, plugin);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosPluginVectorGetById failed");
        }
    }

    return result;
}

safuResultE_t elosPluginManagerEntryLoad(elosPluginManager_t *pluginManager, elosPluginId_t id) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((pluginManager == NULL) || (id == ELOS_ID_INVALID)) {
        safuLogErr("Invalid parameter");
    } else if (pluginManager->state != PLUGINMANAGER_STATE_INITIALIZED) {
        safuLogErr("The given pluginManager struct is not in state 'INITIALIZED'");
    } else {
        elosPlugin_t *plugin = NULL;

        result = elosPluginVectorGetById(&pluginManager->pluginVector, id, &plugin);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosPluginVectorGetById failed");
        } else {
            result = elosPluginLoad(plugin);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("elosPluginLoad");
            }
        }
    }

    return result;
}

safuResultE_t elosPluginManagerEntryUnload(elosPluginManager_t *pluginManager, elosPluginId_t id) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((pluginManager == NULL) || (id == ELOS_ID_INVALID)) {
        safuLogErr("Invalid parameter");
    } else if (pluginManager->state != PLUGINMANAGER_STATE_INITIALIZED) {
        safuLogErr("The given pluginManager struct is not in state 'INITIALIZED'");
    } else {
        elosPlugin_t *plugin = NULL;

        result = elosPluginVectorGetById(&pluginManager->pluginVector, id, &plugin);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosPluginVectorGetById failed");
        } else {
            result = elosPluginUnload(plugin);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("elosPluginUnload failed");
            }
        }
    }

    return result;
}

safuResultE_t elosPluginManagerEntryStart(elosPluginManager_t *pluginManager, elosPluginId_t id) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((pluginManager == NULL) || (id == ELOS_ID_INVALID)) {
        safuLogErr("Invalid parameter");
    } else if (pluginManager->state != PLUGINMANAGER_STATE_INITIALIZED) {
        safuLogErr("The given pluginManager struct is not in state 'INITIALIZED'");
    } else {
        elosPlugin_t *plugin = NULL;

        result = elosPluginVectorGetById(&pluginManager->pluginVector, id, &plugin);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosPluginVectorGetById failed");
        } else {
            result = elosPluginStart(plugin);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("elosPluginStart");
            }
        }
    }

    return result;
}

safuResultE_t elosPluginManagerEntryStop(elosPluginManager_t *pluginManager, elosPluginId_t id) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((pluginManager == NULL) || (id == ELOS_ID_INVALID)) {
        safuLogErr("Invalid parameter");
    } else if (pluginManager->state != PLUGINMANAGER_STATE_INITIALIZED) {
        safuLogErr("The given pluginManager struct is not in state 'INITIALIZED'");
    } else {
        elosPlugin_t *plugin = NULL;

        result = elosPluginVectorGetById(&pluginManager->pluginVector, id, &plugin);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosPluginVectorGetById failed");
        } else {
            result = elosPluginStop(plugin);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("elosPluginStop");
            }
        }
    }

    return result;
}

safuResultE_t elosPluginManagerEntryRemove(elosPluginManager_t *pluginManager, elosPluginId_t pluginId) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((pluginManager == NULL) || (pluginId == ELOS_ID_INVALID)) {
        safuLogErr("Invalid parameter");
    } else if (pluginManager->state != PLUGINMANAGER_STATE_INITIALIZED) {
        safuLogErr("The given pluginManager struct is not in state 'INITIALIZED'");
    } else {
        result = elosPluginVectorRemoveById(&pluginManager->pluginVector, pluginId);
        if (result != SAFU_RESULT_OK) {
            safuLogErrF("elosPluginVectorRemoveById failed (pluginId: %u)", pluginId);
        }
    }

    return result;
}

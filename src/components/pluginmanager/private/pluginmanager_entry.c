// SPDX-License-Identifier: MIT

#include <safu/log.h>
#include <sys/time.h>
#include <unistd.h>

#include "elos/plugincontrol/plugincontrol.h"
#include "elos/plugincontrol/vector.h"
#include "elos/pluginmanager/pluginmanager.h"

safuResultE_t elosPluginManagerEntryAdd(elosPluginManager_t *pluginManager, elosPluginControlParam_t const *param,
                                        elosPluginId_t *id) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((pluginManager == NULL) || (param == NULL)) {
        safuLogErr("Invalid parameter");
    } else if (pluginManager->state != PLUGINMANAGER_STATE_INITIALIZED) {
        safuLogErr("The given pluginManager struct is not in state 'INITIALIZED'");
    } else {
        elosPluginControlParam_t pluginParam = *param;
        pluginParam.id = pluginManager->nextId;

        elosPluginControl_t *plugin = NULL;
        result = elosPluginControlNew(&plugin, &pluginParam);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosPluginControlInitialize failed");
        } else {
            result = elosPluginControlPtrVectorPush(&pluginManager->pluginPtrVector, plugin);
            if (result != SAFU_RESULT_OK) {
                result = elosPluginControlDeleteMembers(plugin);
                if (result != SAFU_RESULT_OK) {
                    safuLogErr("elosPluginControlDeleteMembers failed, possible memory leak");
                }
                safuLogErr("elosPluginControlInitialize failed");
            } else {
                if (id != NULL) {
                    *id = plugin->context.id;
                }

                pluginManager->nextId += 1;
            }
        }
    }

    return result;
}

safuResultE_t elosPluginManagerEntryGet(elosPluginManager_t *pluginManager, elosPluginId_t id,
                                        elosPluginControl_t **plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((pluginManager == NULL) || (id == ELOS_ID_INVALID) || (plugin == NULL)) {
        safuLogErr("Invalid parameter");
    } else if (pluginManager->state != PLUGINMANAGER_STATE_INITIALIZED) {
        safuLogErr("The given pluginManager struct is not in state 'INITIALIZED'");
    } else {
        result = elosPluginControlPtrVectorGetById(&pluginManager->pluginPtrVector, id, plugin);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosPluginControlPtrVectorGetById failed");
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
        elosPluginControl_t *plugin = NULL;

        result = elosPluginControlPtrVectorGetById(&pluginManager->pluginPtrVector, id, &plugin);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosPluginControlPtrVectorGetById failed");
        } else {
            result = elosPluginControlLoad(plugin);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("elosPluginControlLoad");
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
        elosPluginControl_t *plugin = NULL;

        result = elosPluginControlPtrVectorGetById(&pluginManager->pluginPtrVector, id, &plugin);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosPluginControlPtrVectorGetById failed");
        } else {
            result = elosPluginControlUnload(plugin);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("elosPluginControlUnload failed");
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
        elosPluginControl_t *plugin = NULL;

        result = elosPluginControlPtrVectorGetById(&pluginManager->pluginPtrVector, id, &plugin);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosPluginControlPtrVectorGetById failed");
        } else {
            result = elosPluginControlStart(plugin);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("elosPluginControlStart");
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
        elosPluginControl_t *plugin = NULL;

        result = elosPluginControlPtrVectorGetById(&pluginManager->pluginPtrVector, id, &plugin);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosPluginControlPtrVectorGetById failed");
        } else {
            result = elosPluginControlStop(plugin);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("elosPluginControlStop");
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
        result = elosPluginControlPtrVectorRemoveById(&pluginManager->pluginPtrVector, pluginId);
        if (result != SAFU_RESULT_OK) {
            safuLogErrF("elosPluginControlPtrVectorRemoveById failed (pluginId: %u)", pluginId);
        }
    }

    return result;
}

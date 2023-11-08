// SPDX-License-Identifier: MIT

#include <safu/common.h>
#include <safu/log.h>
#include <safu/vector.h>

#include "elos/plugincontrol/vector.h"

safuResultE_t elosPluginVectorInitialize(elosPluginVector_t *pluginVector, size_t elements) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (pluginVector == NULL) {
        safuLogErr("Null parameter given");
    } else {
        int retVal;

        retVal = safuVecCreate(pluginVector, elements, sizeof(elosPlugin_t));
        if (retVal < 0) {
            safuLogErr("safuVecCreate failed");
        } else {
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

safuResultE_t elosPluginVectorPush(elosPluginVector_t *pluginVector, elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((pluginVector == NULL) || (plugin == NULL)) {
        safuLogErr("Null parameter given");
    } else {
        int retVal;

        retVal = safuVecPush(pluginVector, plugin);
        if (retVal < 0) {
            safuLogErr("safuVecPush failed");
        } else {
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

safuResultE_t elosPluginVectorIterate(elosPluginVector_t *pluginVector, safuVecFunc_t *func, void *data,
                                      int *iterResult) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((pluginVector == NULL) || (func == NULL)) {
        safuLogErr("Null parameter given");
    } else {
        int retVal;

        retVal = safuVecIterate(pluginVector, func, data);
        if (retVal < 0) {
            safuLogWarn("safuVecIterate failed");
        } else {
            result = SAFU_RESULT_OK;
        }

        if (iterResult != NULL) {
            *iterResult = retVal;
        }
    }

    return result;
}

static int _pluginVectorMatchById(const void *element, const void *data) {
    elosPlugin_t const *plugin = (elosPlugin_t const *)element;
    elosPluginId_t const id = *(elosPluginId_t const *)data;
    int result = 0;

    if (plugin->id == id) {
        result = 1;
    }

    return result;
}

safuResultE_t elosPluginVectorGetById(elosPluginVector_t *pluginVector, elosPluginId_t id, elosPlugin_t **plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((pluginVector == NULL) || (id == ELOS_ID_INVALID) || (plugin == NULL)) {
        safuLogErr("Null parameter given");
    } else {
        int retVal;

        retVal = safuVecFindGet(pluginVector, (void **)plugin, NULL, _pluginVectorMatchById, &id);
        if (retVal < 0) {
            safuLogErr("safuVecFindGet failed");
        } else if (retVal == 0) {
            safuLogErrF("safuVecFindGet couldn't find plugin with id '%d' in vector", id);
        } else {
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

static int _pluginVectorRemoveById(const void *element, const void *data) {
    elosPlugin_t *plugin = (elosPlugin_t *)element;
    elosPluginId_t const id = *(elosPluginId_t const *)data;
    int result = 0;

    if (plugin->id == id) {
        result = elosPluginControlDeleteMembers(plugin);
        if (result < 0) {
            safuLogWarn("elosPluginControlDeleteMembers failed (likely creating a memory leak)");
        }

        result = 1;
    }

    return result;
}

safuResultE_t elosPluginVectorRemoveById(elosPluginVector_t *pluginVector, elosPluginId_t id) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((pluginVector == NULL) || (id == ELOS_ID_INVALID)) {
        safuLogErr("Null parameter given");
    } else {
        int retVal;

        retVal = safuVecFindRemove(pluginVector, _pluginVectorRemoveById, &id);
        if (retVal < 0) {
            safuLogErr("safuVecFindRemove failed");
        } else if (retVal == 0) {
            safuLogErrF("safuVecFindRemove couldn't find plugin with id '%d' in vector", id);
        } else {
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

static int _deleteMemberfunc(void const *element, UNUSED void const *data) {
    safuResultE_t result;
    elosPlugin_t *plugin = (elosPlugin_t *)element;

    result = elosPluginControlDeleteMembers(plugin);
    if (result < 0) {
        safuLogWarn("elosPluginVectorDeleteMembers failed (likely creating a memory leak)");
    }

    return 0;
}

safuResultE_t elosPluginVectorDeleteMembers(elosPluginVector_t *pluginVector) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (pluginVector == NULL) {
        safuLogErr("Null parameter given");
    } else {
        result = elosPluginVectorIterate(pluginVector, _deleteMemberfunc, NULL, NULL);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErr("elosPluginVectorIterate failed");
        } else {
            int retVal;

            retVal = safuVecFree(pluginVector);
            if (retVal < 0) {
                safuLogErr("safuVecCreate failed");
            } else {
                result = SAFU_RESULT_OK;
            }
        }
    }

    return result;
}

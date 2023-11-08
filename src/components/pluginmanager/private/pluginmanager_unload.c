// SPDX-License-Identifier: MIT

#include <safu/common.h>
#include <safu/log.h>
#include <sys/time.h>
#include <unistd.h>

#include "elos/plugin/vector.h"
#include "elos/pluginmanager/pluginmanager.h"

static int _pluginManagerUnloadHelper(void const *element, UNUSED void const *data) {
    elosPluginManager_t *pluginManager = (elosPluginManager_t *)data;
    elosPlugin_t *plugin = *(elosPlugin_t **)element;
    safuResultE_t result;

    result = elosPluginManagerEntryUnload(pluginManager, plugin->context.id);
    if (result != SAFU_RESULT_OK) {
        safuLogWarnF("Unloading pluginId '%d' failed (likely resulting in a memory leak)", plugin->context.id);
    }

    return 0;
}

safuResultE_t elosPluginManagerUnload(elosPluginManager_t *pluginManager, elosPluginPtrVector_t *pluginPtrVector) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((pluginManager == NULL) || (pluginPtrVector == NULL)) {
        safuLogErr("NULL-Pointer has been passed as parameter");
    } else if (pluginManager->state != PLUGINMANAGER_STATE_INITIALIZED) {
        safuLogErr("The given pluginManager struct is not in state 'INITIALIZED'");
    } else {
        result = elosPluginVectorIterate(pluginPtrVector, _pluginManagerUnloadHelper, pluginManager, NULL);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErr("elosPluginVectorIterate failed");
        }
    }

    return result;
}

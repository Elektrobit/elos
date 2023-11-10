// SPDX-License-Identifier: MIT

#include <safu/log.h>
#include <safu/vector.h>
#include <sys/time.h>
#include <unistd.h>

#include "elos/plugincontrol/plugincontrol.h"
#include "elos/pluginmanager/pluginmanager.h"

safuResultE_t elosPluginManagerRemove(elosPluginManager_t *pluginManager,
                                      elosPluginControlPtrVector_t *controlPtrVector) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((pluginManager == NULL) || (controlPtrVector == NULL)) {
        safuLogErr("NULL-Pointer has been passed as parameter");
    } else if (pluginManager->state != PLUGINMANAGER_STATE_INITIALIZED) {
        safuLogErr("The given pluginManager struct is not in state 'INITIALIZED'");
    } else {
        elosPluginControlVector_t *pluginVector = &pluginManager->pluginVector;
        ssize_t const elements = safuVecElements(pluginVector);

        result = SAFU_RESULT_OK;

        for (ssize_t i = 0; i < elements; i += 1) {
            safuResultE_t resIter = SAFU_RESULT_OK;
            elosPluginControl_t *plugin;

            plugin = *(elosPluginControl_t **)safuVecGetLast(controlPtrVector);
            if (plugin == NULL) {
                safuLogErr("safuVecGetLast failed");
                resIter = SAFU_RESULT_FAILED;
            } else {
                int retVal;

                resIter = elosPluginManagerEntryRemove(pluginManager, plugin->context.id);
                if (resIter != SAFU_RESULT_OK) {
                    safuLogErrF("elosPluginManagerEntryRemove failed (plugin->id: %d)", plugin->context.id);
                }

                retVal = safuVecPop(controlPtrVector);
                if (retVal < 0) {
                    safuLogErr("safuVecPop failed");
                    resIter = SAFU_RESULT_FAILED;
                }
            }

            if (resIter != SAFU_RESULT_OK) {
                safuLogWarnF("A least one step failed in iteration %ld (likely resulting in a memory leak)", i);
                result = SAFU_RESULT_FAILED;
            }
        }
    }

    return result;
}

// SPDX-License-Identifier: MIT
#include "elos/eventlogging/PluginFilterLoader.h"

#include <safu/log.h>
#include <samconf/samconf.h>

#include "elos/eventfilter/eventfilter.h"
#include "elos/eventfilter/vector.h"
#include "elos/eventlogging/StorageBackend.h"

safuResultE_t elosPluginFilterLoaderLoad(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    samconfConfigStatusE_t configResult = SAMCONF_CONFIG_ERROR;
    elosRpnFilterResultE_t filterResult = RPNFILTER_RESULT_ERROR;
    elosStorageBackend_t *backend = NULL;
    const samconfConfig_t *filterConfig = NULL;
    const char *pluginName = NULL;

    if (plugin != NULL && plugin->context.config != NULL && plugin->context.data != NULL) {
        elosPluginControlGetName(plugin, &pluginName);
        size_t filterCount = 0;

        configResult = samconfConfigGet(plugin->context.config, "Filter", &filterConfig);
        if (configResult == SAMCONF_CONFIG_OK) {
            result = SAFU_RESULT_OK;
            filterCount = filterConfig->childCount;
        } else if (configResult == SAMCONF_CONFIG_NOT_FOUND) {
            safuLogWarnF("No filter rules for %s\n", pluginName);
            result = SAFU_RESULT_FAILED;
        } else {
            safuLogErrF("Failed to load filter rules for %s\n", pluginName);
            result = SAFU_RESULT_FAILED;
        }

        backend = (elosStorageBackend_t *)plugin->context.data;
        filterResult = elosEventFilterVectorInit(&backend->filter, filterCount);
        if (filterResult != RPNFILTER_RESULT_OK) {
            safuLogErr("Failed to to create filter vector");
            result = SAFU_RESULT_FAILED;
        }
    } else {
        safuLogErr("Invalid parameter");
        result = SAFU_RESULT_FAILED;
    }

    if (result == SAFU_RESULT_OK) {
        safuLogDebugF("Found %zu filter for %s", filterConfig->childCount, pluginName);
        for (size_t i = 0; i < filterConfig->childCount; i++) {
            elosEventFilter_t eventFilter = {0};

            if (filterConfig->children[i]->type == SAMCONF_CONFIG_VALUE_STRING) {
                elosEventFilterParam_t const eventFilterParam = {.filterString =
                                                                     filterConfig->children[i]->value.string};

                safuLogDebugF("Create filter for %s", eventFilterParam.filterString);
                filterResult = elosEventFilterCreate(&eventFilter, &eventFilterParam);
                if (filterResult != RPNFILTER_RESULT_OK) {
                    safuLogErrF("elosEventFilterCreate failed (%d)", filterResult);
                    result = SAFU_RESULT_FAILED;
                } else {
                    result = SAFU_RESULT_OK;
                }
            } else {
                safuLogErrF("Failed to fetch filter %zu. string form config", i);
                result = SAFU_RESULT_FAILED;
            }

            if (result == SAFU_RESULT_OK) {
                filterResult = elosEventFilterVectorPush(&backend->filter, &eventFilter);
                if (filterResult != RPNFILTER_RESULT_OK) {
                    safuLogErrF("%s", "elosEventFilterVectorPush failed");
                    result = SAFU_RESULT_FAILED;
                }
            }
        }
    }

    return result;
}

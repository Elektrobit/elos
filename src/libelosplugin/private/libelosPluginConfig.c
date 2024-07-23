// SPDX-License-Identifier: MIT

#include "elos/libelosplugin/libelosPluginConfig.h"

#include <errno.h>
#include <safu/common.h>
#include <stdlib.h>

#include "elos/libelosplugin/types.h"

int32_t elosPluginConfigGetInt(elosPlugin_t const *plugin, const char *pathToConfigOption, const char *envVarName,
                               int32_t defaultValue) {
    bool validValNotPresent = true;
    int32_t optionsFromFile = 0;
    int32_t result = defaultValue;
    samconfConfigStatusE_t exists = {0};

    if (plugin->useEnv) {
        const char *getEnvValue = safuGetEnvOr(envVarName, NULL);
        char *endPtr;
        if (getEnvValue) {
            errno = 0;
            result = strtol(getEnvValue, &endPtr, 10);
            validValNotPresent = endPtr == getEnvValue || errno == ERANGE || errno == EINVAL;
        }
    }

    if (validValNotPresent) {
        exists = samconfConfigGetInt32(plugin->config, pathToConfigOption, &optionsFromFile);
        result = exists == SAMCONF_CONFIG_OK ? optionsFromFile : defaultValue;
    }
    return result;
}

const char *elosPluginConfigGetString(elosPlugin_t const *plugin, const char *pathToConfigOption,
                                      const char *envVarName, const char *defaultValue) {
    const char *optionsFromFile = NULL;
    const char *result = NULL;

    samconfConfigGetString(plugin->config, pathToConfigOption, &optionsFromFile);

    if (plugin->useEnv) {
        result = safuGetEnvOr(envVarName, optionsFromFile ? optionsFromFile : defaultValue);
    } else {
        result = optionsFromFile ? optionsFromFile : defaultValue;
    }
    return result;
}

// SPDX-License-Identifier: MIT

#include "elos/libelosplugin/libelosPluginConfig.h"

#include <errno.h>
#include <safu/common.h>
#include <samconf/samconf_types.h>
#include <stdlib.h>

#include "elos/libelosplugin/types.h"

int32_t elosPluginConfigGetInt(elosPlugin_t const *plugin, const char *pathToConfigOption,
                               UNUSED const char *envVarName, int32_t defaultValue) {
    return samconfConfigGetInt32Or(plugin->config, pathToConfigOption, defaultValue);
}

const char *elosPluginConfigGetString(elosPlugin_t const *plugin, const char *pathToConfigOption,
                                      UNUSED const char *envVarName, const char *defaultValue) {
    return samconfConfigGetStringOr(plugin->config, pathToConfigOption, defaultValue);
}

// SPDX-License-Identifier: MIT

#pragma once

#include <samconf/samconf.h>

#include "elos/libelosplugin/types.h"

/**********************************************************************************
 * This component provides common utility functions to handle plugin configuration.
 *
 **********************************************************************************/

/**********************************************************************************
 * Get integer value for given plugin option path.
 * I.e. `/Config/anIntegerOption`
 *
 * Note: Actually it is the task of samconf to merge different configuration sources, but until support for
 * `environment` is implemented, it must be done here manually.
 *
 * Parameters:
 *
 **********************************************************************************/
int32_t elosPluginConfigGetInt(elosPlugin_t const *plugin, const char *pathToConfigOption, const char *envVarName,
                               int32_t defaultValue);

const char *elosPluginConfigGetString(elosPlugin_t const *plugin, const char *pathToConfigOption,
                                      const char *envVarName, const char *defaultValue);

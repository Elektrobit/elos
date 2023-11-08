// SPDX-License-Identifier: MIT
#pragma once

#include "elos/plugincontrol/types.h"

#define ELOS_FUNCTABLEENTRIES 4

typedef struct elosFuncTableEntry {
    char const *name;
    void **func;
} elosFuncTableEntry_t;

safuResultE_t elosPluginControlLoadHelperSearchFile(elosPlugin_t const *plugin, char **file);
safuResultE_t elosPluginControlLoadHelperGetFunctions(elosPlugin_t *plugin);

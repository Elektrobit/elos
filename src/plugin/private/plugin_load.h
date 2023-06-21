// SPDX-License-Identifier: MIT
#ifndef __ELOS_PLUGIN_PLUGINLOAD_H__
#define __ELOS_PLUGIN_PLUGINLOAD_H__

#include "elos/plugin/types.h"

#define ELOS_FUNCTABLEENTRIES 4

typedef struct elosFuncTableEntry {
    char const *name;
    void **func;
} elosFuncTableEntry_t;

safuResultE_t elosPluginLoadHelperSearchFile(elosPlugin_t const *plugin, char **file);
safuResultE_t elosPluginLoadHelperGetFunctions(elosPlugin_t *plugin);

#endif /* __ELOS_PLUGIN_PLUGINLOAD_H__ */

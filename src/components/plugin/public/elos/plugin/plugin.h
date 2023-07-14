// SPDX-License-Identifier: MIT
#ifndef __ELOS_PLUGIN_PLUGIN_H__
#define __ELOS_PLUGIN_PLUGIN_H__

#include "elos/plugin/types.h"

safuResultE_t elosPluginInitialize(elosPlugin_t *plugin, elosPluginParam_t const *param);

safuResultE_t elosPluginNew(elosPlugin_t **plugin, elosPluginParam_t const *param);

safuResultE_t elosPluginLoad(elosPlugin_t *plugin);

safuResultE_t elosPluginUnload(elosPlugin_t *plugin);

safuResultE_t elosPluginStart(elosPlugin_t *plugin);

safuResultE_t elosPluginStop(elosPlugin_t *plugin);

safuResultE_t elosPluginDeleteMembers(elosPlugin_t *plugin);

safuResultE_t elosPluginGetName(const elosPlugin_t *plugin, const char **name);

#endif /* __ELOS_PLUGIN_PLUGIN_H__ */

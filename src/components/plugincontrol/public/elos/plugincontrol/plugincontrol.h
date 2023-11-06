// SPDX-License-Identifier: MIT
#pragma once

#include "elos/plugincontrol/types.h"

safuResultE_t elosPluginControlInitialize(elosPlugin_t *plugin, elosPluginParam_t const *param);

safuResultE_t elosPluginControlNew(elosPlugin_t **plugin, elosPluginParam_t const *param);

safuResultE_t elosPluginControlLoad(elosPlugin_t *plugin);

safuResultE_t elosPluginControlUnload(elosPlugin_t *plugin);

safuResultE_t elosPluginControlStart(elosPlugin_t *plugin);

safuResultE_t elosPluginControlStop(elosPlugin_t *plugin);

safuResultE_t elosPluginControlDeleteMembers(elosPlugin_t *plugin);

safuResultE_t elosPluginControlDelete(elosPlugin_t **plugin);

safuResultE_t elosPluginControlGetName(const elosPlugin_t *plugin, const char **name);

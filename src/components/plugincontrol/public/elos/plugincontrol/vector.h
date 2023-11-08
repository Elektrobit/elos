// SPDX-License-Identifier: MIT
#pragma once

#include "elos/plugincontrol/plugincontrol.h"
#include "safu/vector.h"

safuResultE_t elosPluginVectorInitialize(elosPluginVector_t *plugin, size_t elements);

safuResultE_t elosPluginVectorPush(elosPluginVector_t *pluginVector, elosPlugin_t *plugin);

safuResultE_t elosPluginVectorIterate(elosPluginVector_t *pluginVector, safuVecFunc_t *func, void *data,
                                      int *iterResult);

safuResultE_t elosPluginVectorGetById(elosPluginVector_t *pluginVector, elosPluginId_t id, elosPlugin_t **plugin);

safuResultE_t elosPluginVectorRemoveById(elosPluginVector_t *pluginVector, elosPluginId_t id);

safuResultE_t elosPluginVectorDeleteMembers(elosPluginVector_t *plugin);

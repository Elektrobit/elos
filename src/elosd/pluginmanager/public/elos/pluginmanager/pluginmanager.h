// SPDX-License-Identifier: MIT
#ifndef __ELOS_PLUGINMANAGER_PLUGINMANAGER_H__
#define __ELOS_PLUGINMANAGER_PLUGINMANAGER_H__

#include <safu/result.h>

#include "elos/pluginmanager/types.h"

__BEGIN_DECLS

safuResultE_t elosPluginManagerInitialize(elosPluginManager_t *pluginManager, elosPluginManagerParam_t const *param);

safuResultE_t elosPluginManagerLoad(elosPluginManager_t *pluginManager, elosPluginTypeE_t type,
                                    samconfConfig_t const *pluginConfig, char const *pluginSearchPath,
                                    elosPluginControlPtrVector_t *pluginPtrVector);

safuResultE_t elosPluginManagerUnload(elosPluginManager_t *pluginManager,
                                      elosPluginControlPtrVector_t *pluginPtrVector);

safuResultE_t elosPluginManagerRemove(elosPluginManager_t *pluginManager,
                                      elosPluginControlPtrVector_t *pluginPtrVector);

safuResultE_t elosPluginManagerEntryAdd(elosPluginManager_t *pluginManager, elosPluginControlParam_t const *param,
                                        elosPluginId_t *id);

safuResultE_t elosPluginManagerEntryGet(elosPluginManager_t *pluginManager, elosPluginId_t id,
                                        elosPluginControl_t **plugin);

safuResultE_t elosPluginManagerEntryLoad(elosPluginManager_t *pluginManager, elosPluginId_t id);

safuResultE_t elosPluginManagerEntryUnload(elosPluginManager_t *pluginManager, elosPluginId_t id);

safuResultE_t elosPluginManagerEntryStart(elosPluginManager_t *pluginManager, elosPluginId_t id);

safuResultE_t elosPluginManagerEntryStop(elosPluginManager_t *pluginManager, elosPluginId_t id);

safuResultE_t elosPluginManagerEntryRemove(elosPluginManager_t *pluginManager, elosPluginId_t id);

safuResultE_t elosPluginManagerDeleteMembers(elosPluginManager_t *pluginManager);

__END_DECLS

#endif /* __ELOS_PLUGINMANAGER_PLUGINMANAGER_H__ */

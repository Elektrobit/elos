// SPDX-License-Identifier: MIT
#pragma once

#include <safu/common.h>
#include <safu/vector.h>
#include <samconf/samconf.h>

#include "elos/clientmanager/clientmanager_types.h"
#include "elos/eventlogging/StorageBackend.h"
#include "elos/pluginmanager/pluginmanager.h"

safuResultE_t elosClientManagerInitialize(elosClientManager_t *clientManager, elosClientManagerParam_t const *param);
safuResultE_t elosClientManagerLoad(elosClientManager_t *clientManager);
safuResultE_t elosClientManagerShutdown(elosClientManager_t *clientManager);

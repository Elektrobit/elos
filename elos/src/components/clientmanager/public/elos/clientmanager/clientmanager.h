// SPDX-License-Identifier: MIT
#pragma once

#include "elos/clientmanager/types.h"

safuResultE_t elosClientManagerInitialize(elosClientManager_t *clientmanager, elosClientManagerParam_t *param);
safuResultE_t elosClientManagerDeleteMembers(elosClientManager_t *clientmanager);
safuResultE_t elosClientManagerStart(elosClientManager_t *clientmanager);
safuResultE_t elosClientManagerStop(elosClientManager_t *clientmanager);

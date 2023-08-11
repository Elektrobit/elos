// SPDX-License-Identifier: MIT
#pragma once

#include "elos/clientmanager/types.h"

safuResultE_t elosClientManagerInitialize(elosClientManager_t *clientmanager, elosClientManagerParam_t *param);
safuResultE_t elosClientManagerDeleteMembers(elosClientManager_t *clientmanager);
int elosClientManagerStart(elosClientManager_t *context);
int elosClientManagerStop(elosClientManager_t *context);

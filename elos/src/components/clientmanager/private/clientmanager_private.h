// SPDX-License-Identifier: MIT
#pragma once

#include <safu/result.h>

#include "elos/clientmanager/clientmanager_types.h"

safuResultE_t elosClientManagerGetStatus(elosClientManagerContext_t *context, uint32_t *status);
safuResultE_t elosClientManagerThreadGetFreeConnectionSlot(elosClientManagerContext_t *ctx, int *slot);
safuResultE_t elosClientManagerThreadWaitForIncomingConnection(elosClientManagerContext_t *ctx, int slot);

void *elosClientManagerThreadListen(void *ptr);
void *elosClientManagerThreadConnection(void *ptr);

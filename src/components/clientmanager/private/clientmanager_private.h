// SPDX-License-Identifier: MIT
#pragma once

#include <safu/result.h>

#include "elos/clientmanager/types.h"

safuResultE_t elosClientManagerThreadGetFreeConnectionSlot(elosClientManager_t *ctx, int *slot);
safuResultE_t elosClientManagerThreadWaitForIncomingConnection(elosClientManager_t *ctx, int slot, int *socketFd);

void *elosClientManagerThreadListen(void *ptr);

#define _SYNCFD_VALUE 1
#define _SYNCFD_SIZE  (int)sizeof(eventfd_t)

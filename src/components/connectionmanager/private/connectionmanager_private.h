// SPDX-License-Identifier: MIT
#pragma once

#include <safu/result.h>

#include "elos/connectionmanager/types.h"

safuResultE_t elosConnectionManagerThreadGetFreeConnectionSlot(elosConnectionManager_t *connectionManager, int *slot);
safuResultE_t elosConnectionManagerThreadWaitForIncomingConnection(elosConnectionManager_t *connectionManager, int slot,
                                                                   int *socketFd);

void *elosConnectionManagerThreadListen(void *ptr);

#define _SYNCFD_VALUE 1
#define _SYNCFD_SIZE  (int)sizeof(eventfd_t)

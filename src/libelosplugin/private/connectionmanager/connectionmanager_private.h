// SPDX-License-Identifier: MIT
#pragma once

#include <elos/libelosplugin/connectionmanager_types.h>
#include <safu/result.h>

safuResultE_t elosConnectionManagerThreadGetFreeConnectionSlot(elosConnectionManager_t *connectionManager, int *slot);
safuResultE_t elosConnectionManagerThreadWaitForIncomingConnection(elosConnectionManager_t *connectionManager,
                                                                   int slot);

void *elosConnectionManagerThreadListen(void *ptr);

#define _SYNCFD_VALUE 1
#define _SYNCFD_SIZE  (int)sizeof(eventfd_t)

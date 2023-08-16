// SPDX-License-Identifier: MIT
#pragma once
#include <safu/result.h>

#include "elos/clientmanager/clientconnection_types.h"

safuResultE_t elosClientConnectionInitialize(elosClientConnection_t *clientConnection,
                                             elosClientConnectionParam_t *param);
safuResultE_t elosClientConnectionStart(elosClientConnection_t *clientConnection, int socketFd);
safuResultE_t elosClientConnectionStop(elosClientConnection_t *clientConnection);
safuResultE_t elosClientConnectionDeleteMembers(elosClientConnection_t *clientConnection);
safuResultE_t elosClientConnectionIsActive(elosClientConnection_t *clientConnection, bool *active);
void *elosClientConnectionWorker(void *ptr);

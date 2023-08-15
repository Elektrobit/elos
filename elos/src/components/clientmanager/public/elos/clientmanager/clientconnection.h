// SPDX-License-Identifier: MIT
#pragma once
#include <safu/result.h>
#include "elos/clientmanager/clientconnection_types.h"

safuResultE_t elosClientConnectionInitialize(elosClientConnection_t *clientConnection,
                                             elosClientConnectionParam_t *param);
safuResultE_t elosClientConnectionStart(elosClientConnection_t *clientConnection);
safuResultE_t elosClientConnectionStop(elosClientConnection_t *clientConnection);
safuResultE_t elosClientConnectionDeleteMembers(elosClientConnection_t *clientConnection);
void *elosClientConnectionWorker(void *ptr);

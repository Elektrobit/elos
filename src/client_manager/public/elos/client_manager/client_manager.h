// SPDX-License-Identifier: MIT
#ifndef ELOS_CLIENT_MANAGER_H_
#define ELOS_CLIENT_MANAGER_H_

#include <stdint.h>

#include "elos/client_manager/types.h"
#include "elos/common/message.h"
#include "elos/config/config.h"
#include "elos/eventfilter/eventfilter.h"
#include "elos/eventlogging/LogAggregator.h"
#include "elos/eventprocessor/eventprocessor.h"
#include "safu/common.h"
#include "safu/mutex.h"

safuResultE_t elosClientManagerGetStatus(elosClientManagerContext_t *context, uint32_t *status);
int elosClientManagerStart(elosClientManagerContext_t *context, elosClientManagerParam_t *parameters);
int elosClientManagerStop(elosClientManagerContext_t *context);

safuResultE_t elosClientManagerThreadGetFreeConnectionSlot(elosClientManagerContext_t *ctx, int *slot);
safuResultE_t elosClientManagerThreadWaitForIncomingConnection(elosClientManagerContext_t *ctx, int slot);

#endif

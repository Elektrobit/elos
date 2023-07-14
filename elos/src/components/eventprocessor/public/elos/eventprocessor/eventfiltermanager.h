// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTPROCESSOR_EVENTFILTERMANAGER_H
#define ELOS_EVENTPROCESSOR_EVENTFILTERMANAGER_H

#include <safu/common.h>
#include <safu/flags.h>
#include <stdint.h>

#include "elos/event/event_types.h"
#include "elos/eventprocessor/types.h"

// Standard interface functions
safuResultE_t elosEventFilterManagerInitialize(elosEventFilterManager_t *efm,
                                               elosEventFilterManagerParam_t const *param);
safuResultE_t elosEventFilterManagerDeleteMembers(elosEventFilterManager_t *efm);

// Create a new EventFilterNode
safuResultE_t elosEventFilterManagerNodeCreate(elosEventFilterManager_t *efm, char const **filterStrings,
                                               size_t filterStringCount, elosEventQueueId_t eqId,
                                               elosEventFilterNodeId_t *efnId);
safuResultE_t elosEventFilterManagerNodeRemove(elosEventFilterManager_t *efm, elosEventFilterNodeId_t efnId);
safuResultE_t elosEventFilterManagerNodeRemoveByEventQueueId(elosEventFilterManager_t *efm, elosEventQueueId_t eqId);
safuResultE_t elosEventFilterManagerProcess(elosEventFilterManager_t *efm, elosEvent_t const *event);

#endif /* ELOS_EVENTPROCESSOR_EVENTFILTERMANAGER_H */

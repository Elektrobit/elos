// SPDX-License-Identifier: MIT
#pragma once

#include <safu/common.h>

#include "elos/event/event.h"
#include "elos/eventprocessor/types.h"

__BEGIN_DECLS

// Standard interface functions
safuResultE_t elosEventQueueManagerInitialize(elosEventQueueManager_t *eqm, elosEventQueueManagerParam_t const *param);
safuResultE_t elosEventQueueManagerDeleteMembers(elosEventQueueManager_t *eqm);

// Functions for interacting with EventQueues
safuResultE_t elosEventQueueManagerEntryNew(elosEventQueueManager_t *eqm, elosEventQueueId_t *eqId);
safuResultE_t elosEventQueueManagerEntryDelete(elosEventQueueManager_t *eqm, elosEventQueueId_t eqId);

safuResultE_t elosEventQueueManagerEntrySetLimit(elosEventQueueManager_t *eqm, elosEventQueueId_t eqId,
                                                 elosEventQueueLimitEvents_t limitEvents,
                                                 elosEventQueueLimitTime_t limitTime);
safuResultE_t elosEventQueueManagerEntryGetLimit(elosEventQueueManager_t *eqm, elosEventQueueId_t eqId,
                                                 elosEventQueueLimitEvents_t *limitEvents,
                                                 elosEventQueueLimitTime_t *limitTime);

safuResultE_t elosEventQueueManagerEntryGet(elosEventQueueManager_t *eqm, elosEventQueueId_t eqId,
                                            elosEventQueue_t **eventQueue);

safuResultE_t elosEventQueueManagerEntryRead(elosEventQueueManager_t *eqm, elosEventQueueId_t eqId,
                                             elosEventVector_t **eventVector);

safuResultE_t elosEventQueueManagerEntryPush(elosEventQueueManager_t *eqm, elosEventQueueId_t eqId, elosEvent_t *event);

__END_DECLS

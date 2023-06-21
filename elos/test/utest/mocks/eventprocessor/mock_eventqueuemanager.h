// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTPROCESSOR_EVENTQUEUEMANAGER_MOCK_H
#define ELOS_EVENTPROCESSOR_EVENTQUEUEMANAGER_MOCK_H

#include <cmocka_extensions/mock_extensions.h>
#ifdef MOCK_EVENTPROCESSOR_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "elos/eventprocessor/eventqueuemanager.h"

MOCK_FUNC_PROTOTYPE(elosEventQueueManagerInitialize, safuResultE_t, elosEventQueueManager_t *eqm,
                    elosEventQueueManagerParam_t const *param)

MOCK_FUNC_PROTOTYPE(elosEventQueueManagerDeleteMembers, safuResultE_t, elosEventQueueManager_t *eqm)

MOCK_FUNC_PROTOTYPE(elosEventQueueManagerEntryNew, safuResultE_t, elosEventQueueManager_t *eqm,
                    elosEventQueueId_t *eqId)

MOCK_FUNC_PROTOTYPE(elosEventQueueManagerEntryDelete, safuResultE_t, elosEventQueueManager_t *eqm,
                    elosEventQueueId_t eqId)

MOCK_FUNC_PROTOTYPE(elosEventQueueManagerEntrySetLimit, safuResultE_t, elosEventQueueManager_t *eqm,
                    elosEventQueueId_t eqId, elosEventQueueLimitEvents_t limitEvents,
                    elosEventQueueLimitTime_t limitTime)

MOCK_FUNC_PROTOTYPE(elosEventQueueManagerEntryGetLimit, safuResultE_t, elosEventQueueManager_t *eqm,
                    elosEventQueueId_t eqId, elosEventQueueLimitEvents_t *limitEvents,
                    elosEventQueueLimitTime_t *limitTime)

MOCK_FUNC_PROTOTYPE(elosEventQueueManagerEntryGet, safuResultE_t, elosEventQueueManager_t *eqm, elosEventQueueId_t eqId,
                    elosEventQueue_t **eventQueue)

MOCK_FUNC_PROTOTYPE(elosEventQueueManagerEntryRead, safuResultE_t, elosEventQueueManager_t *eqm,
                    elosEventQueueId_t eqId, elosEventVector_t **eventVector)

MOCK_FUNC_PROTOTYPE(elosEventQueueManagerEntryPush, safuResultE_t, elosEventQueueManager_t *eqm,
                    elosEventQueueId_t eqId, elosEvent_t *event)

#endif /* ELOS_EVENTPROCESSOR_EVENTFILTERMANAGER_MOCK_H */

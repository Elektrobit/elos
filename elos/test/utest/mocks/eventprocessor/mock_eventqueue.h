// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTPROCESSOR_EVENTQUEUE_MOCK_H
#define ELOS_EVENTPROCESSOR_EVENTQUEUE_MOCK_H

#include <cmocka_extensions/mock_extensions.h>
#ifdef MOCK_EVENTPROCESSOR_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "elos/eventprocessor/eventqueue.h"

MOCK_FUNC_PROTOTYPE(elosEventQueueNew, safuResultE_t, elosEventQueue_t **eventQueue, elosEventQueueParam_t const *param)
MOCK_FUNC_PROTOTYPE(elosEventQueueInitialize, safuResultE_t, elosEventQueue_t *eventQueue,
                    elosEventQueueParam_t const *param)
MOCK_FUNC_PROTOTYPE(elosEventQueueDeleteMembers, void, elosEventQueue_t *eventQueue)
MOCK_FUNC_PROTOTYPE(elosEventQueueDelete, void, elosEventQueue_t *eventQueue)
MOCK_FUNC_PROTOTYPE(elosEventQueuePush, safuResultE_t, elosEventQueue_t *eventQueue, elosEvent_t const *event)
MOCK_FUNC_PROTOTYPE(elosEventQueueRead, safuResultE_t, elosEventQueue_t *eventQueue, elosEventVector_t **eventVector)

#endif /* ELOS_EVENTPROCESSOR_EVENTQUEUE_MOCK_H */

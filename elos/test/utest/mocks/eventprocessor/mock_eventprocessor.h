// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTPROCESSOR_EVENTPROCESSOR_MOCK_H
#define ELOS_EVENTPROCESSOR_EVENTPROCESSOR_MOCK_H

#include <cmocka_extensions/mock_extensions.h>
#ifdef MOCK_EVENTPROCESSOR_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "elos/eventprocessor/eventprocessor.h"

MOCK_FUNC_PROTOTYPE(elosEventProcessorInitialize, safuResultE_t, elosEventProcessor_t *eventProcessor,
                    elosEventProcessorParam_t const *param)

MOCK_FUNC_PROTOTYPE(elosEventProcessorDeleteMembers, safuResultE_t, elosEventProcessor_t *eventProcessor)

MOCK_FUNC_PROTOTYPE(elosEventProcessorFilterNodeCreateWithQueue, safuResultE_t, elosEventProcessor_t *eventProcessor,
                    char const **filterStrings, size_t filterStringCount, elosEventQueueId_t *eventQueueId,
                    elosEventFilterNodeId_t *eventFilterNodeId)

MOCK_FUNC_PROTOTYPE(elosEventProcessorFilterNodeCreate, safuResultE_t, elosEventProcessor_t *eventProcessor,
                    char const **filterStrings, size_t filterStringCount, elosEventQueueId_t eventQueueId,
                    elosEventFilterNodeId_t *eventFilterNodeId)

MOCK_FUNC_PROTOTYPE(elosEventProcessorFilterNodeRemove, safuResultE_t, elosEventProcessor_t *eventProcessor,
                    elosEventFilterNodeId_t eventFilterNodeId)

MOCK_FUNC_PROTOTYPE(elosEventProcessorQueueCreate, safuResultE_t, elosEventProcessor_t *eventProcessor,
                    elosEventQueueId_t *eventQueueId)

MOCK_FUNC_PROTOTYPE(elosEventProcessorQueueRemove, safuResultE_t, elosEventProcessor_t *eventProcessor,
                    elosEventQueueId_t eventQueueId)

MOCK_FUNC_PROTOTYPE(elosEventProcessorQueueRead, safuResultE_t, elosEventProcessor_t *eventProcessor,
                    elosEventQueueId_t eventQueueId, elosEventVector_t **eventVector)

MOCK_FUNC_PROTOTYPE(elosEventProcessorPublish, safuResultE_t, elosEventProcessor_t *eventProcessor,
                    elosEvent_t const *event)

#endif /* ELOS_EVENTPROCESSOR_EVENTPROCESSOR_MOCK_H */

// SPDX-License-Identifier: MIT
#pragma once

#include "elos/event/event_vector.h"
#include "elos/eventprocessor/types.h"

__BEGIN_DECLS

safuResultE_t elosEventProcessorInitialize(elosEventProcessor_t *eventProcessor,
                                           elosEventProcessorParam_t const *param);

safuResultE_t elosEventProcessorDeleteMembers(elosEventProcessor_t *eventProcessor);

safuResultE_t elosEventProcessorFilterNodeCreateWithQueue(elosEventProcessor_t *eventProcessor,
                                                          char const **filterStrings, size_t filterStringCount,
                                                          elosEventQueueId_t *eventQueueId,
                                                          elosEventFilterNodeId_t *eventFilterNodeId);

safuResultE_t elosEventProcessorFilterNodeCreate(elosEventProcessor_t *eventProcessor, char const **filterStrings,
                                                 size_t filterStringCount, elosEventQueueId_t eventQueueId,
                                                 elosEventFilterNodeId_t *eventFilterNodeId);

safuResultE_t elosEventProcessorFilterNodeRemove(elosEventProcessor_t *eventProcessor,
                                                 elosEventFilterNodeId_t eventFilterNodeId);

safuResultE_t elosEventProcessorQueueCreate(elosEventProcessor_t *eventProcessor, elosEventQueueId_t *eventQueueId);

safuResultE_t elosEventProcessorQueueRemove(elosEventProcessor_t *eventProcessor, elosEventQueueId_t eventQueueId);

safuResultE_t elosEventProcessorQueueRead(elosEventProcessor_t *eventProcessor, elosEventQueueId_t eventQueueId,
                                          elosEventVector_t **eventVector);

safuResultE_t elosEventProcessorPublish(elosEventProcessor_t *eventProcessor, elosEvent_t const *event);

__END_DECLS

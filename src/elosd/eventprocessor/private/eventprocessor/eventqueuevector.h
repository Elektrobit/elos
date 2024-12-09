// SPDX-License-Identifier: MIT

#ifndef ELOS_EVENTQUEUEVECTOR_H
#define ELOS_EVENTQUEUEVECTOR_H

#include "elos/eventprocessor/eventqueue.h"
#include "elos/eventprocessor/types.h"

// Basic interface functions
safuResultE_t elosEventQueueVectorNew(elosEventQueueVector_t **eventQueueVector, uint32_t elements);
safuResultE_t elosEventQueueVectorInitialize(elosEventQueueVector_t *eventQueueVector, uint32_t elements);
void elosEventQueueVectorDeleteMembers(elosEventQueueVector_t *eventQueueVector);
void elosEventQueueVectorDelete(elosEventQueueVector_t *eventQueueVector);

// Adding and removing elements
safuResultE_t elosEventQueueVectorPushByParam(elosEventQueueVector_t *eventQueueVector,
                                              elosEventQueueParam_t const *param);
safuResultE_t elosEventQueueVectorGetById(elosEventQueueVector_t *eventQueueVector, elosEventQueueId_t id,
                                          elosEventQueue_t **eventQueue);
safuResultE_t elosEventQueueVectorRemoveById(elosEventQueueVector_t *eventQueueVector, elosEventQueueId_t id);

#endif /* ELOS_EVENTQUEUEVECTOR_H */

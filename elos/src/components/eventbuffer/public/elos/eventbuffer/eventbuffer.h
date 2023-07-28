// SPDX-License-Identifier: MIT
#pragma once

#include "elos/event/event_types.h"
#include "elos/eventbuffer/defines.h"
#include "elos/eventbuffer/types.h"

safuResultE_t elosEventBufferNew(elosEventBuffer_t **eventBuffer, elosEventBufferParam_t const *const param);
safuResultE_t elosEventBufferInitialize(elosEventBuffer_t *eventBuffer, elosEventBufferParam_t const *const param);
safuResultE_t elosEventBufferRead(elosEventBuffer_t *eventBuffer, size_t priority, elosEventPtrVector_t *eventPtrVector,
                                  size_t *elementsWritten);
safuResultE_t elosEventBufferWrite(elosEventBuffer_t *eventBuffer, elosEvent_t const *const event);
safuResultE_t elosEventBufferSetWriteTrigger(elosEventBuffer_t *eventBuffer, int eventfd);
safuResultE_t elosEventBufferDeleteMembers(elosEventBuffer_t *eventBuffer);
safuResultE_t elosEventBufferDelete(elosEventBuffer_t **eventBuffer);

// SPDX-License-Identifier: MIT
#pragma once

#include <safu/result.h>

#include "elos/event/event_vector.h"
#include "elos/eventprocessor/types.h"

safuResultE_t elosEventQueueNew(elosEventQueue_t **eventQueue, elosEventQueueParam_t const *param);
safuResultE_t elosEventQueueInitialize(elosEventQueue_t *eventQueue, elosEventQueueParam_t const *param);
void elosEventQueueDeleteMembers(elosEventQueue_t *eventQueue);
void elosEventQueueDelete(elosEventQueue_t *eventQueue);
safuResultE_t elosEventQueuePush(elosEventQueue_t *eventQueue, elosEvent_t const *event);
safuResultE_t elosEventQueueRead(elosEventQueue_t *eventQueue, elosEventVector_t **eventVector);

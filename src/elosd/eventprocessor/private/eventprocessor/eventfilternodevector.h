// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTPROCESSOR_EVENTFILTERNODEVECTOR_H
#define ELOS_EVENTPROCESSOR_EVENTFILTERNODEVECTOR_H

#include <safu/common.h>
#include <stdint.h>

#include "elos/eventfilter/eventfilter.h"
#include "elos/eventprocessor/eventfilternode.h"
#include "elos/eventprocessor/types.h"

// Standard interface functions
safuResultE_t elosEventFilterNodeVectorInitialize(elosEventFilterNodeVector_t *eventFilterNodeVector,
                                                  uint32_t elements);
safuResultE_t elosEventFilterNodeVectorDeleteMembers(elosEventFilterNodeVector_t *eventFilterNodeVector);

// Interface functions

safuResultE_t elosEventFilterNodeVectorPushByParam(elosEventFilterNodeVector_t *eventFilterNodeVector,
                                                   elosEventFilterNodeParam_t const *param);
safuResultE_t elosEventFilterNodeVectorRemoveById(elosEventFilterNodeVector_t *eventFilterNodeVector,
                                                  elosEventFilterNodeId_t id);
safuResultE_t elosEventFilterNodeVectorProcess(elosEventFilterNodeVector_t *eventFilterNodeVector,
                                               elosEventFilterStack_t const *filterStack, elosEvent_t const *event);

#endif /* ELOS_EVENTPROCESSOR_EVENTFILTERNODEVECTOR_H */

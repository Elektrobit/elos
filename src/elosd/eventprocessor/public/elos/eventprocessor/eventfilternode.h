// SPDX-License-Identifier: MIT
#pragma once

#include <safu/result.h>
#include <stddef.h>

#include "elos/eventfilter/eventfilter_types.h"
#include "elos/eventfilter/vector_types.h"
#include "elos/eventprocessor/eventqueue.h"
#include "elos/eventprocessor/idmanager.h"
#include "elos/eventprocessor/types.h"

__BEGIN_DECLS

// Standard interface functions
safuResultE_t elosEventFilterNodeInitialize(elosEventFilterNode_t *eventFilterNode,
                                            elosEventFilterNodeParam_t const *param);

safuResultE_t elosEventFilterNodeDeleteMembers(elosEventFilterNode_t *eventFilterNode);

// Interface functions
safuResultE_t elosEventFilterNodePush(elosEventFilterNode_t *eventFilterNode, char const **filterString,
                                      size_t filterStringCount);

safuResultE_t elosEventFilterNodeProcess(elosEventFilterNode_t *eventFilterNode,
                                         elosEventFilterStack_t const *filterStack, elosEvent_t const *event);

__END_DECLS

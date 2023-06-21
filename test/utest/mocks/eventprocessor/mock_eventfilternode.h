// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTPROCESSOR_EVENTFILTERNODE_MOCK_H
#define ELOS_EVENTPROCESSOR_EVENTFILTERNODE_MOCK_H

#include <cmocka_extensions/mock_extensions.h>
#ifdef MOCK_EVENTPROCESSOR_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "elos/eventprocessor/eventfilternode.h"

MOCK_FUNC_PROTOTYPE(elosEventFilterNodeInitialize, safuResultE_t, elosEventFilterNode_t *eventFilterNode,
                    elosEventFilterNodeParam_t const *param)

MOCK_FUNC_PROTOTYPE(elosEventFilterNodeDeleteMembers, safuResultE_t, elosEventFilterNode_t *eventFilterNode)

MOCK_FUNC_PROTOTYPE(elosEventFilterNodePush, safuResultE_t, elosEventFilterNode_t *eventFilterNode,
                    char const **filterString, size_t filterStringCount)

MOCK_FUNC_PROTOTYPE(elosEventFilterNodeProcess, safuResultE_t, elosEventFilterNode_t *eventFilterNode,
                    elosEventFilterStack_t const *filterStack, elosEvent_t const *event)

#endif /* ELOS_EVENTPROCESSOR_EVENTFILTERNODE_MOCK_H */

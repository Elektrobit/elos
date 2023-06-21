// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTPROCESSOR_EVENTFILTERNODEVECTOR_MOCK_H
#define ELOS_EVENTPROCESSOR_EVENTFILTERNODEVECTOR_MOCK_H

#include <cmocka_extensions/mock_extensions.h>
#ifdef MOCK_EVENTPROCESSOR_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "eventprocessor/eventfilternodevector.h"

MOCK_FUNC_PROTOTYPE(elosEventFilterNodeVectorInitialize, safuResultE_t,
                    elosEventFilterNodeVector_t *eventFilterNodeVector, uint32_t elements)

MOCK_FUNC_PROTOTYPE(elosEventFilterNodeVectorDeleteMembers, safuResultE_t,
                    elosEventFilterNodeVector_t *eventFilterNodeVector)

MOCK_FUNC_PROTOTYPE(elosEventFilterNodeVectorPushByParam, safuResultE_t,
                    elosEventFilterNodeVector_t *eventFilterNodeVector, elosEventFilterNodeParam_t const *param)

MOCK_FUNC_PROTOTYPE(elosEventFilterNodeVectorRemoveById, safuResultE_t,
                    elosEventFilterNodeVector_t *eventFilterNodeVector, elosEventFilterNodeId_t id)

MOCK_FUNC_PROTOTYPE(elosEventFilterNodeVectorProcess, safuResultE_t, elosEventFilterNodeVector_t *eventFilterNodeVector,
                    elosEventFilterStack_t const *filterStack, elosEvent_t const *event)

#endif /* ELOS_EVENTPROCESSOR_EVENTFILTERNODEVECTOR_MOCK_H */

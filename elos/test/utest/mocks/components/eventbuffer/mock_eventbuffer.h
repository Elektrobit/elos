// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#ifdef EVENTBUFFER_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "elos/eventbuffer/eventbuffer.h"

MOCK_FUNC_PROTOTYPE(elosEventBufferInitialize, safuResultE_t, elosEventBuffer_t *eventBuffer,
                    elosEventBufferParam_t const *const param)

MOCK_FUNC_PROTOTYPE(elosEventBufferNew, safuResultE_t, elosEventBuffer_t **eventBuffer,
                    elosEventBufferParam_t const *const param)

MOCK_FUNC_PROTOTYPE(elosEventBufferDeleteMembers, safuResultE_t, elosEventBuffer_t *eventBuffer)

MOCK_FUNC_PROTOTYPE(elosEventBufferDelete, safuResultE_t, elosEventBuffer_t **eventBuffer)

MOCK_FUNC_PROTOTYPE(elosEventBufferRead, safuResultE_t, elosEventBuffer_t *eventBuffer, size_t priority,
                    elosEventPtrVector_t *eventPtrVector, size_t *elementsWritten)

MOCK_FUNC_PROTOTYPE(elosEventBufferWrite, safuResultE_t, elosEventBuffer_t *eventBuffer, elosEvent_t const *const event)

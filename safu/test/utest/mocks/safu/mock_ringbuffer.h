// SPDX-License-Identifier: MIT

#ifndef __MOCK_SAFU_RINGBUFFER_H__
#define __MOCK_SAFU_RINGBUFFER_H__

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>

#ifdef MOCK_SAFU_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "safu/ringbuffer.h"

MOCK_FUNC_PROTOTYPE(safuRingBufferNew, safuResultE_t, safuRingBuffer_t **ringBuffer,
                    safuRingBufferParam_t const *const param)
MOCK_FUNC_PROTOTYPE(safuRingBufferInitialize, safuResultE_t, safuRingBuffer_t *ringBuffer,
                    safuRingBufferParam_t const *const param)
MOCK_FUNC_PROTOTYPE(safuRingBufferDeleteMembers, safuResultE_t, safuRingBuffer_t *ringBuffer)
MOCK_FUNC_PROTOTYPE(safuRingBufferDelete, safuResultE_t, safuRingBuffer_t **ringBuffer)
MOCK_FUNC_PROTOTYPE(safuRingBufferWrite, safuResultE_t, safuRingBuffer_t *ringBuffer, void *element)
MOCK_FUNC_PROTOTYPE(safuRingBufferRead, safuResultE_t, safuRingBuffer_t *ringBuffer, safuVec_t *elementVector,
                    size_t *elementsWritten)

#endif /* __MOCK_SAFU_RINGBUFFER_H___H__ */
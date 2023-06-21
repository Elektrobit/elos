// SPDX-License-Identifier: MIT
#pragma once

#include <safu/common.h>
#include <safu/flags.h>
#include <safu/ringbuffer_types.h>
#include <safu/vector.h>

/**
 * Create a new Ringbuffer.
 *
 * - Shall allocate memory for a new ring buffer.
 * - Shall initialize the ring buffer via safuRingBufferInitialize.
 *
 * Parameters:
 *      ringBuffer: A pointer where the address of the newly created ringBuffer
 *                  can be stored. On Failure `ringBuffer` is unchanged.
 *      param: A `safuRingBufferParam_t` providing information about the to be
 *             created ring buffer.
 *
 * Return:
 *      safuResultE_t:
 *                      - SAFU_RESULT_OK – on success
 *                      - SAFU_RESULT_FAILED – if creation failed for any reason
 */
safuResultE_t safuRingBufferNew(safuRingBuffer_t **ringBuffer, safuRingBufferParam_t const *const param);
safuResultE_t safuRingBufferInitialize(safuRingBuffer_t *ringBuffer, safuRingBufferParam_t const *const param);
safuResultE_t safuRingBufferDeleteMembers(safuRingBuffer_t *ringBuffer);
safuResultE_t safuRingBufferDelete(safuRingBuffer_t **ringBuffer);

safuResultE_t safuRingBufferWrite(safuRingBuffer_t *ringBuffer, void *element);
safuResultE_t safuRingBufferRead(safuRingBuffer_t *ringBuffer, safuVec_t *elementVector, size_t *elementsWritten);

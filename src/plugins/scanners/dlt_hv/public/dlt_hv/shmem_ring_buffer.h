// SPDX-License-Identifier: MIT

#pragma once

#include <safu/result.h>
#include <safu/ringbuffer_types.h>

#include "dlt_hv/types.h"
#include "elos/libelosplugin/types.h"

/*************************************************
 * Read the `elosEbLogRingBuffer_t` and write all new entries into the readQueue
 *
 * Parameters:
 *   buff: The DLT ring buffer to check for new entries
 *   idxRead: The read index to start reading from (will be updated if new entries are read)
 *   readQueue: The queue to write them to for further processing
 *
 * Returns:
 *   - SAFU_RESULT_OK: on success
 *   - SAFU_RESULT_NOT_FOUND: when nothing new was in the `elosEbLogRingBuffer_t`
 *   - SAFU_RESULT_FAILED: on error
 *************************************************/
safuResultE_t elosQueueNextLogEntries(elosEbLogRingBuffer_t *buff, uint16_t *idxRead, safuRingBuffer_t *readQueue);

/*************************************************
 * publishes all the entreis from the read queue to elos
 *
 * Parameters:
 *   readQueue: the queue to read the next entries from
 *   plugin: the reference to the plugin
 *   publisher: the publisher to publish to
 *************************************************/
safuResultE_t elosPublishEntriesFromQueue(safuRingBuffer_t *readQueue, elosPlugin_t *plugin,
                                          struct elosPublisher *publisher);

/*************************************************
 * frees the memory of a dlt log entry
 *
 * Parameters:
 *   entry: the entry to free
 *************************************************/
safuResultE_t elosFreeEbLogEntry(elosEbLogEntry_t *entry);

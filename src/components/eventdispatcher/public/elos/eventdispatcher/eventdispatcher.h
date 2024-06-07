// SPDX-License-Identifier: MIT
#pragma once

#include <safu/result.h>

#include "elos/eventdispatcher/defines.h"
#include "elos/eventdispatcher/types.h"

__BEGIN_DECLS

/*******************************************************************
 * Allocates space for and initialize a new EventDispatcher
 *
 * Parameters:
 *   eventDispatcher:
 *       Pointer to a pointer that will contain the new EventDispatcher.
 *       Won't be modified in case the calls fails.
 *   param:
 *       Parameters for the initialization
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosEventDispatcherNew(elosEventDispatcher_t **eventDispatcher,
                                     elosEventDispatcherParam_t const *const param);

/*******************************************************************
 * Intialize an EventDispatcher
 *
 * Parameters:
 *   eventDispatcher:
 *       Pointer to a EventDispatcher.
 *   param:
 *       Parameters for the initialization
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosEventDispatcherInitialize(elosEventDispatcher_t *eventDispatcher,
                                            elosEventDispatcherParam_t const *const param);

/*******************************************************************
 * Shutdown and free resources used by the given EventDispatcher.
 *
 * Parameters:
 *   eventDispatcher:
 *       Pointer to the EventDispatcher.
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosEventDispatcherDeleteMembers(elosEventDispatcher_t *eventDispatcher);

/*******************************************************************
 * Shutdown and free resources used by the given EventDispatcher,
 * including the memory allocated for the elosEventDispatcher_t data type
 *
 * Parameters:
 *   eventDispatcher:
 *       Pointer to a pointer containing the EventDispatcher.
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosEventDispatcherDelete(elosEventDispatcher_t **eventDispatcher);

/*******************************************************************
 * Starts the EventDispatcher's worker thread that is responsible for
 * dispatching the Events from various EventBuffers to the EventProcessor
 *
 * Parameters:
 *   eventDispatcher:
 *       Pointer to the EventDispatcher.
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosEventDispatcherStart(elosEventDispatcher_t *eventDispatcher);

/*******************************************************************
 * Stops the EventDispatcher's worker thread.
 *
 * Parameters:
 *   eventDispatcher:
 *       Pointer to the EventDispatcher.
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosEventDispatcherStop(elosEventDispatcher_t *eventDispatcher);

/*******************************************************************
 * Dispatch Events from EventBuffers to the EventProcessor.
 * Used internally by the EventDispatcher's worker thread.
 *
 * Parameters:
 *   eventDispatcher:
 *       Pointer to the EventDispatcher.
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosEventDispatcherDispatch(elosEventDispatcher_t *eventDispatcher);

/*******************************************************************
 * Add an EventBuffer to be monitored by the EventDispatcher.
 *
 * Parameters:
 *   eventDispatcher:
 *       Pointer to the EventDispatcher.
 *   eventBuffer:
 *       Pointer to the EventBuffer that shall be monitored.
 *       The EventBuffer must be initialized prior to calling this function.
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosEventDispatcherBufferAdd(elosEventDispatcher_t *eventDispatcher, elosEventBuffer_t *eventBuffer);

/*******************************************************************
 * Remove an monitored EventBuffer from the EventDispatcher.
 *
 * Parameters:
 *   eventDispatcher:
 *       Pointer to the EventDispatcher.
 *   eventBuffer:
 *       Pointer to the EventBuffer that shall not be monitored anymore.
 *       The EventBuffer is not free'd by this call and is still useable after the Remove.
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosEventDispatcherBufferRemove(elosEventDispatcher_t *eventDispatcher, elosEventBuffer_t *eventBuffer);

__END_DECLS

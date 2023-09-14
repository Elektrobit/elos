// SPDX-License-Identifier: MIT
#pragma once

#include <json-c/json_types.h>
#include <safu/result.h>

#include "elos/event/event_types.h"

/*******************************************************************
 * Allocates a new event.
 *
 * Info:
 *      The returned object must be destroyed with :c:func:`elosEventSourceDelete()`.
 *
 * Parameters:
 *      eventSource (elosEventSource_t **): the event to allocate.
 * Returns:
 *      - `SAFU_RESULT_OK` on success,
 *      - `SAFU_RESULT_FAILED` on failure.
 ******************************************************************/
safuResultE_t elosEventSourceNew(elosEventSource_t **eventSource);

/*******************************************************************
 * Frees all members of an event.
 *
 * Parameters:
 *      eventSource (elosEvnetSource_t *): Pointer to the eventSource.
 ******************************************************************/
void elosEventSourceDeleteMembers(elosEventSource_t *eventSource);

/*******************************************************************
 * Frees an event allocated with :c:func:`elosEventSourceNew()`
 * or :c:func:`losEventSourceDeepCopy()`.
 *
 * Parameters:
 *      eventSource (elosEventSource_t): Pointer to the allocated memory space.
 ******************************************************************/
void elosEventSourceDelete(elosEventSource_t *eventSource);

/*******************************************************************
 * Saves the members of an :c:type:`elosEventSource_t` into a JSON formatted string.
 *
 * Parameters:
 *      toJsonString (char **): the string to safe the event to.
 *      from (elosEventSouce_t *): the event to serialize.
 * Returns:
 *      - `SAFU_RESULT_OK` on success,
 *      - `SAFU_RESULT_FAILED` on failure.
 ******************************************************************/
safuResultE_t elosEventSourceSerialize(char **toJsonString, elosEventSource_t *from);

/*******************************************************************
 * Stores the information of a JSON formatted string into a elosEventSource_t object.
 *
 * Parameters:
 *      to (elosEventSource_t *): Event to safe the data from the JSON to.
 *      fromJsonString (const char *): JSON formatted string to deserialize.
 * Returns:
 *      - `SAFU_RESULT_OK` on success,
 *      - `SAFU_RESULT_FAILED` on failure.
 ******************************************************************/
safuResultE_t elosEventSourceDeserialize(elosEventSource_t *to, const char *fromJsonString);

/*******************************************************************
 * Duplicates elosEventSource_t with its members
 *
 * Info:
 *      The returned object must be destroyed with :c:func:`elosEventSourceDelete()`.
 *
 * Parameters:
 *      to (elosEventSource_t *): The event to copy to.
 *      from (elosEventSource_t *): The event to copy.
 * Returns:
 *      - `SAFU_RESULT_OK` on success,
 *      - `SAFU_RESULT_FAILED` on failure.
 ******************************************************************/
safuResultE_t elosEventSourceDeepCopy(elosEventSource_t *to, const elosEventSource_t *from);

/*******************************************************************
 * Converts the information from an event
 * and stores them into an existing JSON object`.
 *
 * Parameters:
 *      to (json_object *): The JSON object to convert to.
 *      from (elosEventSource_t *): The event to convert.
 * Returns:
 *      - `SAFU_RESULT_OK` on success,
 *      - `SAFU_RESULT_FAILED` on failure.
 ******************************************************************/
safuResultE_t elosEventSourceToJsonObject(json_object *to, const elosEventSource_t *from);

/*******************************************************************
 * Converts the information from a JSON object
 * and stores them into an existing event.
 *
 * Parameters:
 *      to (elosEventSource_t *): The evnt to convert into.
 *      from (const json_object *): The JSON obect to convert.
 * Returns:
 *      - `SAFU_RESULT_OK` on success,
 *      - `SAFU_RESULT_FAILED` on failure.
 ******************************************************************/
safuResultE_t elosEventSourceFromJsonObject(elosEventSource_t *to, const json_object *from);

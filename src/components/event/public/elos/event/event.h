// SPDX-License-Identifier: MIT
#pragma once

#include <json-c/json_types.h>
#include <safu/result.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "elos/event/event_source.h"
#include "elos/event/types.h"

__BEGIN_DECLS

/*******************************************************************
 * Allocates a new zero initialized event.
 *
 * Parameters:
 *      event (elosEvent_t **): the event is allocated and needs to be freed later.
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosEventNew(elosEvent_t **event);

/*******************************************************************
 * Zero initilizes an event.
 *
 * Parameters:
 *      event (elosEvent_t *): the event to initialize.
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosEventInitialize(elosEvent_t *event);

/*******************************************************************
 * Creates a deep copy into an exsiting event.
 *
 * Parameters:
 *      to (elosEvent_t *): the event to fill with the copy.
 *      from (elosEvent_t *): the event to copy from.
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosEventDeepCopy(elosEvent_t *to, elosEvent_t const *from);

/*******************************************************************
 * Allocates a new event as a copy of an existing one
 *
 * Parameters:
 *      to (elosEvent_t *): return location for the Copy
 *      from (elosEvent_t *): event to clone
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosEventClone(elosEvent_t **to, elosEvent_t const *from);

/*******************************************************************
 * Free the memory for all the mebers of an event.
 *
 * Parameters:
 *      event (elosEvent_t *): the event to remove the members from.
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosEventDeleteMembers(elosEvent_t *event);

/*******************************************************************
 * Free the memory of an event.
 *
 * Parameters:
 *      event (elosEvent_t *): the event to free.
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosEventDelete(elosEvent_t *event);

/*******************************************************************
 * Serializes an event into a JSON string
 *
 * Parameters:
 *      toJsonString (char **): the string to serialize into.
 *      from (const elosEvent_t *): the event to serialize.
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosEventSerialize(char **toJsonString, const elosEvent_t *from);

/*******************************************************************
 * Parse an event from a JSON string.
 *
 * Parameters:
 *      to (elosEvent_t *): Pointer to the event to deserialize into.
 *      from (const char *): the string to deserialize.
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosEventDeserialize(elosEvent_t *to, const char *fromJsonString);

/*******************************************************************
 * Fill an event with data from a JSON object.
 *
 * Parameters:
 *      to (elosEvent_t *): Pointer to a :c:type:`elosEvent_t` to fill with :c:type:`json_object` data.
 *      from (json_object *): The :c:type:`json_object` make into an :c:expr:`elosEvent_t *`.
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosEventFromJsonObject(elosEvent_t *to, const json_object *from);

/*******************************************************************
 * Generates a JSON object from an event.
 *
 * Parameters:
 *      to (json_object *): Pointer to a :c:type:`json_object` to fill with :c:expr:`elosEvent_t *` data.
 *      from (elosEvent_t *): the :c:type:`elosEvent_t` to make into an :c:type:`json_object`.
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosEventToJsonObject(json_object *to, const elosEvent_t *from);

/*******************************************************************
 * Check if the date field of an event is empty.
 *
 * Parameters:
 *      event (const elosEvent_t * const): the event to check.
 * Returns:
 *      `true` if seconds and nano second of date are 0
 ******************************************************************/
bool elosEventIsDateEmpty(const elosEvent_t *const event);

__END_DECLS

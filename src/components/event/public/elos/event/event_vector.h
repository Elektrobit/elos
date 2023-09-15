// SPDX-License-Identifier: MIT

#ifndef ELOS_EVENT_VECTOR_H
#define ELOS_EVENT_VECTOR_H

#include "elos/event/event.h"
#include "safu/common.h"
#include "safu/vector.h"

/*******************************************************************
 * Allocate a new event vector.
 *
 * Parameters:
 *      eventVector (elosEventVector_t **): The vector to allocate.
 *      elements (uint32_t): the number of elements to allocate.
 * Returns:
 *      - `SAFU_RESULT_OK` on success,
 *      - `SAFU_RESULT_FAILED` on failure.
 ******************************************************************/
safuResultE_t elosEventVectorNew(elosEventVector_t **eventVector, uint32_t elements);

/*******************************************************************
 * Parameters:
 *      eventVector (elosEventVector_t *): the event vector to initialize.
 *      elements (uint32_t): the number of elements to initialize.
 * Returns:
 *      - `SAFU_RESULT_OK` on success,
 *      - `SAFU_RESULT_FAILED` on failure.
 ******************************************************************/
safuResultE_t elosEventVectorInitialize(elosEventVector_t *eventVector, uint32_t elements);

/*******************************************************************
 * Serialize an event vector.
 *
 * Parameters:
 *      eventVector (elosEventVector_t *): the event vector to serialize.
 *      sEventVector (char **): the string to serialize into.
 * Returns:
 *      - `SAFU_RESULT_OK` on success,
 *      - `SAFU_RESULT_FAILED` on failure.
 ******************************************************************/
safuResultE_t elosEventVectorSerialize(elosEventVector_t *eventVector, char **sEventVector);

/*******************************************************************
 * Convert an event vector into a JSON object.
 *
 * Parameters:
 *      eventVector (elosEventVector_t *): the event vector to convert.
 *      jEventVector (json_object **): the JSON object to convert into.
 * Returns:
 *      - `SAFU_RESULT_OK` on success,
 *      - `SAFU_RESULT_FAILED` on failure.
 ******************************************************************/
safuResultE_t elosEventVectorToJsonObject(elosEventVector_t *eventVector, json_object **eventVecJobj);

/*******************************************************************
 * Convert an event vector into a JSON array.
 *
 * Parameters:
 *      eventVector (elosEventVector_t *): the event vector to convert.
 *      jEventVector (json_object **): the JSON Array to convert into.
 * Returns:
 *      - `SAFU_RESULT_OK` on success,
 *      - `SAFU_RESULT_FAILED` on failure.
 ******************************************************************/
safuResultE_t elosEventVectorToJsonArray(elosEventVector_t *eventVector, json_object **jEventVector);

/*******************************************************************
 * Convert a JSON object into an event vector.
 *
 * Parameters:
 *      eventVecJobj (json_object *): the JSON object to convert.
 *      eventVector (elosEventVector_t **): the event vector to convert into.
 * Returns:
 *      - `SAFU_RESULT_OK` on success,
 *      - `SAFU_RESULT_FAILED` on failure.
 ******************************************************************/
safuResultE_t elosEventVectorFromJsonObject(json_object *eventVecJobj, elosEventVector_t **eventVector);

/*******************************************************************
 * Convert a JSON array into an event vector.
 *
 * Parameters:
 *      eventVecJobj (json_object *): the JSON array to convert.
 *      eventVector (elosEventVector_t **): the event vector to convert into.
 * Returns:
 *      - `SAFU_RESULT_OK` on success,
 *      - `SAFU_RESULT_FAILED` on failure.
 ******************************************************************/
safuResultE_t elosEventVectorFromJsonArray(json_object const *jEventArray, elosEventVector_t **eventVector);

/*******************************************************************
 * Push an elosEvent to an elosEventVector
 *
 * Parameters:
 *      eventVector (elosEventVector_t *): the event vector to push into.
 *      event (elosEvent_t const *): The event to be pushed.
 * Returns:
 *      - `SAFU_RESULT_OK` on success,
 *      - `SAFU_RESULT_FAILED` on failure.
 ******************************************************************/
safuResultE_t elosEventVectorPush(elosEventVector_t *eventVector, elosEvent_t const *event);

/*******************************************************************
 * Push a newly allocated deep copy of an elosEvent to an elosEventVector
 *
 * Parameters:
 *      eventVector (elosEventVector_t *): the event vector to push into.
 *      event (elosEvent_t const *): The event to be copied.
 * Returns:
 *      - `SAFU_RESULT_OK` on success,
 *      - `SAFU_RESULT_FAILED` on failure.
 ******************************************************************/
safuResultE_t elosEventVectorPushDeepCopy(elosEventVector_t *eventVector, elosEvent_t const *event);

/*******************************************************************
 * Free the memomry of all members of an :c:type:`elosEventVector_t`.
 *
 * Parameters:
 *      eventVector (elosEventVector_t *): the event vector to delete the members from
 ******************************************************************/
void elosEventVectorDeleteMembers(elosEventVector_t *eventVector);

/*******************************************************************
 * Free the memomry of an event vector.
 *
 * Parameters:
 *      eventVector (elosEventVector_t *): the event vector to free.
 ******************************************************************/
void elosEventVectorDelete(elosEventVector_t *eventVector);

#endif /* ELOS_EVENT_VECTOR_H */

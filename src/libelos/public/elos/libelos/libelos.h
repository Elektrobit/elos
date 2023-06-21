// SPDX-License-Identifier: MIT
#ifndef LIBELOS_H
#define LIBELOS_H

#include <netinet/in.h>
#include <safu/common.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "elos/event/event.h"
#include "elos/event/event_vector.h"
#include "elos/event/eventqueue_types.h"

typedef struct elosSession {
    struct sockaddr_in addr;
    int fd;
    bool connected;
} elosSession_t;

/*******************************************************************
 * Function: elosConnectTcpip
 *------------------------------------------------------------------
 * Description:
 *      Establishes connection to elos over tcp/ip.
 * Input:
 *      - **ip**:       ip address, e.g. "192.168.2.1"
 *      - **port**:     port number, e.g. 54321
 * Output:
 *      - **session**:  session data structure used by other functions
 * Return:
 *      - `SAFU_RESULT_OK` for success or `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosConnectTcpip(const char *ip, uint16_t port, elosSession_t **session);

/*******************************************************************
 * Function: elosDisconnect
 *------------------------------------------------------------------
 * Description:
 *      Disconnects from elos and frees the allocated memory and objects.
 * Input:
 *      - **session**:  session data structure (invalid after function call)
 * Returns:
 *      - `SAFU_RESULT_OK` for success or `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosDisconnect(elosSession_t *session);

/*******************************************************************
 * Function: elosGetVersion
 *------------------------------------------------------------------
 * Description:
 *      Returns the version of the elos server.
 * Input:
 *      - **session**:  session data structure
 * Output:
 *      - **version**:  version of the elos server in form of *"major.minor"* (e.g. "1.0")
 * Returns:
 *      - `SAFU_RESULT_OK` for success or `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosGetVersion(elosSession_t *session, const char **version);

/*******************************************************************
 * Function: elosEventSubscribe
 *------------------------------------------------------------------
 * Description:
 *      Creates an EventQueue that is filled with Events that match at least
 *      one of the given EventFilterStrings.
 * Info:
 *      - The EventQueue can be referred to by the EventQueueID while polling for new events.
 *      - Multiple EventQueues can be created and used.
 *      - The EventQueueID is only valid during the active session.
 *        Once the session is closed (elosDisconnect) the EventQueues associated with it are removed.
 * Input:
 *      - **session**:              session data structure
 *      - **filterRuleArray**:      list of EventFilterStrings
 *        (e.g. `{".event.messageCode 8004 EQ", ".event.messageCode 8005 EQ"}`)
 *      - **filterRuleArraySize**:  the lenght of the `filterRuleArray`
 * Output:
 *      - **eventQueueId**:         an id for the created eventlist that is used for fetching events
 * Returns:
 *      - `SAFU_RESULT_OK` for success or `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosEventSubscribe(elosSession_t *session, char const **filterRuleArray, size_t filterRuleArraySize,
                                 elosEventQueueId_t *eventQueueId);

/*******************************************************************
 * Function: elosEventUnsubscribe
 *------------------------------------------------------------------
 * Description:
 *      Deletes the EventQueue and the associated filters.
 * Info:
 *      - The EventQueue can be referred to by the EventQueueID while polling for new events.
 * Input:
 *      - **session**:              session data structure
 * Output:
 *      - **eventQueueId**:         an id for the eventlist tto be removed
 * Returns:
 *      - `SAFU_RESULT_OK` for success or `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosEventUnsubscribe(elosSession_t *session, elosEventQueueId_t eventQueueId);

/*******************************************************************
 * Function: elosEventPublish
 *------------------------------------------------------------------
 * Description:
 *      Publishes the given Event.
 * Input:
 *      - **session**:  session data structure
 * Output:
 *      - **event**:    content of the event in form of a elosEvent_t struct
 * Returns:
 *      - `SAFU_RESULT_OK` for success or `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosEventPublish(elosSession_t *session, const elosEvent_t *event);

/*******************************************************************
 * Function: elosEventQueueRead
 *------------------------------------------------------------------
 * Description:
 *      Waits for new events in the given EventQueue and returns them in form of a json string.
 *
 *      - The function is blocking or non blocking depending on how the timeout value is set
 *      - The memory for events will be allocated by the library and must be freed by the user
 *        _TBD: How to pass through EventFree()?_
 * Input:
 *      - **session**:       session data structure
 *      - **eventQueueId**:  the id of the eventQueue to read from
 * Output:
 *      - **eventsVector**:  list of elosEvent_t structs containing the occuring events
 * Returns:
 *      - `SAFU_RESULT_OK` for success or `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosEventQueueRead(elosSession_t *session, elosEventQueueId_t eventQueueId,
                                 elosEventVector_t **eventVector);

/*******************************************************************
 * Function: elosLogFindEvent
 *------------------------------------------------------------------
 * Description:
 *      Fetches all logged events based on the used filter (e.g. based on payload or other data).
 *
 *      - In case no events are in the log, the vector parameter won't be changed.
 *      - The memory of the event vector will be allocated by the library
 *        and must be freed afterwards with elosEventVectorDelete().
 * Input:
 *      - **session**:     session data structure
 *      - **filterRule**:  filter rule for entry selection
 * Output:
 *      - **vector**:      list of event structs
 * Returns:
 *      - `SAFU_RESULT_OK` for success or `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosLogFindEvent(elosSession_t *session, const char *filterRule, elosEventVector_t **eventVector);

/*******************************************************************
 * Function: elosSessionValid
 *------------------------------------------------------------------
 * Description:
 *      Checks if the session is valid.
 * Input:
 *      - **session**:  session data structure to check if its valid.
 * Returns:
 *      - `true` for a valid seesion and `false` otherwise.
 ******************************************************************/
bool elosSessionValid(elosSession_t const *session);

#endif

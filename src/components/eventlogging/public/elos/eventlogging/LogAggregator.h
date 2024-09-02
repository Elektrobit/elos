// SPDX-License-Identifier: MIT
#ifndef __ELOS_LOGAGGREGATOR_H__
#define __ELOS_LOGAGGREGATOR_H__

#include <safu/common.h>
#include <safu/vector.h>
#include <samconf/samconf.h>

#include "elos/event/event.h"
#include "elos/eventlogging/LogAggregatorTypes.h"

__BEGIN_DECLS

/*******************************************************************
 * Initialize the event logging system. It initializes the configured
 * storage backend and setup synchronization and filter functionality.
 *
 * Parameters:
 *      logAggregator : a pointer to a logAggregator instance
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosLogAggregatorStart(elosLogAggregator_t *logAggregator, elosLogAggregatorParam_t const *param);

/*******************************************************************
 * Queue an event to store on non volatile storage.
 *
 * Parameters:
 *      logAggregator : a pointer to a logAggregator instance
 *      event         : the event that will get stored
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosLogAggregatorAdd(elosLogAggregator_t *logAggregator, const elosEvent_t *event);

/*******************************************************************
 * Shutdown the logging system and free all related resources.
 *
 * Parameters:
 *      logAggregator : a pointer to a logAggregator instance
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosLogAggregatorShutdown(elosLogAggregator_t *logAggregator);

/*******************************************************************
 * Find all logged events that match a given filter rule. The events are
 * gathered from the backend plugin instance named "fetchapi".
 * Matching events are appended to the third parameter, the event vector.
 *
 * Parameters:
 *      logAggregator : a pointer to a logAggregator instance
 *      rule          : a RPN-filter rule in string format
 *      events        : an event vector, where elements will be appended
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosLogAggregatorFindEvents(elosLogAggregator_t *logAggregator, const char *rule,
                                          struct timespec const *newest, struct timespec const *oldest,
                                          safuVec_t *events);

__END_DECLS

#endif /* __ELOS_LOGAGGREGATOR_H__ */

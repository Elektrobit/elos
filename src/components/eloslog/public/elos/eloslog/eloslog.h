// SPDX-License-Identifier: MIT
#pragma once

#include <safu/common.h>
#include <safu/log.h>

#include "elos/event/event.h"
#include "elos/event/event_types.h"

__BEGIN_DECLS

/*******************************************************************
 * A fallback function is to be used when the log event creation fails.
 * In this case the when log event creation fails, the elos log
 * uses this function to pass logData to safulog in order to output
 * log data to console.
 *
 * Parameters:
 *      event (elosEvent_t *) : log event to be logged
 ******************************************************************/
void elosLogSafuFallback(elosEvent_t *event);

/*******************************************************************
 * Create an new elos event and assign value from logData to it. Pass
 * this new event to output varaiable event.
 *
 * Parameters:
 *      messageCode (elosEventMessageCodeE_t) : message code for the message to be logged.
 *      severity (elosSeverityE_t) : severity of the message which is to be logged.
 *      classification (uint64_t) : classification of the message to be logged.
 *      logMessage (const char *) : the message to be logged.
 *      event (elosEvent_t *) : the elos event to which the above parameters are assigned.
 ******************************************************************/
void elosLogCreateElosEventFromLog(elosEventMessageCodeE_t messageCode, elosSeverityE_t severity,
                                   uint64_t classification, const char *logMessage, elosEvent_t *event);

__END_DECLS

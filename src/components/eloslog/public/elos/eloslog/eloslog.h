// SPDX-License-Identifier: MIT
#pragma once

#include "elos/eloslog/types.h"

/*******************************************************************
 * Pack given messageCode, severity, classification and logMessage
 * into the provided logdata variable of type `elosLogData_t`
 *
 * Parameters:
 *      messageCode (elosEventMessageCodeE_t) :  message code for the message to be logged
 *      severity (elosSeverityE_t) : severity of message to be logged
 *      classification (uint64_t) : classification of message to be logged
 *      logMessage (const char *) : the message to be packed into logdata
 *      logdata (elosLogData_t **) : the log data variable to pack other values into.
 ******************************************************************/
void elosLogCreateLogData(elosEventMessageCodeE_t messageCode, elosSeverityE_t severity, uint64_t classification,
                          const char *logMessage, elosLogData_t **logdata);

/*******************************************************************
 * Free Memory of all members of logData
 *
 * Parameters:
 *      logData (elosLogData_t *) : logData whos' allocated memory needs to be freed.
 ******************************************************************/
void elosLogDeleteLogData(elosLogData_t *logData);

/*******************************************************************
 * A fallback function is to be used when the log event creation fails.
 * In this case the when log event creation fails, the elos log
 * uses this function to pass logData to safulog in order to output
 * log data to console.
 *
 * Parameters:
 *      logData (elosLogData_t *) : log data to be logged via safulog
 ******************************************************************/
void elosLogSafuFallback(elosLogData_t *logData);

/*******************************************************************
 * Create an new elos event and assign value from logData to it. Pass
 * this new event to output varaiable event.
 *
 * Parameters:
 *      logData (elosLogData_t *) : log data containing data to be logged.
 *      event (elosEvent_t **) : elos event created from logData.
 *
 * Returns:
 *      - `ELOS_LOG_STATUS_SUCCESS` on success
 *      - `ELOS_LOG_STATUS_ERROR` on failure
 ******************************************************************/
elosLogStatusE_t elosLogCreateElosEventFromLog(elosLogData_t *logData, elosEvent_t **event);

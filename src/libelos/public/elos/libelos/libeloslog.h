// SPDX-License-Identifier: MIT
#pragma once

#include <elos/libelos/libelos.h>

#include "elos/event/event_types.h"

__BEGIN_DECLS

typedef struct elosLogger {
    elosSession_t *session;
} elosLogger_t;

/*******************************************************************
 * Function: elosInitLogger
 *------------------------------------------------------------------
 * Description:
 *      Initialize default logger with the given connection session
 * Input:
 *      - **session**:  session data structure used by other functions
 * Output:
 *      - None
 * Return:
 *      - `SAFU_RESULT_OK` for success or `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosInitLogger(elosSession_t *session);

/*******************************************************************
 * Function: elosDeleteLogger
 *------------------------------------------------------------------
 * Description:
 *      Delete default logger
 * Input:
 *      - **None**
 * Output:
 *      - **None**
 * Return:
 *      - **void**
 ******************************************************************/
void elosDeleteLogger();

/*******************************************************************
 * Function: elosLog
 *------------------------------------------------------------------
 * Description:
 *      Log a message as elos event with specified event parameters
 * Input:
 *      - **messageCode**:     	  message code of log event
 *      - **severity**:     	  severity of the logging event
 *      - **classification**:     classification for categorizing log event
 *      - **logMessage**:    	  message to be logged
 * Output:
 *      - **None**
 * Return:
 *      - **void**
 ******************************************************************/
void elosLog(elosEventMessageCodeE_t messageCode, elosSeverityE_t severity, uint64_t classification,
             const char *logMessage);

__END_DECLS

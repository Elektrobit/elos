// SPDX-License-Identifier: MIT
#pragma once

#include <safu/common.h>
#include <safu/flags.h>

#include "elos/eloslog/eloslog.h"
#include "elos/eventbuffer/eventbuffer.h"

/*******************************************************************
 * A representation of elosLogger
 *
 * Members:
 *      logEventBuffer : the event buffer to which the elos log event will be
 *                       temporarily stored before being permanently published
 *      flags          : flags of type `safuFlags_t` to indicate if the elosLogger
 *                       is initialized . It is atomically set when initialized
 *                       and reset when the logger is deleted.
 ******************************************************************/
typedef struct elosLogger {
    elosEventBuffer_t *logEventBuffer;
    safuFlags_t flags;
} elosLogger_t;

/*******************************************************************
 * Initializes the event buffer member of the provided logger and sets
 * the loggers flag to initialized.
 *
 * Parameters:
 *      logger (elosLogger_t *) : the logger which needs to be initialized.
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosLoggerInitialize(elosLogger_t *logger);

/*******************************************************************
 * Deletes the event buffer member of the provided logger and resets
 * the loggers flag to not initialized.
 *
 * Parameters:
 *      logger (elosLogger_t *) : the logger which needs iits members to be deleted.
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosLoggerDeleteMembers(elosLogger_t *logger);

/*******************************************************************
 * Initializes the given logger parameter by setting the logger flag to initialized
 * and initializing the loggers event buffer.
 *
 * Parameters:
 *      logger (elosLogger_t *) : the logger which needs its members to be deleted.
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosLoggerGetDefaultLogger(elosLogger_t **logger);

/*******************************************************************
 * Retrieves the elosd internal logger, creates the log event from provided
 * parameters and publishes them.
 *
 * Parameters:
 *      messageCode (elosEventMessageCodeE_t) : message code for the message to be logged.
 *      severity (elosSeverityE_t) : severity of the message which is to be logged.
 *      classification (uint64_t) : classification of the message to be logged.
 *      logMessage (const char *) : the message to be logged.
 ******************************************************************/
void elosLog(elosEventMessageCodeE_t messageCode, elosSeverityE_t severity, uint64_t classification,
             const char *logMessage);

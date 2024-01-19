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

__BEGIN_DECLS

/*******************************************************************
 * Initializes the event buffer member of the provided logger and sets
 * the loggers flag to initialized.
 * - If event buffer creation fails then the flag member of the provided
 *   logger is not set and the event buffer remains uninitialized.
 * - Since no event buffer or flags are set, clean up is not necessary.
 * - If the provided logger is already initialized, it does not matter
 *   the logger is initialized again.
 * - It is required that the logger is initialized only once during the
 *   entire time elos is running.

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
 * - If event buffer delete fails then the flag member of the provided
 *   logger is not set and the event buffer remains initialized
 * - Manually set the free event buffer and clear logger flags
 * - If the provided logger is already deleted, then the function tries
 *   to delete the logger again but will return a `SAFU_RESULT_FAILED`
 *   as event buffer is NULL and elosEventBufferDelete will fail.

 * Parameters:
 *      logger (elosLogger_t *) : the logger which needs its members to be deleted.
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosLoggerDeleteMembers(elosLogger_t *logger);

/*******************************************************************
 * Returns a pointer to the internal elos default logger.
 * - On Function call it checks if the default logger initialize flag is set and
 *   when not it sets it.
 * - It then calls `elosLoggerInitialize` to initialize the event buffer member
 *   of the default logger.
 * - If the initialization fails then the logger parameter is not set to the
 *   internal default logger.
 * - Upon successful initialization the logger parameter is set to point at the
 *   initialized default logger.
 * - Clean up is to be done i.e `elosLoggerDeleteMembers` should be done once
 *   in the lifetime of the process, so when done with the logger instance it is
 *   better to drop the logger instance.
 *
 * Parameters:
 *      logger (elosLogger_t i**) : the logger pointer which should point
 *                                  at the initialized default internal elos logger
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosLoggerGetDefaultLogger(elosLogger_t **logger);

/*******************************************************************
 * Publishes via the elosd internal logger, creates the log event from provided
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

__END_DECLS

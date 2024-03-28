// SPDX-License-Identifier: MIT

#pragma once

#include <elos/event/event.h>
#include <elos/libelosdlt/libelosdlt.h>
#include <pthread.h>

#include "elos/libelosdlt/types.h"

/*******************************************************************
 * Members:
 *     connectionString: path or address to used DLT interface
 *     ecuId: string to use as ecuId in DLT communication
 *     appId: string to use as appId in DLT communication
 ******************************************************************/
typedef struct elosDltLoggerBackendParam {
    const char *connectionString;
    const char *ecuId;
    const char *appId;
} elosDltLoggerBackendParam_t;

/*******************************************************************
 * Members:
 *     dlt: the DLT client structure
 *     socketPath: the absolute path to the DLT unix socket
 ******************************************************************/
typedef struct elosDltLoggerBackend {
    elosDltConnection_t dlt;
    uint8_t messageCount;
    pthread_mutex_t mutex;
} elosDltLoggerBackend_t;

/*******************************************************************
 * Allocate a new `elosDltLoggerBackend`. It shall be safe to call
 * `elosDltLoggerBackendDelete` even if `SAFU_RESULT_FAILED` is returned.
 * The new dlt is initialized by `elosDltLoggerBackendInit`.
 *
 * Parameters:
 *       dlt: pointer to a `elosDltLoggerBackend` pointer to be set to the
 *                new dlt. In case of an error it is unchanged.
 *       param: configuration options see elosDltLoggerBackendParam_t
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosDltLoggerBackendNew(elosDltLoggerBackend_t **dlt, elosDltLoggerBackendParam_t const *param);

/*******************************************************************
 * Initialize a `elosDltLoggerBackend`. It shall be safe to call
 * `elosDltLoggerBackendDelete` even if `SAFU_RESULT_FAILED` is returned.
 *
 * Parameters:
 *       dlt: pointer to a `elosDltLoggerBackend` to be initialized.
 *       param: configuration options see elosDltLoggerBackendParam_t
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosDltLoggerBackendNew(elosDltLoggerBackend_t **dlt, elosDltLoggerBackendParam_t const *param);

/*******************************************************************
 * Initialize a `elosDltLoggerBackend`. It shall be safe to call
 * `elosDltLoggerBackendDelete` even if `SAFU_RESULT_FAILED` is returned.
 *
 * Parameters:
 *       dlt: pointer to a `elosDltLoggerBackend` to be initialized.
 *       connectionString: Absolut path to the the DLT unix socket
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosDltLoggerBackendInit(elosDltLoggerBackend_t *dlt, elosDltLoggerBackendParam_t const *param);

/*******************************************************************
 * Delete a `elosDltLoggerBackend`. It also delete its members by calling
 * `eloDltLoggerBackendDelete`.
 *
 * Parameters:
 *       dlt: pointer to a `elosDltLoggerBackend` to be deleted.
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosDltLoggerBackendDelete(elosDltLoggerBackend_t *dlt);

/*******************************************************************
 * Delete and free all resources allocated by members of
 * `elosDltLoggerBackend`.
 *
 * Parameters:
 *       dlt: pointer to a `elosDltLoggerBackend` to be deleted.
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosDltLoggerBackendDeleteMembers(elosDltLoggerBackend_t *dlt);

/*******************************************************************
 * Start a `elosDltLoggerBackend` which actually connect to the DLT daemon.
 *
 * Parameters:
 *       dlt: pointer to a `elosDltLoggerBackend` to be started.
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosDltLoggerBackendStart(elosDltLoggerBackend_t *dlt);

/*******************************************************************
 * Stop a `elosDltLoggerBackend`. It shall be safe to call this function on an
 * already stopped instance.
 *
 * Parameters:
 *       dlt: pointer to a `elosDltLoggerBackend` to be stopped.
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosDltLoggerBackendStop(elosDltLoggerBackend_t *dlt);

/*******************************************************************
 * Store an event into DLT infrastructure.
 *
 * Parameters:
 *       dlt: pointer to a `elosDltLoggerBackend` to be stopped.
 *       event: the event to store
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosDltLoggerBackendPersist(elosDltLoggerBackend_t *dlt, const elosEvent_t *event);

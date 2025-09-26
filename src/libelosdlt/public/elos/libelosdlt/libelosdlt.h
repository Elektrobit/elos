// SPDX-License-Identifier: MIT
#pragma once

#include "elos/libelosdlt/extheader.h"
#include "elos/libelosdlt/payload.h"
#include "elos/libelosdlt/stdheader.h"
#include "elos/libelosdlt/storageheader.h"

__BEGIN_DECLS

/*******************************************************************
 * Connect to DLT daemon using connection info defined in `param`.
 * Lookup order is:
 * 1. param.pipePath
 * 2. param.socketPath
 * 3. param.connectionString
 *
 * Note: Currently only `pipe` connection mode is known to work for logging.
 *
 * Parameters:
 *       dlt: pointer to an instance of `elosDltConnection_t` to be connected.
 *       param: structure containing the connection parameter.
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosDltConnect(elosDltConnection_t *dlt, elosDltConnectionParam_t *param);

/*******************************************************************
 * Connect to DLT daemon, the `elosDltConnection_t` must have set `port` and
 * `host`.
 *
 * Parameters:
 *       dlt: pointer to an instance of `elosDltConnection_t` to be connected.
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosDltConnectTcp(elosDltConnection_t *dlt);

/*******************************************************************
 * Connect to DLT daemon, the `elosDltConnection_t` must have set `socketPath`.
 *
 * Parameters:
 *       dlt: pointer to an instance of `elosDltConnection_t` to be connected.
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosDltConnectUnix(elosDltConnection_t *dlt);

/*******************************************************************
 * Connect to DLT daemon, the `elosDltConnection_t` must have set `pipePath`.
 *
 * Parameters:
 *       dlt: pointer to an instance of `elosDltConnection_t` to be connected.
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosDltConnectPipe(elosDltConnection_t *dlt);

/*******************************************************************
 * Disconnect from DLT daemon by closing the connection. No session management
 * is done, means unregister any created application or session contexts must be
 * done in advance.
 *
 * Parameters:
 *       dlt: pointer to an instance of `elosDltConnection_t` to be disconnected.
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosDltDisconnect(elosDltConnection_t *dlt);

/*******************************************************************
 * Create an application and session context if not already existing.
 * For the application and session context the Ids specified in `dlt.appId` and
 * `dlt.contextId` are used.
 *
 * Parameters:
 *       dlt: pointer to an instance of `elosDltConnection_t` to holding the Ids
 *            to be registered.
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosDltRegisterContext(elosDltConnection_t *dlt);

/*******************************************************************
 * Unregister an application and session context if not already done.
 * For the application and session context the Ids specified in `dlt.appId` and
 * `dlt.contextId` are used.
 *
 * Parameters:
 *       dlt: pointer to an instance of `elosDltConnection_t` to holding the Ids
 *            to be unregistered.
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosDltUnregisterContext(elosDltConnection_t *dlt);

/*******************************************************************
 * Send a log message as `user message` to DLT daemon.
 *
 * Parameters:
 *       dlt: pointer to an instance of `elosDltConnection_t` to send send a
 *            log message to DLT daemon.
 *       payload: The string to be logged, must be a `\0`-terminated c-string.
 *                The string must be less then 128 bytes.
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosDltSendUserLog(elosDltConnection_t *dlt, char *payload);

/*******************************************************************
 * Send a log message as `control message` to DLT daemon.
 *
 * Parameters:
 *       dlt: pointer to an instance of `elosDltConnection_t` to send send a
 *            log message to DLT daemon.
 *       payload: The data to be logged.
 *       payloadLength: length of the payload, must be less the 128 byte.
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosDltSendControlMessage(elosDltConnection_t *dlt, const char *payload, size_t payloadLength);

/*******************************************************************
 * Read dlt buffer and unpack its contents into dltData.
 *
 * Parameters:
 *       dltBuffer: a buffer containing dlt data as bytes
 *       dltBufferSize: size of dltBuffer.
 *       parResult: result got from dlt header parsing.
 *       dltData: dlt struct into which the buffer data is parsed
 *
 * Returns:
 *      - `ELOS_DLT_RESULT_OK` as parResult on success
 *      - `ELOS_DLT_INCOMPLETE_HDR` as parResult on failure with dltBufferSize < Min. dltHeader size
 *      - `ELOS_DLT_INCOMPLETE_MSG` as parResult on failure with dltBufferSize < dltMessage Length
 *      - `ELOS_DLT_INVALID_MSG` as parResult on failure with not DLT magic bytes
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosDltReadData(unsigned char *dltBuffer, size_t dltBufferSize, elosDltParseResultE_t *parResult,
                              elosDltData_t *dltData);
__END_DECLS

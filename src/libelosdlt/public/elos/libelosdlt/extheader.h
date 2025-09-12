// SPDX-License-Identifier: MIT
#pragma once

#include "elos/libelosdlt/types.h"

__BEGIN_DECLS

/*******************************************************************
 * Represent a given message type and message type info as string
 *
 * Parameters:
 *       dltData: dlt data whose message type needs to represented as string
 *       dltMessageName : struct with string representation of dltData message type and info
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosDltGetMessageName(elosDltData_t *dltData, elosDltMessageName_t *dltMessageName);

/*******************************************************************
 * Parse extended header data from provided data buffer
 *
 * Parameters:
 *       data: a buffer containing dlt data as bytes
 *       dataLength: length of the data buffer
 *       dltData: dlt struct into which the buffer data is parsed
 *       parResult: result got when data buffer is parsed into extended header.
 *
 * Returns:
 *      - `ELOS_DLT_RESULT_OK` as parResult on success
 *      - `ELOS_DLT_INCOMPLETE_DLT_HDR` as parResult on failure with dltBufferSize < Min. dltHeader size
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosDltExtHeaderParse(unsigned char *data, size_t dataLength, elosDltData_t *dltData,
                                    elosDltParseResultE_t *parResult);

__END_DECLS

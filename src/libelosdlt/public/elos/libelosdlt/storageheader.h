// SPDX-License-Identifier: MIT
#pragma once

#include "elos/libelosdlt/types.h"

__BEGIN_DECLS

/*******************************************************************
 * Parse storage header data from provided data buffer
 *
 * Parameters:
 *       data: a buffer containing dlt data as bytes
 *       dataLength: length of the data buffer
 *       dltData: dlt struct into which the buffer data is parsed
 *       parResult: result got when data buffer is parsed into storage header.
 *
 * Returns:
 *      - `ELOS_DLT_RESULT_OK` as parResult on success
 *      - `ELOS_DLT_INCOMPLETE_DLT_HDR` as parResult on failure with dltBufferSize < Min. dltHeader size
 *      - `ELOS_DLT_INVALID_DLT_MSG` as parResult on failure with not DLT magic bytes or dlt message time not set
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosDltStorageHeaderParse(unsigned char *data, size_t dataLength, elosDltData_t *dltData,
                                        elosDltParseResultE_t *parResult);

__END_DECLS

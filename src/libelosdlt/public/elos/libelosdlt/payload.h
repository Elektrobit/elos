// SPDX-License-Identifier: MIT
#pragma once

#include "elos/libelosdlt/types.h"

__BEGIN_DECLS
/*******************************************************************
 * Parse payload data from provided data buffer
 *
 * Parameters:
 *       data: a buffer containing dlt data as bytes
 *       dataLength: length of the data buffer
 *       dltData: dlt struct into which the buffer data is parsed
 *       parResult: result got when data buffer is parsed into payload.
 *
 * Returns:
 *      - `ELOS_DLT_RESULT_OK` as parResult on success
 *      - `ELOS_DLT_INCOMPLETE_DLT_MSG` as parResult on failure with dltBufferSize < dltMessage Length
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosDltPayloadParse(unsigned char *data, size_t dataLength, elosDltData_t *dltData,
                                  elosDltParseResultE_t *parResult);

__END_DECLS

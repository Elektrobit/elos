// SPDX-License-Identifier: MIT
#pragma once

#include "elos/libelosdlt/types.h"

__BEGIN_DECLS
/*******************************************************************
 * Return the length of a standard header. A default value of 4 bytes
 * will be returned if no flags are set.
 *
 * Parameters:
 *       stdHeader: header whos size needs to b calculated
 *
 * Returns:
 *      - calculated standard header length on success
 *      - default value of 4 bytes if no flags set.
 ******************************************************************/
size_t elosDltGetStdHeaderLength(elosDltStdHeader_t *stdHeader);

/*******************************************************************
 * Parse standard header data from provided data buffer
 *
 * Parameters:
 *       data: a buffer containing dlt data as bytes
 *       dataLength: length of the data buffer
 *       dltData: dlt struct into which the buffer data is parsed
 *       parResult: result got when data buffer is parsed into standard header.
 *
 * Returns:
 *      - `ELOS_DLT_RESULT_OK` as parResult on success
 *      - `ELOS_DLT_INCOMPLETE_DLT_HDR` as parResult on failure with dltBufferSize < Min. dltHeader size
 *      - `ELOS_DLT_INVALID_DLT_VERSION` as parResult when dlt protocol version not equals 1
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosDltStdHeaderParse(unsigned char *data, size_t dataLength, elosDltData_t *dltData,
                                    elosDltParseResultE_t *parResult);

__END_DECLS

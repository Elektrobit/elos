// SPDX-License-Identifier: MIT

#include <bits/types/struct_iovec.h>
#include <limits.h>
#include <netinet/in.h>

#include "elos/libelosdlt/libelosdlt.h"

size_t elosDltGetStdHeaderLength(elosDltStdHeader_t *stdHeader) {
    size_t stdHeaderLength = STD_H_DATA_LENGTH;
    if (stdHeader->headerType.weid) {
        stdHeaderLength += 4;
    }
    if (stdHeader->headerType.wsid) {
        stdHeaderLength += 4;
    }
    if (stdHeader->headerType.wtms) {
        stdHeaderLength += 4;
    }
    return stdHeaderLength;
}

safuResultE_t elosDltStdHeaderParse(unsigned char *data, size_t dataLength, elosDltData_t *dltData,
                                    elosDltParseResultE_t *parResult) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    size_t expectedDataLength = 0;
    size_t byteIndex = 0;

    if (data != NULL && dltData != NULL && dataLength > 0) {
        if (dataLength >= expectedDataLength) {
            dltData->stdHeader.headerType.ueh = data[byteIndex] & USE_EXTENDED_HEADER_MASK;
            dltData->stdHeader.headerType.msbf = data[byteIndex] & MSB_FIRST_MASK;
            dltData->stdHeader.headerType.weid = data[byteIndex] & WITH_ECU_ID_MASK;
            dltData->stdHeader.headerType.wsid = data[byteIndex] & WITH_SESSION_ID_MASK;
            dltData->stdHeader.headerType.wtms = data[byteIndex] & WITH_TIME_STAMP_MASK;
            dltData->stdHeader.headerType.vers = (data[byteIndex] & VERSION_NUMBER_MASK) >> 5;
            expectedDataLength = elosDltGetStdHeaderLength(&dltData->stdHeader);
            byteIndex += 1;
        } else {
            safuLogErrF("Data length : %d is lesser that expected length : %d\n", dataLength, expectedDataLength);
            *parResult = ELOS_DLT_INCOMPLETE_DLT_HDR;
        }
        if (dltData->stdHeader.headerType.vers < 2) {
            if (dataLength >= expectedDataLength) {
                dltData->stdHeader.messageCounter = data[byteIndex];
                byteIndex += 1;
                memcpy(&dltData->stdHeader.length, &data[byteIndex], sizeof(dltData->stdHeader.length));
                dltData->stdHeader.length = ntohs(dltData->stdHeader.length);
                byteIndex += sizeof(dltData->stdHeader.length);
                if (dltData->stdHeader.headerType.weid) {
                    for (size_t i = 0; i < sizeof(dltData->stdHeader.ecuID); i++) {
                        dltData->stdHeader.ecuID[i] = data[byteIndex];
                        byteIndex += 1;
                    }
                }

                if (dltData->stdHeader.headerType.wsid) {
                    memcpy(&dltData->stdHeader.sessionId, &data[byteIndex], sizeof(dltData->stdHeader.sessionId));
                    dltData->stdHeader.sessionId = ntohl(dltData->stdHeader.sessionId);
                    byteIndex += sizeof(dltData->stdHeader.sessionId);
                }

                if (dltData->stdHeader.headerType.wtms) {
                    memcpy(&dltData->stdHeader.timestamp, &data[byteIndex], sizeof(dltData->stdHeader.timestamp));
                    dltData->stdHeader.timestamp = ntohl(dltData->stdHeader.timestamp);
                    byteIndex += sizeof(dltData->stdHeader.timestamp);
                }
                dltData->dltDataSize.standardHeaderSize = byteIndex;
                dltData->dltDataSize.dataSize += dltData->dltDataSize.standardHeaderSize;
                *parResult = ELOS_DLT_RESULT_OK;
                result = SAFU_RESULT_OK;
            } else {
                safuLogErrF("Data length : %d is lesser that expected length : %d\n", dataLength, expectedDataLength);
                *parResult = ELOS_DLT_INCOMPLETE_DLT_HDR;
            }
        } else {
            safuLogInfoF("Dlt Protocol Version %d is not supported by elos dlt parser\n",
                         dltData->stdHeader.headerType.vers);
            *parResult = ELOS_DLT_INVALID_DLT_VERSION;
        }
    } else {
        safuLogErr("Invalid parameters\n");
    }

    return result;
}

// SPDX-License-Identifier: MIT

#include "elos/libelosdlt/libelosdlt.h"

safuResultE_t elosDltStorageHeaderParse(unsigned char *data, size_t dataLength, elosDltData_t *dltData,
                                        elosDltParseResultE_t *parResult) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    size_t expectedDataLength = STO_H_DATA_LENGTH;
    size_t byteIndex = 0;

    if (data != NULL && dltData != NULL && dataLength > 0) {
        if (dataLength >= expectedDataLength) {
            if (memcmp(data, elosDltPattern, sizeof(elosDltPattern)) == 0) {
                byteIndex += sizeof(elosDltPattern);
                memcpy(&dltData->storageHeader.seconds, &data[byteIndex], sizeof(dltData->storageHeader.seconds));
                if (dltData->storageHeader.seconds != 0) {
                    byteIndex += sizeof(dltData->storageHeader.seconds);
                    memcpy(&dltData->storageHeader.microseconds, &data[byteIndex],
                           sizeof(dltData->storageHeader.microseconds));
                    if (dltData->storageHeader.microseconds != 0) {
                        byteIndex += sizeof(dltData->storageHeader.microseconds);
                        for (size_t i = 0; i < sizeof(dltData->storageHeader.ecuID); i++) {
                            dltData->storageHeader.ecuID[i] = data[byteIndex];
                            byteIndex += 1;
                        }
                        dltData->dltDataSize.storageHeaderSize = byteIndex;
                        dltData->dltDataSize.dataSize += dltData->dltDataSize.storageHeaderSize;
                        *parResult = ELOS_DLT_RESULT_OK;
                        result = SAFU_RESULT_OK;
                    }
                } else {
                    *parResult = ELOS_DLT_INVALID_DLT_MSG;
                }
            } else {
                *parResult = ELOS_DLT_INVALID_DLT_MSG;
            }
        } else {
            safuLogErrF("Data length : %d is lesser that expected length : %d\n", dataLength, expectedDataLength);
            *parResult = ELOS_DLT_INCOMPLETE_DLT_HDR;
        }
    } else {
        safuLogErr("Invalid parameters\n");
    }
    return result;
}

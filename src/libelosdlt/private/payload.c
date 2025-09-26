// SPDX-License-Identifier: MIT

#include "elos/libelosdlt/libelosdlt.h"

safuResultE_t elosDltPayloadParse(unsigned char *data, size_t dataLength, elosDltData_t *dltData,
                                  elosDltParseResultE_t *parResult) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    size_t payloadStringSize = 0;
    size_t byteIndex = 0;

    if (data != NULL && dltData != NULL && dataLength > 0) {
        if (dataLength < MESSAGE_ID_LENGTH) {
            *parResult = ELOS_DLT_INCOMPLETE_DLT_MSG;
        } else {
            memcpy(&dltData->payload.messageId, &data[byteIndex], sizeof(dltData->payload.messageId));
            byteIndex += (sizeof(dltData->payload.messageId));
            if (dataLength > MESSAGE_ID_LENGTH) {
                payloadStringSize = dataLength - sizeof(dltData->payload.messageId);
                dltData->payload.data = safuAllocMem(NULL, payloadStringSize);
                memcpy(dltData->payload.data, &data[byteIndex], payloadStringSize);
            }
            dltData->payload.size = payloadStringSize;
            dltData->dltDataSize.payloadSize = (byteIndex + payloadStringSize);
            dltData->dltDataSize.dataSize += dltData->dltDataSize.payloadSize;
            *parResult = ELOS_DLT_RESULT_OK;
            result = SAFU_RESULT_OK;
        }
    } else {
        safuLogErr("Invalid parameters\n");
    }
    return result;
}

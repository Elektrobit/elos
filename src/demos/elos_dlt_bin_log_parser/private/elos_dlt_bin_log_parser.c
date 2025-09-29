// SPDX-License-Identifier: MIT

#include <bits/types/struct_iovec.h>
#include <safu/common.h>
#include <safu/result.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "elos/libelosdlt/libelosdlt.h"

#define DLT_BUFFER_SIZE 255

void _printDltMessage(elosDltData_t *dltData) {
    elosDltMessageName_t dltMessageName = {0};
    time_t msgTimestamp = dltData->storageHeader.seconds + (dltData->storageHeader.microseconds / 1000000);
    struct tm *msgTimeInfo = localtime(&msgTimestamp);
    if (dltData->stdHeader.headerType.ueh) {
        elosDltGetMessageName(dltData, &dltMessageName);
    }
    printf(
        "%04d/%02d/%02d %02d:%02d:%02d.%06u %06u.%04u %d %s %s %s %d %s %s "
        "%s "
        "%d %d "
        "%.*s\n",
        msgTimeInfo->tm_year + 1900,  // Year since 1900
        msgTimeInfo->tm_mon + 1,      // Month (0-11)
        msgTimeInfo->tm_mday,         // Day of the month
        msgTimeInfo->tm_hour,         // Hour (0-23)
        msgTimeInfo->tm_min,          // Minute (0-59)
        msgTimeInfo->tm_sec,          // Second (0-60)
        dltData->storageHeader.microseconds, dltData->stdHeader.timestamp / 10000, dltData->stdHeader.timestamp % 10000,
        dltData->stdHeader.messageCounter, dltData->stdHeader.ecuID, dltData->extHeader.applicationId,
        dltData->extHeader.contextId, dltData->stdHeader.sessionId, dltMessageName.messageTypeName,
        dltMessageName.messageTypeInfoName, dltMessageName.isVerbose, dltData->extHeader.noOfArguments,
        dltData->payload.messageId, (int)dltData->payload.size, dltData->payload.data);

    if (dltData->stdHeader.headerType.ueh) {
        free(dltMessageName.messageTypeName);
        free(dltMessageName.messageTypeInfoName);
        free(dltMessageName.isVerbose);
    }
    free(dltData->payload.data);
}

int main(int argc, char *argv[]) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosDltParseResultE_t parseResult = ELOS_DLT_RESULT_OK;
    unsigned char *dltBuffer;
    size_t dltBufferSize = DLT_BUFFER_SIZE;
    char *dltLogFile = NULL;
    FILE *fp;
    size_t offset = 0;
    unsigned long fileSize = 0;
    int ret = -1;

    if (argc < 2) {
        printf("usage :\n %s <path to .dlt file>\n", argv[0]);
    } else {
        dltLogFile = strdup(argv[1]);
        if (dltLogFile != NULL) {
            printf("parsing binary log : %s\n", dltLogFile);
            fp = fopen(dltLogFile, "rb");
            if (fp != NULL) {
                if (fseek(fp, 0, SEEK_END) == 0) {
                    fileSize = ftell(fp);
                }
                fseek(fp, 0, SEEK_SET);
                dltBuffer = safuAllocMem(NULL, dltBufferSize);
                while (offset < fileSize) {
                    elosDltData_t dltData = {0};
                    if (fseek(fp, offset, SEEK_SET) == 0) {
                        if (fread(dltBuffer, dltBufferSize, 1, fp) > 0) {
                            result = elosDltReadData(dltBuffer, dltBufferSize, &parseResult, &dltData);
                            if (parseResult == ELOS_DLT_RESULT_OK && result == SAFU_RESULT_OK) {
                                _printDltMessage(&dltData);
                                offset += dltData.dltDataSize.dataSize;
                                dltBufferSize = dltBufferSize - dltData.dltDataSize.dataSize;
                                if (dltBufferSize == 0) {
                                    dltBufferSize += DLT_BUFFER_SIZE;
                                }
                                dltBuffer = safuAllocMem(dltBuffer, dltBufferSize);
                            } else {
                                dltBufferSize += DLT_BUFFER_SIZE;
                                if ((offset + dltBufferSize) > fileSize) {
                                    dltBufferSize = (fileSize - offset);
                                }
                                dltBuffer = safuAllocMem(dltBuffer, dltBufferSize);
                            }
                        }
                    }
                }
                free(dltBuffer);
            }
            free(dltLogFile);
        }
    }
    return ret;
}

// SPDX-License-Identifier: MIT

#include <arpa/inet.h>

#include "elos/libelosdlt/libelosdlt.h"

safuResultE_t elosDltGetMessageName(elosDltData_t *dltData, elosDltMessageName_t *dltMessageName) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (dltData->extHeader.messageInfo.verbose) {
        dltMessageName->isVerbose = strdup("verbose");
    } else {
        dltMessageName->isVerbose = strdup("non-verbose");
    }

    if (dltData->extHeader.messageInfo.messageType == ELOS_DLT_TYPE_LOG) {
        dltMessageName->messageTypeName = strdup("Log");
        switch (dltData->extHeader.messageInfo.messageTypeInfo.logInfo) {
            case ELOS_DLT_LOG_FATAL:
                dltMessageName->messageTypeInfoName = strdup("log fatal");
                result = SAFU_RESULT_OK;
                break;
            case ELOS_DLT_LOG_ERROR:
                dltMessageName->messageTypeInfoName = strdup("log error");
                result = SAFU_RESULT_OK;
                break;
            case ELOS_DLT_LOG_WARN:
                dltMessageName->messageTypeInfoName = strdup("log warn");
                result = SAFU_RESULT_OK;
                break;
            case ELOS_DLT_LOG_INFO:
                dltMessageName->messageTypeInfoName = strdup("log info");
                result = SAFU_RESULT_OK;
                break;
            case ELOS_DLT_LOG_DEBUG:
                dltMessageName->messageTypeInfoName = strdup("log debug");
                result = SAFU_RESULT_OK;
                break;
            case ELOS_DLT_LOG_VERBOSE:
                dltMessageName->messageTypeInfoName = strdup("log verbose");
                result = SAFU_RESULT_OK;
                break;
            default:
                safuLogWarnF("Invalid message type info: %d\n", dltData->extHeader.messageInfo.messageTypeInfo);
                break;
        }
    } else if (dltData->extHeader.messageInfo.messageType == ELOS_DLT_TYPE_APP_TRACE) {
        dltMessageName->messageTypeName = strdup("App Trace");
        switch (dltData->extHeader.messageInfo.messageTypeInfo.traceInfo) {
            case ELOS_DLT_TRACE_VARIABLE:
                dltMessageName->messageTypeInfoName = strdup("variable value");
                result = SAFU_RESULT_OK;
                break;
            case ELOS_DLT_TRACE_FUNCTION_IN:
                dltMessageName->messageTypeInfoName = strdup("function call");
                result = SAFU_RESULT_OK;
                break;
            case ELOS_DLT_TRACE_FUNCTION_OUT:
                dltMessageName->messageTypeInfoName = strdup("function return");
                result = SAFU_RESULT_OK;
                break;
            case ELOS_DLT_TRACE_STATE:
                dltMessageName->messageTypeInfoName = strdup("state-machine state");
                result = SAFU_RESULT_OK;
                break;
            case ELOS_DLT_TRACE_VFB:
                dltMessageName->messageTypeInfoName = strdup("RTE events");
                result = SAFU_RESULT_OK;
                break;
            default:
                safuLogWarnF("Invalid message type info: %d\n", dltData->extHeader.messageInfo.messageTypeInfo);
                break;
        }
    } else if (dltData->extHeader.messageInfo.messageType == ELOS_DLT_TYPE_NW_TRACE) {
        dltMessageName->messageTypeName = strdup("Network Trace");
        switch (dltData->extHeader.messageInfo.messageTypeInfo.networkInfo) {
            case ELOS_DLT_NW_TRACE_IPC:
                dltMessageName->messageTypeInfoName = strdup("nw-trace IPC");
                result = SAFU_RESULT_OK;
                break;
            case ELOS_DLT_NW_TRACE_CAN:
                dltMessageName->messageTypeInfoName = strdup("nw-trace CAN");
                result = SAFU_RESULT_OK;
                break;
            case ELOS_DLT_NW_TRACE_FLEXRAY:
                dltMessageName->messageTypeInfoName = strdup("nw-trace FLEYRAY");
                result = SAFU_RESULT_OK;
                break;
            case ELOS_DLT_NW_TRACE_MOST:
                dltMessageName->messageTypeInfoName = strdup("nw-trace MOST");
                result = SAFU_RESULT_OK;
                break;
            case ELOS_DLT_NW_TRACE_ETHERNET:
                dltMessageName->messageTypeInfoName = strdup("nw-trace ETHERNET");
                result = SAFU_RESULT_OK;
                break;
            case ELOS_DLT_NW_TRACE_SOMEIP:
                dltMessageName->messageTypeInfoName = strdup("nw-trace SOMEIP");
                result = SAFU_RESULT_OK;
                break;
            default:
                safuLogWarnF("Invalid message type info: %d\n", dltData->extHeader.messageInfo.messageTypeInfo);
                break;
        }

    } else if (dltData->extHeader.messageInfo.messageType == ELOS_DLT_TYPE_CONTROL) {
        dltMessageName->messageTypeName = strdup("Control");
        switch (dltData->extHeader.messageInfo.messageTypeInfo.controlInfo) {
            case ELOS_DLT_CONTROL_REQUEST:
                dltMessageName->messageTypeInfoName = strdup("request");
                result = SAFU_RESULT_OK;
                break;
            case ELOS_DLT_CONTROL_RESPONSE:
                dltMessageName->messageTypeInfoName = strdup("response");
                result = SAFU_RESULT_OK;
                break;
            case ELOS_DLT_CONTROL_TIME:
                dltMessageName->messageTypeInfoName = strdup("time");
                result = SAFU_RESULT_OK;
                break;
            default:
                safuLogWarnF("Invalid message type info: %d\n", dltData->extHeader.messageInfo.messageTypeInfo);
                break;
        }
    } else {
        safuLogWarnF("Invalid message type : %d\n", dltData->extHeader.messageInfo.messageInfo);
    }
    return result;
}

safuResultE_t elosDltExtHeaderParse(unsigned char *data, size_t dataLength, elosDltData_t *dltData,
                                    elosDltParseResultE_t *parResult) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    size_t expectedDataLength = EXT_H_DATA_LENGTH;
    size_t byteIndex = 0;

    if (data != NULL && dltData != NULL && dataLength > 0) {
        if (dataLength >= expectedDataLength) {
            dltData->extHeader.messageInfo.verbose = data[byteIndex] & VERBOSE_MASK;
            dltData->extHeader.messageInfo.messageType = (data[byteIndex] & MESSAGE_TYPE_MASK) >> _MESSAGE_TYPE_SHIFT;
            if (dltData->extHeader.messageInfo.messageType == ELOS_DLT_TYPE_LOG) {
                dltData->extHeader.messageInfo.messageTypeInfo.logInfo =
                    (data[byteIndex] & MESSAGE_TYPE_INFO_MASK) >> _MESSAGE_TYPE_INFO_SHIFT;
                result = SAFU_RESULT_OK;
            } else if (dltData->extHeader.messageInfo.messageType == ELOS_DLT_TYPE_APP_TRACE) {
                dltData->extHeader.messageInfo.messageTypeInfo.traceInfo =
                    (data[byteIndex] & MESSAGE_TYPE_INFO_MASK) >> _MESSAGE_TYPE_INFO_SHIFT;
                result = SAFU_RESULT_OK;
            } else if (dltData->extHeader.messageInfo.messageType == ELOS_DLT_TYPE_NW_TRACE) {
                dltData->extHeader.messageInfo.messageTypeInfo.networkInfo =
                    (data[byteIndex] & MESSAGE_TYPE_INFO_MASK) >> _MESSAGE_TYPE_INFO_SHIFT;
                result = SAFU_RESULT_OK;
            } else if (dltData->extHeader.messageInfo.messageType == ELOS_DLT_TYPE_CONTROL) {
                dltData->extHeader.messageInfo.messageTypeInfo.controlInfo =
                    (data[byteIndex] & MESSAGE_TYPE_INFO_MASK) >> _MESSAGE_TYPE_INFO_SHIFT;
                result = SAFU_RESULT_OK;
            } else {
                safuLogWarnF("Invalid message type : %d\n", dltData->extHeader.messageInfo.messageType);
            }

            if (result == SAFU_RESULT_OK) {
                byteIndex += 1;

                dltData->extHeader.noOfArguments = data[byteIndex];
                dltData->extHeader.noOfArguments = ntohs(dltData->extHeader.noOfArguments);
                byteIndex += 1;

                for (size_t i = 0; i < sizeof(dltData->extHeader.applicationId); i++) {
                    dltData->extHeader.applicationId[i] = data[byteIndex];
                    byteIndex += 1;
                }

                for (size_t j = 0; j < sizeof(dltData->extHeader.contextId); j++) {
                    dltData->extHeader.contextId[j] = data[byteIndex];
                    byteIndex += 1;
                }
                dltData->dltDataSize.extHeaderSize = byteIndex;
                dltData->dltDataSize.dataSize += dltData->dltDataSize.extHeaderSize;
                *parResult = ELOS_DLT_RESULT_OK;
                result = SAFU_RESULT_OK;
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

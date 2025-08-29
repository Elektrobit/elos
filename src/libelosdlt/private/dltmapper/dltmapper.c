// SPDX-License-Identifier: MIT

#include "elos/libelosdlt/dltmapper.h"

#include <bits/types/struct_iovec.h>
#include <safu/common.h>
#include <safu/result.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <time.h>

safuResultE_t elosDltMapperInit(elosDltMapper_t *mapper, char *fileName, char *appId, char *hardwareid, pid_t pid) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (mapper != NULL) {
        if (fileName == NULL) {
            mapper->fileName = NULL;
            result = SAFU_RESULT_OK;
        } else {
            mapper->fileName = strdup(fileName);
            if (mapper->fileName == NULL) {
                safuLogErr("strdup error");
            } else {
                result = SAFU_RESULT_OK;
            }
        }

        if (result == SAFU_RESULT_OK) {
            if (appId == NULL) {
                mapper->appId = NULL;
            } else {
                mapper->appId = strdup(appId);
                if (mapper->appId == NULL) {
                    safuLogErr("strdup error");
                    result = SAFU_RESULT_FAILED;
                }
            }
        }

        if (result == SAFU_RESULT_OK) {
            if (hardwareid == NULL) {
                mapper->hardwareid = strdup(safuGetHardwareId());
                if (mapper->hardwareid == NULL) {
                    safuLogErr("strdup error");
                } else {
                    result = SAFU_RESULT_OK;
                }
            } else {
                mapper->hardwareid = strdup(hardwareid);
                if (mapper->hardwareid == NULL) {
                    safuLogErr("strdup error");
                    result = SAFU_RESULT_FAILED;
                }
            }
        }

        if (result == SAFU_RESULT_OK) {
            if (pid != 0) {
                mapper->pid = pid;
            } else {
                mapper->pid = 0;
            }

            struct sysinfo elosSysInfo = {0};
            time_t elosBootTime = 0;
            if (sysinfo(&elosSysInfo) != 0) {
                safuLogErr("sysinfo startTime error");
                result = SAFU_RESULT_FAILED;
            } else {
                elosBootTime = time(NULL) - elosSysInfo.uptime;
                mapper->startTime.tv_sec = elosBootTime;
                mapper->startTime.tv_nsec = 0;
            }
        }
    }
    return result;
}

safuResultE_t elosDltMapperDeleteMembers(elosDltMapper_t *mapper) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    if (mapper != NULL) {
        free(mapper->fileName);
        free(mapper->appId);
        free(mapper->hardwareid);
        result = SAFU_RESULT_OK;
    }
    return result;
}

static bool _isIdEmpty(unsigned char *data) {
    bool ret = true;

    for (size_t i = 0; i < DLT_ID_SIZE; i++) {
        if (data[i] != 0) {
            ret = false;
            break;
        }
    }
    return ret;
}

safuResultE_t elosDltMapperMapDataToEvent(elosDltMapper_t *mapper, elosDltData_t *dltData, elosEvent_t *event) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    int ret = 0;
    bool isControlPayload = false;

    if (dltData == NULL || mapper == NULL || event == NULL) {
        safuLogErr("Invalid Parameter");
    } else {
        if (dltData->stdHeader.headerType.wtms) {
            if (dltData->stdHeader.timestamp != 0) {
                event->date.tv_sec = mapper->startTime.tv_sec + dltData->stdHeader.timestamp;
                event->date.tv_nsec = mapper->startTime.tv_nsec;
            }
        } else {
            if (dltData->storageHeader.seconds != 0) {
                event->date.tv_sec = dltData->storageHeader.seconds;
            }
            if (dltData->storageHeader.microseconds != 0) {
                event->date.tv_nsec = dltData->storageHeader.microseconds * 1000;
            }
        }
        if (dltData->stdHeader.headerType.ueh) {
            if (!_isIdEmpty(dltData->extHeader.applicationId)) {
                event->source.appName = safuAllocMem(NULL, DLT_ID_SIZE + 1);
                memcpy(event->source.appName, dltData->extHeader.applicationId,
                       sizeof(dltData->extHeader.applicationId) + 1);
                event->source.appName[strlen(event->source.appName)] = '\0';
            } else if (dltData->stdHeader.headerType.weid && (!_isIdEmpty(dltData->stdHeader.ecuID))) {
                char *appId = NULL;
                size_t appIdSize = strlen(mapper->appId) + sizeof(dltData->stdHeader.ecuID) + 2;
                appId = safuAllocMem(NULL, appIdSize);
                ret = snprintf(appId, appIdSize, "%s_%.*s", mapper->appId, DLT_ID_SIZE, dltData->stdHeader.ecuID);
                if (ret < 0) {
                    safuLogWarn("snprintf error, appId set to default");
                    event->source.appName = strdup(mapper->appId);
                } else {
                    event->source.appName = strdup(appId);
                    free(appId);
                }
            }

            if (dltData->extHeader.messageInfo.messageType == ELOS_DLT_TYPE_LOG) {
                event->classification = ELOS_CLASSIFICATION_LOG;
                switch (dltData->extHeader.messageInfo.messageTypeInfo.logInfo) {
                    case ELOS_DLT_LOG_FATAL:
                        event->severity = ELOS_SEVERITY_FATAL;
                        event->messageCode = ELOS_MSG_CODE_FATAL_LOG;
                        result = SAFU_RESULT_OK;
                        break;
                    case ELOS_DLT_LOG_ERROR:
                        event->severity = ELOS_SEVERITY_ERROR;
                        event->messageCode = ELOS_MSG_CODE_ERROR_LOG;
                        result = SAFU_RESULT_OK;
                        break;
                    case ELOS_DLT_LOG_WARN:
                        event->severity = ELOS_SEVERITY_WARN;
                        event->messageCode = ELOS_MSG_CODE_WARN_LOG;
                        result = SAFU_RESULT_OK;
                        break;
                    case ELOS_DLT_LOG_INFO:
                        event->severity = ELOS_SEVERITY_INFO;
                        event->messageCode = ELOS_MSG_CODE_INFO_LOG;
                        result = SAFU_RESULT_OK;
                        break;
                    case ELOS_DLT_LOG_DEBUG:
                        event->severity = ELOS_SEVERITY_DEBUG;
                        event->messageCode = ELOS_MSG_CODE_DEBUG_LOG;
                        result = SAFU_RESULT_OK;
                        break;
                    case ELOS_DLT_LOG_VERBOSE:
                        event->severity = ELOS_SEVERITY_VERBOSE;
                        event->messageCode = ELOS_MSG_CODE_VERBOSE_LOG;
                        result = SAFU_RESULT_OK;
                        break;
                    default:
                        safuLogWarnF("Invalid message type info: %d\n", dltData->extHeader.messageInfo.messageTypeInfo);
                        break;
                }
            } else if (dltData->extHeader.messageInfo.messageType == ELOS_DLT_TYPE_APP_TRACE) {
                event->severity = ELOS_SEVERITY_OFF;
                event->classification = ELOS_CLASSIFICATION_PROCESS;
                event->messageCode = ELOS_MSG_CODE_TRACE_LOG;
                result = SAFU_RESULT_OK;

            } else if (dltData->extHeader.messageInfo.messageType == ELOS_DLT_TYPE_NW_TRACE) {
                event->severity = ELOS_SEVERITY_OFF;
                event->classification = ELOS_CLASSIFICATION_NETWORK;
                event->messageCode = ELOS_MSG_CODE_TRACE_LOG;
                result = SAFU_RESULT_OK;
            } else {
                isControlPayload = true;
                event->severity = ELOS_SEVERITY_OFF;
                event->classification = ELOS_CLASSIFICATION_IPC;
                event->messageCode = ELOS_MSG_CODE_TRACE_LOG;
                size_t psize = 0;
                result =
                    safuBase64Encode(dltData->payload.data, dltData->dltDataSize.dataSize, &event->payload, &psize);
                if (result == SAFU_RESULT_FAILED || psize == 0) {
                    safuLogWarnF("Base64 Encoding of : %s Failed\n", dltData->payload.data);
                }
            }
        } else {
            if (mapper->appId == NULL) {
                safuLogWarn("default appId is NULL, event appName is not set");
            } else {
                event->source.appName = strdup(mapper->appId);
                result = SAFU_RESULT_OK;
            }
            event->severity = ELOS_SEVERITY_OFF;
            event->classification = ELOS_CLASSIFICATION_LOG;
            event->messageCode = ELOS_MSG_CODE_FORWARDED_LOG;
        }
        if (result == SAFU_RESULT_OK) {
            if (!isControlPayload) {
                event->payload = safuAllocMem(NULL, dltData->dltDataSize.dataSize + 1);
                ret = snprintf(event->payload, dltData->dltDataSize.dataSize + 1, "%s", dltData->payload.data);
                if (ret < 0) {
                    safuLogWarn("snprintf error, payload setting failed");
                } else {
                    event->payload[strlen(event->payload)] = '\0';
                }
            }
            if (mapper->fileName == NULL) {
                safuLogWarn("default fileName is NULL, event fileName is not set");
                result = SAFU_RESULT_FAILED;
            } else {
                event->source.fileName = strdup(mapper->fileName);
                if (mapper->hardwareid == NULL) {
                    safuLogWarn("default hardwareid is NULL, event hardwareid is not set");
                    result = SAFU_RESULT_FAILED;
                } else {
                    event->hardwareid = strdup(mapper->hardwareid);
                    result = SAFU_RESULT_OK;
                }
            }
        }
        event->source.pid = mapper->pid;
    }

    return result;
}

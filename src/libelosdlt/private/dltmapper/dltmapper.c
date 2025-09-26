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

static safuResultE_t _setStringToField(char **field, const char *string, bool isEmptyOk) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (string == NULL) {
        if (isEmptyOk == true) {
            result = SAFU_RESULT_OK;
        } else {
            safuLogErr("string empty")
        }
    } else {
        *field = strdup(string);
        if (*field == NULL) {
            safuLogErr("strdup error")
        } else {
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

static safuResultE_t _setStartTimeToField(struct timespec *field) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    struct sysinfo elosSysInfo = {0};
    time_t elosBootTime = 0;
    if (sysinfo(&elosSysInfo) != 0) {
        safuLogErr("sysinfo startTime error");
    } else {
        elosBootTime = time(NULL) - elosSysInfo.uptime;
        field->tv_sec = elosBootTime;
        field->tv_nsec = 0;
        result = SAFU_RESULT_OK;
    }

    return result;
}

safuResultE_t elosDltMapperInit(elosDltMapper_t *mapper, char *fileName, char *appId, char *hardwareid, pid_t pid) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (mapper == NULL) {
        safuLogErr("Invalid Parameter");
    } else {
        mapper->pid = pid;
        result = _setStartTimeToField(&mapper->startTime);
        if (result != SAFU_RESULT_OK) {
            safuLogWarn("Could not set start time in dlt mapper");
        } else {
            result = _setStringToField(&mapper->fileName, fileName, true);
            if (result != SAFU_RESULT_OK) {
                safuLogWarn("Could not set filename in dlt mapper");
            } else {
                result = _setStringToField(&mapper->appId, appId, true);
                if (result != SAFU_RESULT_OK) {
                    safuLogWarn("Could not set appId in dlt mapper");
                } else {
                    if (hardwareid == NULL) {
                        mapper->hardwareid = strdup(safuGetHardwareId());
                    }
                    result = _setStringToField(&mapper->hardwareid, hardwareid, true);
                    if (result != SAFU_RESULT_OK) {
                        safuLogWarn("Could not set hardwareid in dlt mapper");
                    }
                }
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

static void _mapTimeToEvent(elosDltMapper_t *mapper, elosDltData_t *dltData, elosEvent_t *event) {
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
}

static safuResultE_t _mapMsgTypeToEvent(elosDltData_t *dltData, elosEvent_t *event, bool *isControlPayload) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (dltData->stdHeader.headerType.ueh) {
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
            *isControlPayload = true;
            event->severity = ELOS_SEVERITY_OFF;
            event->classification = ELOS_CLASSIFICATION_IPC;
            event->messageCode = ELOS_MSG_CODE_TRACE_LOG;
            result = SAFU_RESULT_OK;
        }
    } else {
        event->severity = ELOS_SEVERITY_OFF;
        event->classification = ELOS_CLASSIFICATION_LOG;
        event->messageCode = ELOS_MSG_CODE_FORWARDED_LOG;
        result = SAFU_RESULT_OK;
    }

    return result;
}

static safuResultE_t _mapAppIdToEvent(elosDltMapper_t *mapper, elosDltData_t *dltData, elosEvent_t *event) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (dltData->stdHeader.headerType.ueh) {
        if (!_isIdEmpty(dltData->extHeader.applicationId)) {
            event->source.appName = safuAllocMem(NULL, 5);
            if (event->source.appName == NULL) {
                safuLogErr("allocation failed");
            } else {
                memcpy(event->source.appName, dltData->extHeader.applicationId, 4);
                event->source.appName[4] = '\0';
                result = SAFU_RESULT_OK;
            }
        } else if (dltData->stdHeader.headerType.weid && (!_isIdEmpty(dltData->stdHeader.ecuID))) {
            char *appId = NULL;
            size_t appIdSize = strlen(mapper->appId) + sizeof(dltData->stdHeader.ecuID) + 2;
            appId = safuAllocMem(NULL, appIdSize);
            if (appId == NULL) {
                safuLogErr("allocation failed");
            } else {
                int ret = snprintf(appId, appIdSize, "%s_%.*s", mapper->appId, DLT_ID_SIZE, dltData->stdHeader.ecuID);
                if (ret < 0 || (size_t)ret >= appIdSize) {
                    safuLogWarn("snprintf error, appId set to default");
                    result = _setStringToField(&event->source.appName, mapper->appId, false);
                } else {
                    result = _setStringToField(&event->source.appName, appId, false);
                }
                free(appId);
            }

        } else {
            result = _setStringToField(&event->source.appName, mapper->appId, false);
        }
    } else {
        result = _setStringToField(&event->source.appName, mapper->appId, false);
    }

    return result;
}

safuResultE_t _mapDefaultsToEvent(elosDltMapper_t *mapper, elosEvent_t *event) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    event->source.pid = mapper->pid;
    result = _setStringToField(&event->source.fileName, mapper->fileName, false);
    if (result != SAFU_RESULT_OK) {
        safuLogWarn("Could not set filename in event");
    } else {
        result = _setStringToField(&event->hardwareid, mapper->hardwareid, false);
        if (result != SAFU_RESULT_OK) {
            safuLogWarn("Could not set hardwareid in event");
        }
    }

    return result;
}

safuResultE_t _mapPayloadToEvent(elosDltData_t *dltData, elosEvent_t *event, bool *isControlPayload) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (*isControlPayload) {
        size_t psize = 0;
        result = safuBase64Encode(dltData->payload.data, dltData->payload.size, &event->payload, &psize);
        if (result == SAFU_RESULT_FAILED || psize == 0) {
            safuLogWarnF("Base64 Encoding of : %s Failed\n", dltData->payload.data);
        }
    } else {
        event->payload = safuAllocMem(NULL, dltData->payload.size + 1);
        if (event->payload != NULL) {
            memcpy(event->payload, dltData->payload.data, dltData->payload.size);
            event->payload[dltData->payload.size] = '\0';
            result = SAFU_RESULT_OK;
        } else {
            safuLogWarn("alloc error, payload setting failed");
        }
    }

    return result;
}

safuResultE_t elosDltMapperMapDataToEvent(elosDltMapper_t *mapper, elosDltData_t *dltData, elosEvent_t *event) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    bool isControlPayload = false;

    if (dltData == NULL || mapper == NULL || event == NULL) {
        safuLogErr("Invalid Parameter");
    } else {
        _mapTimeToEvent(mapper, dltData, event);
        result = _mapAppIdToEvent(mapper, dltData, event);
        if (result != SAFU_RESULT_OK) {
            safuLogWarn("Could not map App Id to event");
        } else {
            result = _mapMsgTypeToEvent(dltData, event, &isControlPayload);
            if (result != SAFU_RESULT_OK) {
                safuLogWarn("Could not map Msg Type to event");
            } else {
                result = _mapPayloadToEvent(dltData, event, &isControlPayload);
                if (result != SAFU_RESULT_OK) {
                    safuLogWarn("Could not map payload to event");
                } else {
                    result = _mapDefaultsToEvent(mapper, event);
                }
            }
        }
    }

    return result;
}

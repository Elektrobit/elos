// SPDX-License-Identifier: MIT

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "elos/scanner_manager/scanner_manager.h"

#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "elos/config/config.h"
#include "elos/eventbuffer/eventbuffer.h"
#include "elos/eventdispatcher/eventdispatcher.h"
#include "elos/eventlogging/LogAggregator.h"
#include "elos/scanner/scanner.h"
#include "safu/common.h"
#include "safu/log.h"

#define SO_EXT     ".so"
#define SO_EXT_LEN 3

static void *_scannerThread(void *ptr) {
    elosScannerEntry_t *entry = ptr;

    entry->state = SCANNER_RUNNING;
    entry->funcRun(&entry->session);
    entry->state = SCANNER_STOPPED;

    pthread_exit(NULL);
}

static elosScannerManagerErrorCodeE_t _parsePath(elosScannerManagerContext_t *context, const char *path) {
    elosScannerManagerErrorCodeE_t errorCode = NO_ERROR;
    DIR *dirHandle = NULL;
    struct dirent *dirEnt = NULL;
    int ret;

    if ((context == NULL) || (path == NULL)) {
        errorCode = ERROR_ZERO_POINTER;
        safuLogErr("invalid parameter");
    }

    if (errorCode == NO_ERROR) {
        dirHandle = opendir(path);
        if (dirHandle == NULL) {
            errorCode = ERROR_SCANNER_PATH_INVALID;
            safuLogErrF("opendir %s - %s", path, strerror(errno));
        }
    }

    if (errorCode == NO_ERROR) {
        dirEnt = readdir(dirHandle);
        while (dirEnt != NULL) {
            size_t len;

            if (dirEnt->d_type == DT_DIR) {
                elosScannerManagerErrorCodeE_t tmpErrorCode;
                const char *name = dirEnt->d_name;
                char *subpath = NULL;

                if ((strcmp(name, "..") == 0) || (strcmp(name, ".") == 0)) {
                    dirEnt = readdir(dirHandle);
                    continue;
                }

                ret = asprintf(&subpath, "%s/%s", path, name);
                if (ret < 0) {
                    errorCode = ERROR_ALLOCATION;
                    safuLogErrErrno("asprintf");
                    break;
                }

                tmpErrorCode = _parsePath(context, subpath);
                if (tmpErrorCode != NO_ERROR) {
                    errorCode = tmpErrorCode;
                }

                free(subpath);

                dirEnt = readdir(dirHandle);
                continue;
            } else if (dirEnt->d_type != DT_REG) {
                dirEnt = readdir(dirHandle);
                continue;
            }

            len = strlen(dirEnt->d_name);
            if ((len > SO_EXT_LEN) && (strcmp(SO_EXT, &(dirEnt->d_name[len - SO_EXT_LEN])) == 0)) {
                elosScannerEntry_t entry = {.state = SCANNER_INVALID};

                ret = asprintf(&entry.file, "%s/%s", path, dirEnt->d_name);
                if (ret < 0) {
                    errorCode = ERROR_ALLOCATION;
                    safuLogErrErrno("asprintf");
                    break;
                }

                ret = safuVecPush(&context->scannerEntry, &entry);
                if (ret < 0) {
                    errorCode = ERROR_VEC_MANIPULATION;
                    safuLogErr("safuVecPush failed");
                    break;
                }
            }

            dirEnt = readdir(dirHandle);
        }
    }

    if (dirHandle != NULL) {
        ret = closedir(dirHandle);
        if (ret < 0) {
            safuLogErrErrno("closedir");
            errorCode = NO_FATAL_ERRORS;
        }
    }

    return errorCode;
}

static elosScannerManagerErrorCodeE_t _entryFree(elosScannerManagerContext_t *context, elosScannerEntry_t *entry) {
    elosScannerManagerErrorCodeE_t errorCode = NO_ERROR;
    safuResultE_t result;
    int ret;

    result = elosEventDispatcherBufferRemove(context->eventDispatcher, &entry->session.eventBuffer);
    if (result != SAFU_RESULT_OK) {
        safuLogWarn("Removing EventBuffer from EventDispatcher failed");
        errorCode = ERROR_FUNCTION_CALL;
    }

    result = elosEventBufferDeleteMembers(&entry->session.eventBuffer);
    if (result != SAFU_RESULT_OK) {
        safuLogWarn("Deleting EventBuffer members failed");
        errorCode = ERROR_FUNCTION_CALL;
    }

    if (entry->file != NULL) {
        free(entry->file);
        entry->file = NULL;
    }

    if (entry->dlHandle != NULL) {
        ret = dlclose(entry->dlHandle);
        entry->dlHandle = NULL;
        if (ret < 0) {
            errorCode = NO_FATAL_ERRORS;
            safuLogErrErrno("dlclose");
        }
    }

    return errorCode;
}

static safuResultE_t _eventPublish(elosScannerCallbackData_t *data, const elosEvent_t *event) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((data == NULL) || (data->eventBuffer == NULL) || (event == NULL)) {
        safuLogErr("invalid parameter");
    } else {
        safuLogDebug("publish event");
        result = elosEventBufferWrite(data->eventBuffer, event);
        safuLogDebug("publish event done");
        if (result != SAFU_RESULT_OK) {
            safuLogErr("elosEventProcessorPublish failed!");
        }
    }

    return result;
}

static safuResultE_t _eventLog(elosScannerCallbackData_t *data, const elosEvent_t *event) {
    elosLogAggregator_t *logAggregator;
    safuResultE_t result = SAFU_RESULT_OK;

    if (data == NULL) {
        safuLogErr("invalid parameter");
        return SAFU_RESULT_FAILED;
    }

    logAggregator = data->logAggregator;

    result = elosLogAggregatorAdd(logAggregator, event);
    if (result != SAFU_RESULT_OK) {
        safuLogErr("elosLogAggregatorAdd failed!");
        return SAFU_RESULT_FAILED;
    }

    return result;
}

int elosScannerManagerStart(elosScannerManagerContext_t *context, elosScannerManagerParam_t const *param) {
    elosScannerManagerErrorCodeE_t errorCode = NO_ERROR;
    const char *scannerPath = "";
    uint32_t scannerCount = 0;
    int ret;

    if ((context == NULL) || (param == NULL)) {
        safuLogErr("Null pointer given");
        errorCode = ERROR_ZERO_POINTER;
    } else if ((param->config == NULL) || (param->logAggregator == NULL)) {
        safuLogErr("'param' has invalid values");
        errorCode = ERROR_ZERO_POINTER;
    } else {
        context->eventDispatcher = param->eventDispatcher;
        context->logAggregator = param->logAggregator;
        context->config = param->config;
    }

    if (errorCode == NO_ERROR) {
        ret = safuVecCreate(&context->scannerEntry, SCANNER_ENTRIES_INIT, sizeof(elosScannerEntry_t));
        if (ret < 0) {
            errorCode = ERROR_VEC_MANIPULATION;
            safuLogErr("safuVecCreate");
        }
    }

    if (errorCode == NO_ERROR) {
        scannerPath = elosConfigGetElosdScannerPath(param->config);
        if (scannerPath == NULL) {
            scannerPath = ELOSD_SCANNER_PATH;
        }

        errorCode = _parsePath(context, scannerPath);
    }

    if ((errorCode == NO_ERROR) || (errorCode == NO_FATAL_ERRORS)) {
        scannerCount = safuVecElements(&context->scannerEntry);
        if (scannerCount == 0) {
            errorCode = ERROR_SCANNER_PATH_EMPTY;
            safuLogErrF("no scanner .so found in path '%s'", scannerPath);
        }
    }

    if ((errorCode == NO_ERROR) || (errorCode == NO_FATAL_ERRORS)) {
        uint32_t i;
        for (i = 0; i < scannerCount; i++) {
            elosScannerEntry_t *entry;
            char *errorStr;

            entry = safuVecGet(&context->scannerEntry, i);
            if (entry == NULL) {
                errorCode = ERROR_VEC_MANIPULATION;
                safuLogErrF("safuVecGet[%d] failed", i);
                break;
            }

            entry->dlHandle = dlopen(entry->file, RTLD_LAZY);
            errorStr = dlerror();
            if ((entry->dlHandle == NULL) || (errorStr != NULL)) {
                errorCode = NO_FATAL_ERRORS;
                entry->state = SCANNER_ERROR_FATAL_DLOPEN;
                safuLogErrF("dlopen: %s", errorStr);
                continue;
            }

            *(void **)(&entry->funcInitialize) = dlsym(entry->dlHandle, "elosScannerInitialize");
            errorStr = dlerror();
            if ((entry->funcInitialize == NULL) || (errorStr != NULL)) {
                errorCode = NO_FATAL_ERRORS;
                entry->state = SCANNER_ERROR_FATAL_DLSYM;
                safuLogErrF("dlsym: %s", errorStr);
                continue;
            }

            *(void **)(&entry->funcRun) = dlsym(entry->dlHandle, "elosScannerRun");
            errorStr = dlerror();
            if ((entry->funcRun == NULL) || (errorStr != NULL)) {
                errorCode = NO_FATAL_ERRORS;
                entry->state = SCANNER_ERROR_FATAL_DLSYM;
                safuLogErrF("dlsym: %s", errorStr);
                continue;
            }

            *(void **)(&entry->funcStop) = dlsym(entry->dlHandle, "elosScannerStop");
            errorStr = dlerror();
            if ((entry->funcStop == NULL) || (errorStr != NULL)) {
                errorCode = NO_FATAL_ERRORS;
                entry->state = SCANNER_ERROR_FATAL_DLSYM;
                safuLogErrF("dlsym: %s", errorStr);
                continue;
            }

            *(void **)(&entry->funcFree) = dlsym(entry->dlHandle, "elosScannerFree");
            errorStr = dlerror();
            if ((entry->funcFree == NULL) || (errorStr != NULL)) {
                errorCode = NO_FATAL_ERRORS;
                entry->state = SCANNER_ERROR_FATAL_DLSYM;
                safuLogErrF("dlsym: %s", errorStr);
                continue;
            }

            entry->state = SCANNER_PARTIALLY_INITIALIZED;
        }
    }

    if ((errorCode == NO_ERROR) || (errorCode == NO_FATAL_ERRORS)) {
        uint32_t i;
        for (i = 0; i < scannerCount; i++) {
            elosScannerParam_t scannerParam = {.config = context->config};
            elosEventBufferParam_t bufferParam = {.limitEventCount = ELOS_EVENTBUFFER_DEFAULT_LIMIT};
            elosScannerEntry_t *entry;
            safuResultE_t result;
            const char *fileName = NULL;

            entry = safuVecGet(&context->scannerEntry, i);
            if (entry == NULL) {
                errorCode = ERROR_VEC_MANIPULATION;
                entry->state = SCANNER_ERROR_FATAL;
                safuLogErrF("safuVecGet[%d] failed", i);
                break;
            }

            if (entry->state != SCANNER_PARTIALLY_INITIALIZED) {
                continue;
            }

            fileName = strrchr(entry->file, '/') + 1;
            safuLogDebugF("initialize scanner '%s' ...", fileName);

            result = elosEventBufferInitialize(&entry->session.eventBuffer, &bufferParam);
            if (result != SAFU_RESULT_OK) {
                safuLogErrValue("Creating EventBuffer failed", result);
                errorCode = ERROR_FUNCTION_CALL;
                break;
            } else {
                result = elosEventDispatcherBufferAdd(context->eventDispatcher, &entry->session.eventBuffer);
                if (result != SAFU_RESULT_OK) {
                    safuLogErrValue("Adding EventBuffer to EventDispatcher failed", result);
                    errorCode = ERROR_FUNCTION_CALL;
                    break;
                }
            }

            entry->session.callback.eventPublish = _eventPublish;
            entry->session.callback.eventLog = _eventLog;
            entry->session.callback.scannerCallbackData.eventBuffer = &entry->session.eventBuffer;
            entry->session.callback.scannerCallbackData.logAggregator = context->logAggregator;

            ret = entry->funcInitialize(&entry->session, &scannerParam);
            if (ret == 0) {
                entry->state = SCANNER_INITIALIZED;
            } else {
                errorCode = NO_FATAL_ERRORS;
                safuLogErrF("scanner_initialize with '%s' failed", fileName);
            }
        }
    }

    if ((errorCode == NO_ERROR) || (errorCode == NO_FATAL_ERRORS)) {
        uint32_t i;
        for (i = 0; i < scannerCount; i++) {
            elosScannerEntry_t *entry;

            entry = safuVecGet(&context->scannerEntry, i);
            if (entry == NULL) {
                errorCode = ERROR_VEC_MANIPULATION;
                entry->state = SCANNER_ERROR_FATAL;
                safuLogErrF("safuVecGet[%d] failed", i);
                break;
            }

            if (entry->state != SCANNER_INITIALIZED) {
                continue;
            }

            ret = pthread_create(&entry->thread, 0, _scannerThread, entry);
            if (ret != 0) {
                errorCode = NO_FATAL_ERRORS;
                entry->state = SCANNER_ERROR_FATAL;
                safuLogErrErrno("pthread_create failed");
                continue;
            }
        }
    }

    if ((errorCode != NO_ERROR) && (errorCode != NO_FATAL_ERRORS)) {
        elosScannerManagerStop(context);
    }

    return errorCode;
}

int elosScannerManagerStop(elosScannerManagerContext_t *context) {
    elosScannerManagerErrorCodeE_t errorCode = NO_ERROR;
    uint32_t scannerCount = 0;

    if (context == NULL) {
        safuLogErr("invalid parameter");
        errorCode = ERROR_ZERO_POINTER;
    } else {
        safuLogDebug("stopping scanners...");
    }

    if (errorCode == NO_ERROR) {
        scannerCount = safuVecElements(&context->scannerEntry);
    }

    if ((errorCode == NO_ERROR) && (scannerCount > 0)) {
        uint32_t i;
        for (i = 0; i < scannerCount; i++) {
            elosScannerEntry_t *entry;
            int ret;

            entry = safuVecGet(&context->scannerEntry, i);
            if (entry == NULL) {
                errorCode = ERROR_VEC_MANIPULATION;
                safuLogErrF("safuVecGet[%d] failed", i);
                break;
            }

            if (entry->state == SCANNER_RUNNING) {
                safuLogDebugF("stopping scanner '%s' ...", strrchr(entry->file, '/') + 1);

                ret = entry->funcStop(&entry->session);
                if (ret < 0) {
                    safuLogErrF("scanner_stop[%d] failed", i);
                    errorCode = NO_FATAL_ERRORS;
                }

                ret = pthread_join(entry->thread, NULL);
                if (ret < 0) {
                    safuLogErrErrno("pthread_join");
                    errorCode = NO_FATAL_ERRORS;
                }
            } else {
                safuLogErrF("Scanner not running '%s' ...", strrchr(entry->file, '/') + 1);
            }

            ret = entry->funcFree(&entry->session);
            if (ret < 0) {
                safuLogErrF("scanner_free[%d] failed", i);
                errorCode = NO_FATAL_ERRORS;
            }

            ret = _entryFree(context, entry);
            if (ret != NO_ERROR) {
                safuLogErrF("scanner_free[%d] failed", i);
                errorCode = ret;
            }
        }
    }

    if (context != NULL) {
        safuVecFree(&context->scannerEntry);
    }

    safuLogDebug("finished");

    return errorCode;
}

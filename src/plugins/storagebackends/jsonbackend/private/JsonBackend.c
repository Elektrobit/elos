// SPDX-License-Identifier: MIT
#include "JsonBackend.h"

#include <fcntl.h>
#include <safu/common.h>
#include <safu/log.h>
#include <safu/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "elos/eventfilter/eventfilter.h"

#define BACKEND_NAME "JSON"

static inline bool _matchAndReplaceMacro(char **str, const char *const path, const char *const macro,
                                         const char *const val, size_t *j, size_t *i, const size_t sizelimit) {
    bool retVal = false;
    if (str == NULL) {
        safuLogErr("Trying to write macro value to NULL string");
    } else if (path == NULL) {
        safuLogErr("Trying to replace macro in NULL string");
    } else if (j == NULL) {
        safuLogErr("Output counter pointer NULL");
    } else if (i == NULL) {
        safuLogErr("Input counter pointer NULL");
    } else if (macro == NULL) {
        safuLogWarn("Called _matchAndReplaceMacro with NULL macro, skipping checks");
    } else {
        size_t macroLen = strlen(macro);
        // ensure macro AND \0 can be contained in rest of string
        if (*i < (strlen(path) - (macroLen + 1))) {
            if (strncmp(path + (*i), macro, macroLen) == 0) {
                if (val == NULL) {
                    safuLogErr("Trying to replace maro with NULL value");
                } else if ((*j + strlen(val)) < sizelimit) {
                    retVal = true;
                    strcpy(*str + *j, val);
                    *j += strlen(val);
                    *i += macroLen;
                } else {
                    *j += strlen(val);  // force storagePath to use default location.
                }
            }
        }
    }
    return retVal;
}

static inline char *_completeStoragePath(const char *const path, size_t sizelimit, const char *countStr,
                                         const char *const date, bool *hasCount, int recCounter) {
    *hasCount = false;
    if (countStr == NULL) {
        safuLogWarn("Called elosCompleteStoragePath with count String NULL, defaulting to \"0\"");
        countStr = "0";
    }
    if (recCounter >= 2) {
        return strdup(STORAGE_LOCATION);
    }
    if (path == NULL) {
        safuLogErr("Invalid argument: Path is NULL");
        return _completeStoragePath(STORAGE_LOCATION, strlen(STORAGE_LOCATION) + strlen(countStr) + 1, countStr, date,
                                    hasCount, recCounter + 1);
    }
    char *out = safuAllocMem(NULL, sizeof(char) * sizelimit);
    if (out == NULL) {
        safuLogErr("Failed to allocate storage path memory.");
        return _completeStoragePath(STORAGE_LOCATION, strlen(STORAGE_LOCATION) + strlen(countStr) + 1, countStr, date,
                                    hasCount, recCounter + 1);
    }
    size_t j = 0;
    size_t i, k;
    const char *hardwareId = safuGetHardwareId();
    const size_t pathLen = strlen(path);
    for (i = 0; i < pathLen; i++) {
        if (j >= sizelimit) {
            safuLogWarnF(
                "completed storage path would have gone beyond defined path length limitations: %lu. Defaulting to %s.",
                sizelimit, STORAGE_LOCATION);
            free(out);
            return _completeStoragePath(STORAGE_LOCATION, strlen(STORAGE_LOCATION) + strlen(countStr) + 1, countStr,
                                        date, hasCount, recCounter + 1);
        }
        bool written = _matchAndReplaceMacro(&out, path, "%count%", countStr, &j, &i, sizelimit);
        *hasCount |= written;
        written &= _matchAndReplaceMacro(&out, path, "%host%", hardwareId, &j, &i, sizelimit);
        written &= _matchAndReplaceMacro(&out, path, "%date%", date, &j, &i, sizelimit);
        if (!written) {
            out[j] = path[i];
            j++;
        }
    }

    out[j] = '\0';
    if (!*hasCount) {
        if ((strlen(out) + strlen(countStr) + 2) <= sizelimit) {
            out[j] = '.';
            for (k = 0; k < strlen(countStr); k++) {
                out[j + k + 1] = countStr[k];
            }
            out[j + k + 1] = '\0';
            *hasCount = true;
        } else {
            safuLogWarn("Failed to append 'count' to log file path .");
        }
    }
    out = safuAllocMem(out, sizeof(char) * (strlen(out) + 1));
    if (out == NULL) {
        safuLogErr("Failed to resize storage path memory.");
        return _completeStoragePath(STORAGE_LOCATION, strlen(STORAGE_LOCATION) + strlen(countStr) + 1, countStr, date,
                                    hasCount, recCounter + 1);
    }

    return out;
}

char *elosCompleteStoragePath(const char *const path, size_t sizelimit, const char *const countStr,
                              const char *const date, bool *hasCount) {
    return _completeStoragePath(path, sizelimit, countStr, date, hasCount, 0);
}

#define max_number_lenght 25
static inline safuResultE_t _jsonBackendOpen(elosStorageBackend_t *backend) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosJsonBackend_t *jsonBackend = backend->backendData;
    char countStr[max_number_lenght];
    int retval = snprintf(countStr, max_number_lenght, "%lu", jsonBackend->count);
    if (retval == 0) {
        safuLogErr("Failed to use correct count for filename, defaulting to 0");
        countStr[0] = '0';
        countStr[1] = '\0';
    }

    jsonBackend->storageDate = safuGetCurrentDateString(jsonBackend->dateFormat);
    if (jsonBackend->storageDate == NULL) {
        jsonBackend->storageDate = safuAllocMem(NULL, sizeof(char));
        jsonBackend->storageDate[0] = '\0';
    }

    jsonBackend->filePath =
        elosCompleteStoragePath(jsonBackend->storageFilePattern, jsonBackend->pathSizeLimit, countStr,
                                jsonBackend->storageDate, &jsonBackend->pathContainsCount);

    errno = 0;
    int fd = open(jsonBackend->filePath, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC | jsonBackend->additionalFlags,
                  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd < 0) {
        safuLogErrF("Failed to open log file %s for write: %s", jsonBackend->filePath, strerror(errno));

    } else {
        jsonBackend->storageFileWrite = fd;
        safuLogDebugF("stored backend file under fd %d (writeable)", fd);

        fd = open(jsonBackend->filePath, O_RDONLY | O_CLOEXEC);
        if (fd < 0) {
            safuLogErrF("Failed to open log file %s for read: %s", jsonBackend->filePath, strerror(errno));
        } else {
            jsonBackend->storageFileRead = fd;
            safuLogDebugF("stored backend file under fd %d (readable)", fd);
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

safuResultE_t elosJsonBackendStart(elosStorageBackend_t *backend) {
    elosJsonBackend_t *jsonBackend = backend->backendData;
    safuLogDebugF("start backend %s", backend->name);
    safuResultE_t result = _jsonBackendOpen(backend);
    safuLogDebugF("filepath %s", jsonBackend->filePath);
    return result;
}

static inline safuResultE_t _jsonBackendClose(elosStorageBackend_t *backend) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (backend != NULL) {
        elosJsonBackend_t *jsonBackend = backend->backendData;

        if (jsonBackend != NULL) {
            close(jsonBackend->storageFileWrite);
            close(jsonBackend->storageFileRead);
            free(jsonBackend->filePath);
            free(jsonBackend->storageDate);
        }
        result = SAFU_RESULT_OK;
    }

    return result;
}

safuResultE_t elosJsonBackendShutdown(elosStorageBackend_t *backend) {
    safuLogDebugF("shutdown backend %s", backend->name);
    safuResultE_t result = _jsonBackendClose(backend);
    free(backend->backendData);
    free(backend);
    return result;
}

bool elosJsonBackendCheckRotate(elosStorageBackend_t *backend, size_t nextWrite) {
    bool shouldRotate = false;
    if (backend != NULL) {
        elosJsonBackend_t *jsonBackend = backend->backendData;
        if (strstr(jsonBackend->filePath, jsonBackend->storageDate) != NULL) {
            char *currentDate = safuGetCurrentDateString(jsonBackend->dateFormat);
            if (currentDate != NULL) {
                if (strcmp(currentDate, jsonBackend->storageDate) != 0) {
                    shouldRotate = true;
                }
                free(currentDate);
            }
        }

        if ((jsonBackend->pathContainsCount) && (jsonBackend->maxFileSize != 0)) {
            struct stat buf;
            int retVal = stat(jsonBackend->filePath, &buf);
            if (retVal != 0) {
                safuLogErrF("Failed to get size of file %s", jsonBackend->filePath);
            } else if (buf.st_size + nextWrite >= jsonBackend->maxFileSize) {
                // rotate without date change, so increase count to make filename unique
                jsonBackend->count++;
                shouldRotate = true;
            }
        }
    }
    return shouldRotate;
}

safuResultE_t elosJsonBackendRotate(elosStorageBackend_t *backend) {
    safuResultE_t result;
    elosJsonBackend_t *jsonBackend = backend->backendData;
    safuLogDebugF("rotating logfile %s", jsonBackend->storageFilePattern);
    safuLogDebugF("Closing logfile %s", jsonBackend->filePath);
    result = _jsonBackendClose(backend);
    if (result == SAFU_RESULT_OK) {
        result = _jsonBackendOpen(backend);
        safuLogDebugF("Opened logfile %s", jsonBackend->filePath);
        if (result == SAFU_RESULT_FAILED) {
            safuLogWarnF("Failed to open new logfile %s", jsonBackend->filePath);
        }
    } else {
        safuLogWarnF("Failed to close logfile %s, refraining from rotating", jsonBackend->filePath);
    }
    return result;
}

/**
 * Writes the event to the underlying storage pointed to by
 * ``elosStorageBackend_t.backendData``. The function shall not call
 * ``sync`` to flush kernel caches, as the file shall be opened with
 * O_SYNC. The function shall block until the event is successful written
 * to the storage backend.
 *
 * Parameters:
 *     backend (elosStorageBackend_t*): The Storage Backend instance itself.
 *     event : a pointer to a event instance to persist
 * Returns:
 *     safuResultE_t:
 *         SAFU_RESULT_OK – on success.
 *         SAFU_RESULT_FAILURE – on failure.
 */
safuResultE_t elosJsonBackendPersist(elosStorageBackend_t *backend, const elosEvent_t *event) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosJsonBackend_t *jsonBackend = backend->backendData;
    ssize_t res = 0;

    char *jsonString = NULL;
    result = elosEventSerialize(&jsonString, event);
    if (result == SAFU_RESULT_FAILED) {
        safuLogErr("elosEventSerialize failed");
    } else if (elosJsonBackendCheckRotate(backend, sizeof(char) * strlen(jsonString))) {
        result = elosJsonBackendRotate(backend);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErrF("Failed to rotate log, trying to store log anyway into %s", jsonBackend->filePath);
        }
    }

    if (result == SAFU_RESULT_OK) {
        safuLogDebugF("persist to %s : %s", backend->name, jsonString);
        res = safuWriteExactly(jsonBackend->storageFileWrite, jsonString, strlen(jsonString));
        if (res < 0) {
            safuLogErrErrno("safuWriteExactly[0] failed");
            result = SAFU_RESULT_FAILED;
        }
    }

    if (result == SAFU_RESULT_OK) {
        res = safuWriteExactly(jsonBackend->storageFileWrite, "\n", 1);
        if (res < 0) {
            safuLogErrErrno("safuWriteExactly[1] failed");
            result = SAFU_RESULT_FAILED;
        }
    }

    free(jsonString);

    if (result != SAFU_RESULT_OK) {
        safuLogErr("Failed to persist event");
    }

    return result;
}

static safuResultE_t elosJsonBackendFilterEvent(elosEventFilter_t *filter, struct timespec const *newest,
                                                struct timespec const *oldest, safuVec_t *events, char *jsonString) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosRpnFilterResultE_t filterResult;
    elosEvent_t *event = NULL;

    result = elosEventNew(&event);
    if (result == SAFU_RESULT_FAILED) {
        safuLogErr("elosEventNew failed!");
    } else {
        result = elosEventDeserialize(event, jsonString);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErr("elosEventDeserialize failed");
        }
    }

    if (result == SAFU_RESULT_OK) {
        safuLogDebugF("Event created: %s", jsonString);

        filterResult = elosEventFilterExecuteInTimeRange(filter, NULL, newest, oldest, event);
        if (filterResult == RPNFILTER_RESULT_MATCH) {
            safuVecPush(events, event);  // copies event and free's its members
            free(event);
            safuLogDebug("Append event: Match");
            result = SAFU_RESULT_OK;
        } else if (filterResult == RPNFILTER_RESULT_NO_MATCH) {
            elosEventDelete(event);
            safuLogDebug("Free Event: No Match");
            result = SAFU_RESULT_OK;
        } else {
            safuLogErr("elosEventFilterExecute failed");
            safuLogDebugF("Free Event: RPN filter result is: %d", filterResult);
            elosEventDelete(event);
            result = SAFU_RESULT_FAILED;
        }
    }

    return result;
}

/**
 * Applies a filter to a given storage backend. Events which weren’t sorted
 * out by the filter, will be appended on the event vector.
 * Shutdown the logging system and free all related resources.
 *
 * Parameters:
 *     backend (elosStorageBackend_t*): The Storage Backend instance itself.
 *     filter : a filter that will be applied to the backend
 *     events : an event vector, where elements will be appended
 * Returns:
 *     safuResultE_t:
 *         SAFU_RESULT_OK – on success.
 *         SAFU_RESULT_FAILURE – on failure.
 */
safuResultE_t elosJsonBackendFindEvents(elosStorageBackend_t *backend, elosEventFilter_t *filter,
                                        struct timespec const *newest, struct timespec const *oldest,
                                        safuVec_t *events) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosJsonBackend_t *jsonBackend = NULL;
    FILE *jsonBackendFile = NULL;
    char *buffer = NULL;
    size_t bufferSize = 0;
    long endOfFilePos = 0;
    ssize_t readSuccess = 0;

    if (backend == NULL || filter == NULL || events == NULL) {
        safuLogErr("Called elosJsonBackendFindEvents with NULL-parameter");
    } else {
        errno = 0;
        jsonBackend = backend->backendData;
        jsonBackendFile = fdopen(jsonBackend->storageFileRead, "r");
        if (jsonBackendFile == NULL) {
            safuLogErrF("Failed to get file stream from log file: %s", strerror(errno));
        } else {
            fseek(jsonBackendFile, 0, SEEK_END);
            endOfFilePos = ftell(jsonBackendFile);
            rewind(jsonBackendFile);
            safuLogDebugF("opened backend file via fd %d (readable)", jsonBackend->storageFileRead);
            safuLogDebugF("file length is %ld", endOfFilePos);

            for (int idx = 1;; idx++) {
                readSuccess = getline(&buffer, &bufferSize, jsonBackendFile);  // calls malloc / realloc internally
                if (readSuccess > 0) {
                    result = elosJsonBackendFilterEvent(filter, newest, oldest, events, buffer);
                    if (result == SAFU_RESULT_FAILED) {
                        safuLogErr("elosJsonBackendFilterEvent failed");
                        free(buffer);
                        break;
                    } else {
                        memset(buffer, '\0', bufferSize);
                    }
                } else if (ftell(jsonBackendFile) == endOfFilePos) {
                    free(buffer);
                    result = SAFU_RESULT_OK;
                    break;
                } else {
                    safuLogErrF("getline failed reading the %d line in log file: %s", idx, strerror(errno));
                    safuLogDebugF("actual file position is: %ld, end of file position is: %ld", ftell(jsonBackendFile),
                                  endOfFilePos);
                    free(buffer);
                    result = SAFU_RESULT_FAILED;
                    break;
                }
            }
        }
    }

    return result;
}

safuResultE_t elosJsonBackendNew(elosStorageBackend_t **backend) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosStorageBackend_t *newBackend = NULL;

    newBackend = safuAllocMem(NULL, sizeof(elosStorageBackend_t));
    if (newBackend == NULL) {
        safuLogErr("safuAllocMem failed");
        result = SAFU_RESULT_FAILED;
    }

    if (result == SAFU_RESULT_OK) {
        memset(newBackend, 0, sizeof(elosStorageBackend_t));
        newBackend->backendData = safuAllocMem(NULL, sizeof(elosJsonBackend_t));
        if (newBackend->backendData == NULL) {
            safuLogCrit("safuAllocMem failed");
            free(newBackend);
            result = SAFU_RESULT_FAILED;
        } else {
            memset(newBackend->backendData, 0, sizeof(elosJsonBackend_t));
            newBackend->name = BACKEND_NAME;
            newBackend->start = elosJsonBackendStart;
            newBackend->persist = elosJsonBackendPersist;
            newBackend->findEvent = elosJsonBackendFindEvents;
            newBackend->shutdown = elosJsonBackendShutdown;
            *backend = newBackend;
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

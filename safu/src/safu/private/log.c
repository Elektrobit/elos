// SPDX-License-Identifier: MIT
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "safu/log.h"

#include <pthread.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

// The compiler doesn't let us use stdout/stderr in static initializers for some bizarre reason,
// using these flags is the 'cleanest' solution, everything else depends on the compiler or involves
// checking for initialization with every single function call, which we should avoid here
#define USE_STDOUT_STREAM   0x80000000
#define USE_STDERR_STREAM   0x40000000
#define USE_STD_STREAM_MASK 0xC0000000

#define FLAGS_CRIT  (SAFU_LOG_FLAG_WRITE_TO_STREAM | USE_STDERR_STREAM | SAFU_LOG_FLAG_WITH_POSITION)
#define FLAGS_ERR   (SAFU_LOG_FLAG_WRITE_TO_STREAM | USE_STDERR_STREAM | SAFU_LOG_FLAG_WITH_POSITION)
#define FLAGS_WARN  (SAFU_LOG_FLAG_WRITE_TO_STREAM | USE_STDERR_STREAM | SAFU_LOG_FLAG_WITH_POSITION)
#define FLAGS_INFO  (SAFU_LOG_FLAG_WRITE_TO_STREAM | USE_STDOUT_STREAM)
#define FLAGS_DEBUG (SAFU_LOG_FLAG_WRITE_TO_STREAM | USE_STDOUT_STREAM | SAFU_LOG_FLAG_WITH_POSITION)

#define FILTER_MAX_SIZE 256
#define PREFIX_MAX_SIZE 64

typedef struct safuLogLevelEntry {
    const char *name;
    const char *prefix;
    FILE *stream;
    safuLogFlags_t flags;
} safuLogLevelEntry_t;

typedef struct safuContext {
    safuLogLevelEntry_t logLevelEntry[SAFU_LOG_LEVEL_COUNT];
    char prefix[PREFIX_MAX_SIZE];
    char filter[FILTER_MAX_SIZE];
    safuLogFlags_t flags;
    pthread_mutex_t mutex;
    safuLogLevelE_t visibleLevel;
    safuLogFuncCb_t logCallbackFunc;
} safuContext_t;

static safuContext_t safuContext = {
    .logLevelEntry =
        {
            [SAFU_LOG_LEVEL_NONE] = {.name = "NONE", .prefix = NULL, .stream = NULL, .flags = 0},
            [SAFU_LOG_LEVEL_CRIT] = {.name = "CRITICAL", .prefix = "CRITICAL: ", .stream = NULL, .flags = FLAGS_CRIT},
            [SAFU_LOG_LEVEL_ERR] = {.name = "ERROR", .prefix = "ERROR: ", .stream = NULL, .flags = FLAGS_ERR},
            [SAFU_LOG_LEVEL_WARN] = {.name = "WARNING", .prefix = "WARNING: ", .stream = NULL, .flags = FLAGS_WARN},
            [SAFU_LOG_LEVEL_INFO] = {.name = "INFO", .prefix = NULL, .stream = NULL, .flags = FLAGS_INFO},
            [SAFU_LOG_LEVEL_DEBUG] = {.name = "DEBUG", .prefix = "DEBUG: ", .stream = NULL, .flags = FLAGS_DEBUG},
        },
    .prefix = {0},
    .filter = {0},
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .flags = 0,
    .visibleLevel = SAFU_LOG_LEVEL_INFO,
    .logCallbackFunc = NULL,
};

static bool _filterMatch(const char *fileName) {
    bool match = false;

    if ((fileName != NULL) && (safuContext.filter[0] != '\0')) {
        size_t fileNameLen = strlen(fileName);
        const char *start = &safuContext.filter[0];
        const char *stop = NULL;

        while (start != NULL) {
            size_t filterLen;
            stop = strchr(start, ';');
            filterLen = (size_t)(stop - start);
            if (fileNameLen < filterLen) {
                filterLen = fileNameLen;
            }
            if (strncmp(start, fileName, filterLen) == 0) {
                match = true;
                break;
            }
            if (stop == NULL) {
                break;
            }
            start = stop + 1;
        }
    }

    return match;
}

static safuLogStatusE_t _setPrefix(const char *prefix) {
    safuLogStatusE_t result = SAFU_LOG_STATUS_ERROR;
    char *retVal;

    retVal = strncpy(safuContext.prefix, prefix, PREFIX_MAX_SIZE - 1);
    if (retVal != NULL) {
        safuContext.prefix[PREFIX_MAX_SIZE - 1] = '\0';
        result = SAFU_LOG_STATUS_SUCCESS;
    }

    return result;
}

static safuLogStatusE_t _setStream(safuLogLevelE_t level, FILE *stream) {
    if (level == SAFU_LOG_LEVEL_ALL) {
        int i;
        for (i = 0; i < SAFU_LOG_LEVEL_COUNT; i++) {
            safuLogLevelEntry_t *entry = &safuContext.logLevelEntry[i];
            entry->stream = stream;
            entry->flags &= ~USE_STD_STREAM_MASK;
        }
    } else {
        safuLogLevelEntry_t *entry = &safuContext.logLevelEntry[level];
        entry->stream = stream;
        entry->flags &= ~USE_STD_STREAM_MASK;
    }
    return SAFU_LOG_STATUS_SUCCESS;
}

static safuLogStatusE_t _setStreamLevel(safuLogLevelE_t level) {
    safuContext.visibleLevel = level;
    return SAFU_LOG_STATUS_SUCCESS;
}

static safuLogStatusE_t _setCallbackFunc(safuLogFuncCb_t logCallbackFunc) {
    safuContext.logCallbackFunc = logCallbackFunc;
    return SAFU_LOG_STATUS_SUCCESS;
}

static safuLogStatusE_t _logFuncStream(safuLogMessage_t logMessage) {
    safuLogLevelEntry_t *entry = &safuContext.logLevelEntry[logMessage.level];
    safuLogStatusE_t status = SAFU_LOG_STATUS_SUCCESS;
    const char *fileName = strrchr(logMessage.file, '/') + 1;
    char *messageString = NULL;
    int retVal;

    retVal = vasprintf(&messageString, logMessage.fmt, logMessage.ap);
    if (retVal < 0) {
        status = SAFU_LOG_STATUS_ERROR;
    }

    if ((status == SAFU_LOG_STATUS_SUCCESS) && (safuContext.visibleLevel >= logMessage.level)) {
        if ((safuContext.filter[0] == '\0') || (_filterMatch(fileName) == true)) {
            const char *globalPrefix = safuContext.prefix;
            const char *levelPrefix = (entry->prefix == NULL) ? "" : entry->prefix;
            if (entry->flags & SAFU_LOG_FLAG_WITH_POSITION) {
                retVal = fprintf(entry->stream, "%s%s%s - file '%s', line %d, %s()\n", globalPrefix, levelPrefix,
                                 messageString, fileName, logMessage.line, logMessage.func);
                if (retVal < 0) {
                    status = SAFU_LOG_STATUS_ERROR;
                }
            } else {
                retVal = fprintf(entry->stream, "%s%s%s\n", globalPrefix, levelPrefix, messageString);
                if (retVal < 0) {
                    status = SAFU_LOG_STATUS_ERROR;
                }
            }
        }
    }

    if (messageString != NULL) {
        free(messageString);
    }

    return status;
}

safuLogLevelE_t safuLogLevelFromString(const char *logLevelStr) {
    safuLogLevelE_t logLevelResult = SAFU_LOG_LEVEL_INVALID;
    if (logLevelStr != NULL) {
        safuLogLevelE_t logLevel;
        for (logLevel = SAFU_LOG_LEVEL_NONE; logLevel < SAFU_LOG_LEVEL_COUNT; logLevel++) {
            const char *name = safuContext.logLevelEntry[logLevel].name;
            if (strcasecmp(logLevelStr, name) == 0) {
                logLevelResult = logLevel;
            }
        }
        if (strcasecmp(logLevelStr, "VERBOSE") == 0) {
            logLevelResult = SAFU_LOG_LEVEL_ALL;
        }
    }
    return logLevelResult;
}

const char *safuLogLevelToString(safuLogLevelE_t logLevel) {
    if (logLevel >= SAFU_LOG_LEVEL_NONE && logLevel < SAFU_LOG_LEVEL_COUNT) {
        return safuContext.logLevelEntry[logLevel].name;
    }
    if (logLevel == SAFU_LOG_LEVEL_ALL) {
        return "VERBOSE";
    }
    return "INVALID";
}

safuLogLevelE_t safuLogGetStreamLevel() {
    return safuContext.visibleLevel;
}

safuLogStatusE_t safuLogSetPrefix(const char *prefix) {
    safuLogStatusE_t status = SAFU_LOG_STATUS_SUCCESS;
    int retVal;

    retVal = pthread_mutex_lock(&safuContext.mutex);
    if (retVal < 0) {
        status = SAFU_LOG_STATUS_ERROR;
    } else {
        status = _setPrefix(prefix);
        retVal = pthread_mutex_unlock(&safuContext.mutex);
        if (retVal < 0) {
            status = SAFU_LOG_STATUS_ERROR;
        }
    }

    return status;
}

safuLogStatusE_t safuLogSetStream(safuLogLevelE_t level, FILE *stream) {
    safuLogStatusE_t status = SAFU_LOG_STATUS_SUCCESS;

    if ((level >= SAFU_LOG_LEVEL_COUNT) && (level != SAFU_LOG_LEVEL_ALL)) {
        status = SAFU_LOG_STATUS_ERROR;
    } else {
        int retVal;
        retVal = pthread_mutex_lock(&safuContext.mutex);
        if (retVal < 0) {
            status = SAFU_LOG_STATUS_ERROR;
        } else {
            status = _setStream(level, stream);
            retVal = pthread_mutex_unlock(&safuContext.mutex);
            if (retVal < 0) {
                status = SAFU_LOG_STATUS_ERROR;
            }
        }
    }

    return status;
}

safuLogStatusE_t safuLogSetCallbackFunc(safuLogFuncCb_t logCallbackFunc) {
    safuLogStatusE_t status = SAFU_LOG_STATUS_SUCCESS;
    int retVal;

    retVal = pthread_mutex_lock(&safuContext.mutex);
    if (retVal < 0) {
        status = SAFU_LOG_STATUS_ERROR;
    } else {
        status = _setCallbackFunc(logCallbackFunc);
        retVal = pthread_mutex_unlock(&safuContext.mutex);
        if (retVal < 0) {
            status = SAFU_LOG_STATUS_ERROR;
        }
    }

    return status;
}

safuLogStatusE_t safuLogSetStreamLevel(safuLogLevelE_t level) {
    safuLogStatusE_t status = SAFU_LOG_STATUS_SUCCESS;

    if ((level >= SAFU_LOG_LEVEL_COUNT) && (level != SAFU_LOG_LEVEL_ALL)) {
        status = SAFU_LOG_STATUS_ERROR;
    } else {
        int retVal;
        retVal = pthread_mutex_lock(&safuContext.mutex);
        if (retVal < 0) {
            status = SAFU_LOG_STATUS_ERROR;
        } else {
            status = _setStreamLevel(level);
            retVal = pthread_mutex_unlock(&safuContext.mutex);
            if (retVal < 0) {
                status = SAFU_LOG_STATUS_ERROR;
            }
        }
    }

    return status;
}

safuLogStatusE_t _setFilterStr(const char *filterStr) {
    safuLogStatusE_t status = SAFU_LOG_STATUS_SUCCESS;
    size_t filterLen = strlen(filterStr);
    if (filterStr != NULL && filterLen < FILTER_MAX_SIZE) {
        char *retVal = strncpy(&safuContext.filter[0], filterStr, FILTER_MAX_SIZE - 1);
        safuContext.filter[FILTER_MAX_SIZE - 1] = '\0';
        if (retVal == NULL || strlen(safuContext.filter) != filterLen) {
            status = SAFU_LOG_STATUS_ERROR;
        }
    } else {
        status = SAFU_LOG_STATUS_ERROR;
    }
    return status;
}

safuLogStatusE_t safuLogSetFilterStr(const char *filterStr) {
    safuLogStatusE_t status = SAFU_LOG_STATUS_SUCCESS;
    int retVal;
    retVal = pthread_mutex_lock(&safuContext.mutex);
    if (retVal < 0) {
        status = SAFU_LOG_STATUS_ERROR;
    } else {
        status = _setFilterStr(filterStr);
        retVal = pthread_mutex_unlock(&safuContext.mutex);
        if (retVal < 0) {
            status = SAFU_LOG_STATUS_ERROR;
        }
    }
    return status;
}

safuLogStatusE_t safuLogFuncV(safuLogLevelE_t level, const char *file, const char *func, unsigned int line,
                              const char *fmt, va_list args) {
    safuLogStatusE_t status = SAFU_LOG_STATUS_SUCCESS;
    safuLogLevelEntry_t *entry = NULL;
    safuLogMessage_t logMessage = {.level = level, .file = file, .func = func, .line = line, .fmt = fmt};
    bool locked = false;

    va_copy(logMessage.ap, args);

    if ((level <= SAFU_LOG_LEVEL_NONE) || (level >= SAFU_LOG_LEVEL_COUNT) || (fmt == NULL)) {
        status = SAFU_LOG_STATUS_ERROR;

    } else if (safuContext.flags & SAFU_LOG_FLAG_WITH_POSITION) {
        if ((file == NULL) || (func == NULL)) {
            status = SAFU_LOG_STATUS_ERROR;
        }
    }

    if (status == SAFU_LOG_STATUS_SUCCESS) {
        int retVal;
        retVal = pthread_mutex_lock(&safuContext.mutex);
        if (retVal < 0) {
            status = SAFU_LOG_STATUS_ERROR;
        } else {
            locked = true;
            entry = &safuContext.logLevelEntry[level];
        }
    }

    if (status == SAFU_LOG_STATUS_SUCCESS) {
        if (entry->flags & USE_STDOUT_STREAM) {
            _setStream(level, stdout);
        } else if (entry->flags & USE_STDERR_STREAM) {
            _setStream(level, stderr);
        }
    }

    if (status == SAFU_LOG_STATUS_SUCCESS) {
        if (entry->flags & SAFU_LOG_FLAG_WRITE_TO_STREAM) {
            if (entry->stream == NULL) {
                status = SAFU_LOG_STATUS_ERROR;
            } else {
                status = _logFuncStream(logMessage);
            }
        }
        if (safuContext.logCallbackFunc != NULL) {
            status = safuContext.logCallbackFunc(logMessage);
        }
    }

    if (locked == true) {
        int retVal;
        retVal = pthread_mutex_unlock(&safuContext.mutex);
        if (retVal < 0) {
            status = SAFU_LOG_STATUS_ERROR;
        }
    }

    return status;
}

safuLogStatusE_t safuLogFuncF(safuLogLevelE_t level, const char *file, const char *func, unsigned int line,
                              const char *fmt, ...) {
    safuLogStatusE_t result;
    va_list args;

    va_start(args, fmt);
    result = safuLogFuncV(level, file, func, line, fmt, args);
    va_end(args);

    return result;
}

safuLogStatusE_t safuLogFunc(safuLogLevelE_t level, const char *file, const char *func, unsigned int line,
                             const char *fmt) {
    safuLogStatusE_t result;
    va_list args;

    result = safuLogFuncV(level, file, func, line, fmt, args);

    return result;
}

// SPDX-License-Identifier: MIT
#ifndef SAFU_LOG_H
#define SAFU_LOG_H

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

typedef int safuLogFlags_t;

#define SAFU_LOG_FLAG_WITH_POSITION   1
#define SAFU_LOG_FLAG_WRITE_TO_STREAM 2

typedef enum safuLogStatusE {
    SAFU_LOG_STATUS_SUCCESS = 0,
    SAFU_LOG_STATUS_ERROR,
} safuLogStatusE_t;

typedef enum safuLogLevelE {
    SAFU_LOG_LEVEL_NONE = 0,
    SAFU_LOG_LEVEL_CRIT,
    SAFU_LOG_LEVEL_ERR,
    SAFU_LOG_LEVEL_WARN,
    SAFU_LOG_LEVEL_INFO,
    SAFU_LOG_LEVEL_DEBUG,
    SAFU_LOG_LEVEL_COUNT,
    SAFU_LOG_LEVEL_ALL,
    SAFU_LOG_LEVEL_INVALID
} safuLogLevelE_t;

typedef struct safuLogMessage {
    safuLogLevelE_t level;
    const char *file;
    const char *func;
    int line;
    const char *fmt;
    va_list ap;
} safuLogMessage_t;

typedef safuLogStatusE_t (*safuLogFuncCb_t)(safuLogMessage_t logMessage);

safuLogLevelE_t safuLogLevelFromString(const char *logLevelStr);
const char *safuLogLevelToString(safuLogLevelE_t logLevel);

safuLogLevelE_t safuLogGetStreamLevel();

safuLogStatusE_t safuLogSetPrefix(const char *prefix);
safuLogStatusE_t safuLogSetStream(safuLogLevelE_t level, FILE *stream);
safuLogStatusE_t safuLogSetStreamLevel(safuLogLevelE_t level);
safuLogStatusE_t safuLogSetFilterStr(const char *filterStr);
safuLogStatusE_t safuLogSetCallbackFunc(safuLogFuncCb_t logCallbackFunc);
safuLogStatusE_t safuLogFuncV(safuLogLevelE_t level, const char *file, const char *func, unsigned int line,
                              const char *fmt, va_list arg);
safuLogStatusE_t safuLogFuncF(safuLogLevelE_t level, const char *file, const char *func, unsigned int line,
                              const char *fmt, ...) __attribute__((__format__(__printf__, 5, 6)));
safuLogStatusE_t safuLogFunc(safuLogLevelE_t level, const char *file, const char *func, unsigned int line,
                             const char *fmt) __attribute__((__format__(__printf__, 5, 0)));

#define safuLogCrit(__message) safuLogFunc(SAFU_LOG_LEVEL_CRIT, __FILE__, __func__, __LINE__, __message)
#define safuLogCritF(__message, ...) \
    safuLogFuncF(SAFU_LOG_LEVEL_CRIT, __FILE__, __func__, __LINE__, __message, __VA_ARGS__)
#define safuLogErr(__message) safuLogFunc(SAFU_LOG_LEVEL_ERR, __FILE__, __func__, __LINE__, __message)
#define safuLogErrF(__message, ...) \
    safuLogFuncF(SAFU_LOG_LEVEL_ERR, __FILE__, __func__, __LINE__, __message, __VA_ARGS__)
#define safuLogErrValue(__message, __value) \
    safuLogFuncF(SAFU_LOG_LEVEL_ERR, __FILE__, __func__, __LINE__, __message " (returned: %d)", __value)
#define safuLogErrErrno(__message) \
    safuLogFuncF(SAFU_LOG_LEVEL_ERR, __FILE__, __func__, __LINE__, __message " - %s", strerror(errno))
#define safuLogErrErrnoValue(__message, __value)                                                              \
    safuLogFuncF(SAFU_LOG_LEVEL_ERR, __FILE__, __func__, __LINE__, __message " (returned: %d - %s)", __value, \
                 strerror(errno))
#define safuLogWarn(__message) safuLogFunc(SAFU_LOG_LEVEL_WARN, __FILE__, __func__, __LINE__, __message)
#define safuLogWarnF(__message, ...) \
    safuLogFuncF(SAFU_LOG_LEVEL_WARN, __FILE__, __func__, __LINE__, __message, __VA_ARGS__)
#define safuLogInfo(__message) safuLogFunc(SAFU_LOG_LEVEL_INFO, __FILE__, __func__, __LINE__, __message)
#define safuLogInfoF(__message, ...) \
    safuLogFuncF(SAFU_LOG_LEVEL_INFO, __FILE__, __func__, __LINE__, __message, __VA_ARGS__)
#define safuLogDebug(__message) safuLogFunc(SAFU_LOG_LEVEL_DEBUG, __FILE__, __func__, __LINE__, __message)
#define safuLogDebugF(__message, ...) \
    safuLogFuncF(SAFU_LOG_LEVEL_DEBUG, __FILE__, __func__, __LINE__, __message, __VA_ARGS__)

#endif

// SPDX-License-Identifier: MIT

#include "mock_log.h"

#include <stdio.h>

// Note: safuLogFuncV is not for direct usage, it exists to make safuLogFuncF mocking possible, so we do not mock it

MOCK_FUNC_BODY(safuLogFuncF, safuLogStatusE_t, safuLogLevelE_t level, const char *file, const char *func,
               unsigned int line, const char *fmt, ...) {
    safuLogStatusE_t result;
    va_list args;

    va_start(args, fmt);
    if (MOCK_IS_ACTIVE(safuLogFuncF)) {
        static char message[MOCK_SAFU_LOG_MESSAGE_SIZE] = {0};
        int retval;

        check_expected(level);
        check_expected_ptr(file);
        check_expected_ptr(func);
        check_expected(line);

        memset(message, '\0', MOCK_SAFU_LOG_MESSAGE_SIZE);
        retval = vsnprintf(message, MOCK_SAFU_LOG_MESSAGE_SIZE, fmt, args);
        if (retval < 0) {
            abort();
        }
        check_expected(message);

        result = mock_type(safuLogStatusE_t);
    } else {
        result = safuLogFuncV(level, file, func, line, fmt, args);
    }

    va_end(args);
    return result;
}

MOCK_FUNC_BODY(safuLogFunc, safuLogStatusE_t, safuLogLevelE_t level, const char *file, const char *func,
               unsigned int line, const char *fmt) {
    safuLogStatusE_t result;
    va_list args;

    if (MOCK_IS_ACTIVE(safuLogFunc)) {
        static char message[MOCK_SAFU_LOG_MESSAGE_SIZE];
        int retval;

        check_expected(level);
        check_expected_ptr(file);
        check_expected_ptr(func);
        check_expected(line);

        memset(message, '\0', MOCK_SAFU_LOG_MESSAGE_SIZE);
        retval = vsnprintf(message, MOCK_SAFU_LOG_MESSAGE_SIZE, fmt, args);
        if (retval < 0) {
            abort();
        }
        check_expected(message);

        result = mock_type(safuLogStatusE_t);
    } else {
        result = safuLogFuncV(level, file, func, line, fmt, args);
    }

    return result;
}

MOCK_FUNC_BODY(safuLogSetPrefix, safuLogStatusE_t, const char *prefix) {
    safuLogStatusE_t result;

    if (MOCK_IS_ACTIVE(safuLogSetPrefix)) {
        check_expected_ptr(prefix);
        result = mock_type(safuLogStatusE_t);
    } else {
        result = MOCK_FUNC_REAL(safuLogSetPrefix)(prefix);
    }

    return result;
}

MOCK_FUNC_BODY(safuLogSetStream, safuLogStatusE_t, safuLogLevelE_t level, FILE *stream) {
    safuLogStatusE_t result;

    if (MOCK_IS_ACTIVE(safuLogSetStream)) {
        check_expected(level);
        check_expected_ptr(stream);
        result = mock_type(safuLogStatusE_t);
    } else {
        result = MOCK_FUNC_REAL(safuLogSetStream)(level, stream);
    }

    return result;
}

MOCK_FUNC_BODY(safuLogSetStreamLevel, safuLogStatusE_t, safuLogLevelE_t level) {
    safuLogStatusE_t result;

    if (MOCK_IS_ACTIVE(safuLogSetStreamLevel)) {
        check_expected(level);
        result = mock_type(safuLogStatusE_t);
    } else {
        result = MOCK_FUNC_REAL(safuLogSetStreamLevel)(level);
    }

    return result;
}

MOCK_FUNC_BODY(safuLogSetCallbackFunc, safuLogStatusE_t, safuLogFuncCb_t logCallbackFunc) {
    safuLogStatusE_t result;

    if (MOCK_IS_ACTIVE(safuLogSetCallbackFunc)) {
        check_expected_ptr(logCallbackFunc);
        result = mock_type(safuLogStatusE_t);
    } else {
        result = MOCK_FUNC_REAL(safuLogSetCallbackFunc)(logCallbackFunc);
    }

    return result;
}

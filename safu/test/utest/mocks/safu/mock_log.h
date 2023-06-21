// SPDX-License-Identifier: MIT

#ifndef __MOCK_COMMON_H__
#define __MOCK_COMMON_H__

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>

#ifdef MOCK_SAFU_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "safu/log.h"

#ifndef MOCK_SAFU_LOG_MESSAGE_SIZE
#define MOCK_SAFU_LOG_MESSAGE_SIZE 512
#endif

MOCK_FUNC_PROTOTYPE(safuLogFuncF, safuLogStatusE_t, safuLogLevelE_t level, const char *file, const char *func,
                    unsigned int line, const char *fmt, ...)
MOCK_FUNC_PROTOTYPE(safuLogFunc, safuLogStatusE_t, safuLogLevelE_t level, const char *file, const char *func,
                    unsigned int line, const char *fmt)
MOCK_FUNC_PROTOTYPE(safuLogSetPrefix, safuLogStatusE_t, const char *prefix)
MOCK_FUNC_PROTOTYPE(safuLogSetStream, safuLogStatusE_t, safuLogLevelE_t level, FILE *stream)
MOCK_FUNC_PROTOTYPE(safuLogSetStreamLevel, safuLogStatusE_t, safuLogLevelE_t level)
MOCK_FUNC_PROTOTYPE(safuLogSetCallbackFunc, safuLogStatusE_t, safuLogFuncCb_t logCallbackFunc)

#endif /* __MOCK_COMMON_H__ */

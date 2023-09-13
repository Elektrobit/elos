// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#ifdef EVENTBUFFER_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "elos/logger/logger.h"

MOCK_FUNC_PROTOTYPE(elosLoggerInitialize, safuResultE_t, elosLogger_t *logger)
MOCK_FUNC_PROTOTYPE(elosLoggerDeleteMembers, safuResultE_t, elosLogger_t *logger)
MOCK_FUNC_PROTOTYPE(elosLoggerGetDefaultLogger, safuResultE_t, elosLogger_t **logger)
MOCK_FUNC_PROTOTYPE(elosLog, void, elosEventMessageCodeE_t messageCode, elosSeverityE_t severity,
                    uint64_t classification, const char *logMessage)

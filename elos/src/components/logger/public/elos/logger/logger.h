// SPDX-License-Identifier: MIT
#pragma once

#include <safu/common.h>
#include <safu/flags.h>

#include "elos/eloslog/eloslog.h"
#include "elos/eventbuffer/eventbuffer.h"

typedef struct elosLogger {
    elosEventBuffer_t *logEventBuffer;
    safuFlags_t flags;
} elosLogger_t;

safuResultE_t elosLoggerInitialize(elosLogger_t *logger);
safuResultE_t elosLoggerDeleteMembers(elosLogger_t *logger);
safuResultE_t elosLoggerGetDefaultLogger(elosLogger_t **logger);
void elosLog(elosEventMessageCodeE_t messageCode, elosSeverityE_t severity, uint64_t classification,
             const char *logMessage);

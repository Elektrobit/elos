// SPDX-License-Identifier: MIT
#pragma once

#include "elos/event/event_types.h"

typedef struct elosLogData {
    elosEventMessageCodeE_t messageCode;
    elosSeverityE_t severity;
    uint64_t classification;
    char *payload;
} elosLogData_t;

typedef enum elosLogStatusE {
    ELOS_LOG_STATUS_SUCCESS = 0,
    ELOS_LOG_STATUS_ERROR = 1,
} elosLogStatusE_t;

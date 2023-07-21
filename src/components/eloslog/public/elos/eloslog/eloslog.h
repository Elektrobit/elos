// SPDX-License-Identifier: MIT
#pragma once

#include "elos/eloslog/types.h"

void elosLogCreateLogData(elosEventMessageCodeE_t messageCode, elosSeverityE_t severity, uint64_t classification,
                          const char *logMessage, elosLogData_t **logdata);
void elosLogDeleteLogData(elosLogData_t *logData);
void elosLogSafuFallback(elosLogData_t *logData);
elosLogStatusE_t elosLogCreateElosEventFromLog(elosLogData_t *logData, elosEvent_t **event);

// SPDX-License-Identifier: MIT
#pragma once

#include "elos/eventprocessor/types.h"

// loggerNew()
// loggerDelet()

void elosLogSetEventProcessor(elosEventProcessor_t *lep);
void elosLog(elosEventMessageCodeE_t messageCode, elosSeverityE_t severity, uint64_t classification,
             const char *logMessage);

// SPDX-License-Identifier: MIT
#pragma once

#include "elos/event/event_types.h"

void elosLog(elosEventMessageCodeE_t messageCode, elosSeverityE_t severity, uint64_t classification,
             const char *logMessage);

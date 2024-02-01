// SPDX-License-Identifier: MIT
#pragma once

#include "elos/event/event_types.h"

__BEGIN_DECLS

void elosLog(elosEventMessageCodeE_t messageCode, elosSeverityE_t severity, uint64_t classification,
             const char *logMessage);

__END_DECLS
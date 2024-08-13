// SPDX-License-Identifier: MIT
#pragma once

#include <sys/cdefs.h>
#include "elos/event/event_types.h"
#include "elos/rpnfilter/rpnfilter_types.h"

__BEGIN_DECLS

#define ELOS_EVENTFILTER_INIT ELOS_RPNFILTER_INIT

typedef enum elosEventFilterValueTypeE {
    EVENTFILTER_VALUE_EVENT = RPNFILTER_VALUE_TYPE_COUNT,  // Extend elosRpnFilterValueTypeE_t
    EVENTFILTER_VALUE_EVENT_DATE,
    EVENTFILTER_VALUE_EVENT_DATE_SEC,
    EVENTFILTER_VALUE_EVENT_DATE_NSEC,
    EVENTFILTER_VALUE_EVENT_SOURCE,
    EVENTFILTER_VALUE_EVENT_SOURCE_PID,
    EVENTFILTER_VALUE_EVENT_SOURCE_APPNAME,
    EVENTFILTER_VALUE_EVENT_SOURCE_FILENAME,
    EVENTFILTER_VALUE_EVENT_SEVERITY,
    EVENTFILTER_VALUE_EVENT_HARDWAREID,
    EVENTFILTER_VALUE_EVENT_CLASSIFICATION,
    EVENTFILTER_VALUE_EVENT_MESSAGECODE,
    EVENTFILTER_VALUE_EVENT_PAYLOAD,
    EVENTFILTER_VALUE_TYPE_COUNT
} elosEventFilterValueTypeE_t;

typedef elosRpnFilterParam_t elosEventFilterParam_t;
typedef elosRpnFilterStack_t elosEventFilterStack_t;
typedef elosRpnFilter_t elosEventFilter_t;

__END_DECLS

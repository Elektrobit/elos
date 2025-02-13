// SPDX-License-Identifier: MIT
#pragma once

#include <safu/result.h>

#include "elos/eventdispatcher/defines.h"
#include "elos/eventdispatcher/types.h"

#ifdef DISPATCHER_DEBUGS_EVENTBUFFER

#define ADDITIONAL_DISPATCHER_DEBUGS(...) \
    do {                                  \
        safuLogDebugF(__VA_ARGS__);       \
    } while (0)

#else
#define ADDITIONAL_DISPATCHER_DEBUGS(...) \
    do {                                  \
    } while (0)

#endif

void *elosEventDispatcherWorker(void *data);

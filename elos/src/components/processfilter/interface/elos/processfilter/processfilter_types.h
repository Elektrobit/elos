// SPDX-License-Identifier: MIT
#pragma once

#include "elos/rpnfilter/rpnfilter_types.h"

#define ELOS_PROCESSFILTER_INIT ELOS_RPNFILTER_INIT

typedef enum elosProcessFilterValueTypeE {
    PROCESSFILTER_VALUE_PROCESS = RPNFILTER_VALUE_TYPE_COUNT,  // Extend elosRpnFilterValueTypeE_t
    PROCESSFILTER_VALUE_PROCESS_UID,
    PROCESSFILTER_VALUE_PROCESS_GID,
    PROCESSFILTER_VALUE_PROCESS_PID,
    PROCESSFILTER_VALUE_PROCESS_EXEC,
    PROCESSFILTER_VALUE_TYPE_COUNT
} elosProcessFilterValueTypeE_t;

typedef elosRpnFilterParam_t elosProcessFilterParam_t;
typedef elosRpnFilterStack_t elosProcessFilterStack_t;
typedef elosRpnFilter_t elosProcessFilter_t;

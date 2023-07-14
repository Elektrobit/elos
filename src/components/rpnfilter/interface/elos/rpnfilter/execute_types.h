// SPDX-License-Identifier: MIT
#pragma once

#include "elos/rpnfilter/rpnfilter_types.h"

typedef struct elosRpnFilterExecute {
    elosRpnFilter_t const *filter;
    elosRpnFilterStack_t const *param;
    elosRpnFilterStack_t *stack;
    void *input;
    uint32_t valueStackIdx;
    uint32_t stackIdx;
} elosRpnFilterExecute_t;

typedef elosRpnFilterResultE_t (*elosRpnFilterExecuteFunc_t)(elosRpnFilterExecute_t *ctx, uint32_t idx);

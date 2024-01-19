// SPDX-License-Identifier: MIT
#pragma once

#include "elos/rpnfilter/execute_types.h"

__BEGIN_DECLS

elosRpnFilterResultE_t elosRpnFilterExecuteStackNew(elosRpnFilterStack_t **stack, size_t stackSize);
elosRpnFilterResultE_t elosRpnFilterExecuteStackDelete(elosRpnFilterStack_t *ctx);
elosRpnFilterResultE_t elosRpnFilterExecuteStep(elosRpnFilterExecute_t *ctx, uint32_t idx);
elosRpnFilterResultE_t elosRpnFilterExecuteWith(elosRpnFilter_t const *filter, elosRpnFilterStack_t const *param,
                                                void *input, elosRpnFilterExecuteFunc_t func[], size_t funcCount);

__END_DECLS

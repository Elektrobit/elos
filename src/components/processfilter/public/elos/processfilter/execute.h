// SPDX-License-Identifier: MIT
#pragma once

#include "elos/common/process.h"
#include "elos/rpnfilter/execute.h"

__BEGIN_DECLS

elosRpnFilterResultE_t elosProcessFilterExecuteStep(elosRpnFilterExecute_t *ctx, uint32_t idx);
elosRpnFilterResultE_t elosProcessFilterExecuteWith(elosRpnFilter_t const *filter, elosRpnFilterStack_t const *param,
                                                    elosProcessIdentity_t *input, elosRpnFilterExecuteFunc_t func[],
                                                    size_t funcCount);

__END_DECLS

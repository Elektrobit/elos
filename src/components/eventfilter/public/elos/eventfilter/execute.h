// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTFILTER_EXECUTE_H
#define ELOS_EVENTFILTER_EXECUTE_H

#include "elos/event/event.h"
#include "elos/eventfilter/step_types.h"
#include "elos/rpnfilter/execute.h"

__BEGIN_DECLS

elosRpnFilterResultE_t elosEventFilterExecuteStep(elosRpnFilterExecute_t *ctx, uint32_t idx);
elosRpnFilterResultE_t elosEventFilterExecuteWith(elosRpnFilter_t const *filter, elosRpnFilterStack_t const *param,
                                                  elosEvent_t *input, elosRpnFilterExecuteFunc_t func[],
                                                  size_t funcCount);

__END_DECLS

#endif /* ELOS_EVENTFILTER_EXECUTE_H */

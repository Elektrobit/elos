// SPDX-License-Identifier: MIT
#pragma once

#include "elos/rpnfilter/rpnfilter_types.h"
#include "elos/rpnfilter/vector_types.h"

__BEGIN_DECLS

elosRpnFilterResultE_t elosRpnFilterVectorInit(elosRpnFilterVector_t *rpnFilterVector, uint32_t elements);
elosRpnFilterResultE_t elosRpnFilterVectorPush(elosRpnFilterVector_t *rpnFilterVector, elosRpnFilter_t *rpnFilter);
elosRpnFilterResultE_t elosRpnFilterVectorIterate(elosRpnFilterVector_t *rpnFilterVector, safuVecFunc_t *func,
                                                  void *data);
elosRpnFilterResultE_t elosRpnFilterVectorDeleteMembers(elosRpnFilterVector_t *rpnFilterVector);

__END_DECLS

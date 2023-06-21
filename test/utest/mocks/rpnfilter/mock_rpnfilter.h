// SPDX-License-Identifier: MIT
#ifndef ELOS_RPNFILTER_RPNFILTER_MOCK_H
#define ELOS_RPNFILTER_RPNFILTER_MOCK_H

#include <cmocka_extensions/mock_extensions.h>
#ifdef MOCK_RPNFILTER_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "elos/rpnfilter/rpnfilter.h"
#include "elos/rpnfilter/vector.h"

MOCK_FUNC_PROTOTYPE(elosRpnFilterDeleteMembers, elosRpnFilterResultE_t, elosRpnFilter_t *filter)
MOCK_FUNC_PROTOTYPE(elosRpnFilterVectorPush, elosRpnFilterResultE_t, elosRpnFilterVector_t *rpnFilterVector,
                    elosRpnFilter_t *rpnFilter)

#endif /* ELOS_RPNFILTER_RPNFILTER_MOCK_H */

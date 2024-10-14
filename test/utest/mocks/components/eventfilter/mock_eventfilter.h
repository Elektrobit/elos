// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTFILTER_EVENTFILTER_MOCK_H
#define ELOS_EVENTFILTER_EVENTFILTER_MOCK_H

#include <cmocka_extensions/mock_extensions.h>
#ifdef MOCK_EVENTFILTER_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "elos/eventfilter/eventfilter.h"

MOCK_FUNC_PROTOTYPE(elosEventFilterInit, elosRpnFilterResultE_t, elosEventFilter_t *filter)

MOCK_FUNC_PROTOTYPE(elosEventFilterNew, elosRpnFilterResultE_t, elosEventFilter_t **filter)

MOCK_FUNC_PROTOTYPE(elosEventFilterDeepCopy, elosRpnFilterResultE_t, elosEventFilter_t *to, const elosRpnFilter_t *from)

MOCK_FUNC_PROTOTYPE(elosEventFilterClone, elosRpnFilterResultE_t, elosEventFilter_t **filter,
                    const elosRpnFilter_t *from)

MOCK_FUNC_PROTOTYPE(elosEventFilterDeleteMembers, elosRpnFilterResultE_t, elosEventFilter_t *filter)

MOCK_FUNC_PROTOTYPE(elosEventFilterDelete, elosRpnFilterResultE_t, elosEventFilter_t *filter)

MOCK_FUNC_PROTOTYPE(elosEventFilterCreate, elosRpnFilterResultE_t, elosEventFilter_t *filter,
                    const elosEventFilterParam_t *param)

MOCK_FUNC_PROTOTYPE(elosEventFilterExecute, elosRpnFilterResultE_t, const elosEventFilter_t *filter,
                    const elosEventFilterStack_t *param, elosEvent_t *input)

MOCK_FUNC_PROTOTYPE(elosEventFilterExecuteInTimeRange, elosRpnFilterResultE_t, const elosEventFilter_t *filter,
                    const elosEventFilterStack_t *param, struct timespec const *newest, struct timespec const *oldest,
                    elosEvent_t *input)

#endif /* ELOS_EVENTFILTER_EVENTFILTER_MOCK_H */

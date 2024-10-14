// SPDX-License-Identifier: MIT

#ifndef __MOCK_ELOS_ELOSD_LOG_AGGREGATOR_H__
#define __MOCK_ELOS_ELOSD_LOG_AGGREGATOR_H__

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>

#ifdef MOCK_ELOS_ELOSD_EVENTLOGGING_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "elos/eventlogging/LogAggregator.h"

MOCK_FUNC_PROTOTYPE(elosLogAggregatorStart, safuResultE_t, elosLogAggregator_t *logAggregator,
                    elosLogAggregatorParam_t const *param)
MOCK_FUNC_PROTOTYPE(elosLogAggregatorAdd, safuResultE_t, elosLogAggregator_t *logAggregator, const elosEvent_t *)
MOCK_FUNC_PROTOTYPE(elosLogAggregatorShutdown, safuResultE_t, elosLogAggregator_t *logAggregator)
MOCK_FUNC_PROTOTYPE(elosLogAggregatorFindEvents, safuResultE_t, elosLogAggregator_t *logAggregator, const char *rule,
                    struct timespec const *newest, struct timespec const *oldest, safuVec_t *events)

#endif /* __MOCK_ELOS_ELOSD_LOG_AGGREGATOR_H__ */

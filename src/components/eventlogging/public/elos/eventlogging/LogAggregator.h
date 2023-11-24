// SPDX-License-Identifier: MIT
#ifndef __ELOS_LOGAGGREGATOR_H__
#define __ELOS_LOGAGGREGATOR_H__

#include <safu/common.h>
#include <safu/vector.h>
#include <samconf/samconf.h>

#include "elos/event/event.h"
#include "elos/eventlogging/LogAggregatorTypes.h"

safuResultE_t elosLogAggregatorStart(elosLogAggregator_t *logAggregator, elosLogAggregatorParam_t const *param);
safuResultE_t elosLogAggregatorAdd(elosLogAggregator_t *logAggregator, const elosEvent_t *);
safuResultE_t elosLogAggregatorShutdown(elosLogAggregator_t *logAggregator);
safuResultE_t elosLogAggregatorFindEvents(elosLogAggregator_t *logAggregator, const char *rule, safuVec_t *events);

#endif /* __ELOS_LOGAGGREGATOR_H__ */

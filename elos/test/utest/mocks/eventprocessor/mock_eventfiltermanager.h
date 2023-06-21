// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTPROCESSOR_EVENTFILTERMANAGER_MOCK_H
#define ELOS_EVENTPROCESSOR_EVENTFILTERMANAGER_MOCK_H

#include <cmocka_extensions/mock_extensions.h>
#ifdef MOCK_EVENTPROCESSOR_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "elos/eventprocessor/eventfiltermanager.h"

MOCK_FUNC_PROTOTYPE(elosEventFilterManagerInitialize, safuResultE_t, elosEventFilterManager_t *eft,
                    elosEventFilterManagerParam_t const *param)

MOCK_FUNC_PROTOTYPE(elosEventFilterManagerDeleteMembers, safuResultE_t, elosEventFilterManager_t *eft)

MOCK_FUNC_PROTOTYPE(elosEventFilterManagerNodeCreate, safuResultE_t, elosEventFilterManager_t *eft,
                    char const **filterStrings, size_t filterStringCount, elosEventQueueId_t eqId,
                    elosEventFilterNodeId_t *efnId)

MOCK_FUNC_PROTOTYPE(elosEventFilterManagerNodeRemove, safuResultE_t, elosEventFilterManager_t *efm,
                    elosEventFilterNodeId_t efnId)

MOCK_FUNC_PROTOTYPE(elosEventFilterManagerNodeRemoveByEventQueueId, safuResultE_t, elosEventFilterManager_t *efm,
                    elosEventQueueId_t eqId)

MOCK_FUNC_PROTOTYPE(elosEventFilterManagerProcess, safuResultE_t, elosEventFilterManager_t *efm,
                    elosEvent_t const *event)

#endif /* ELOS_EVENTPROCESSOR_EVENTFILTERMANAGER_MOCK_H */

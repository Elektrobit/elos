// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTPROCESSOR_EVENTFILTERMANAGERINITIALIZE_H__
#define ELOS_EVENTPROCESSOR_EVENTFILTERMANAGERINITIALIZE_H__

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/eventprocessor/eventfiltermanager.h"
#include "elos/eventprocessor/eventqueuemanager.h"

typedef struct elosUnitTestState {
    elosEventFilterManager_t efm;
    elosEventQueueManager_t eqm;
    elosIdManager_t idm;
} elosUnitTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterManagerInitializeErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterManagerInitializeSuccess)

void elosTestCaseNew(void **state);
void elosTestCaseDelete(void **state);

#endif /* ELOS_EVENTPROCESSOR_EVENTFILTERMANAGERINITIALIZE_H__ */

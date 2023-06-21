// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTFILTERENTRYINITIALIZE_UTEST_H
#define ELOS_EVENTFILTERENTRYINITIALIZE_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/eventprocessor/eventfiltermanager.h"
#include "elos/eventprocessor/eventqueuemanager.h"
#include "eventprocessor/eventfilternodevector.h"

typedef struct elosTestState {
    elosEventFilterManager_t eventFilterManager;
    elosEventQueueManager_t eventQueueManager;
    elosEventFilterNodeId_t eventFilterNodeIdA;
    elosEventFilterNodeId_t eventFilterNodeIdB;
    elosEventFilterNodeId_t eventFilterNodeIdC;
    elosEventQueueId_t eventQueueId;
} elosTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterManagerNodeRemoveErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterManagerNodeRemoveErrRemove)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterManagerNodeRemoveSuccess)

void elosTestCaseNew(void **state);
void elosTestCaseDelete(void **state);

#endif /* ELOS_EVENTFILTERENTRYINITIALIZE_UTEST_H */

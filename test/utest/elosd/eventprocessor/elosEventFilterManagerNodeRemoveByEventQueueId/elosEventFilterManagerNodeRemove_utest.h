// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTFILTERENTRYINITIALIZE_UTEST_H
#define ELOS_EVENTFILTERENTRYINITIALIZE_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>
#include <safu/mock_vector.h>

#include "elos/eventprocessor/eventfiltermanager.h"
#include "elos/eventprocessor/eventqueuemanager.h"

typedef struct elosUnitTestState {
    elosEventQueueManager_t eventQueueManager;
    elosEventFilterManager_t eventFilterManager;
    elosEventQueueId_t eventQueueIdA;
    elosEventQueueId_t eventQueueIdB;
    elosEventFilterNodeId_t eventFilterNodeIdA;
    elosEventFilterNodeId_t eventFilterNodeIdB;
    elosEventFilterNodeId_t eventFilterNodeIdC;
    elosEventFilterNodeId_t eventFilterNodeIdD;
} elosUnitTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterManagerNodeRemoveByEventQueueIdErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterManagerNodeRemoveByEventQueueIdErrRemove)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterManagerNodeRemoveByEventQueueIdSuccess)

#endif /* ELOS_EVENTFILTERENTRYINITIALIZE_UTEST_H */

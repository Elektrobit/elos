// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTFILTERENTRYINITIALIZE_UTEST_H
#define ELOS_EVENTFILTERENTRYINITIALIZE_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/eventprocessor/eventfiltermanager.h"
#include "elos/eventprocessor/eventqueuemanager.h"

typedef struct elosTestState {
    elosEventFilterManager_t efm;
    elosEventQueueManager_t eqm;
    elosEventQueueId_t eqId;
} elosTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterManagerNodeCreateErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterManagerNodeCreateSuccess)

#endif /* ELOS_EVENTFILTERENTRYINITIALIZE_UTEST_H */

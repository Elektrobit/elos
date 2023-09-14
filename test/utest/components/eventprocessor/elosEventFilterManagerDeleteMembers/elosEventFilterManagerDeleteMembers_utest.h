// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTFILTERENTRYINITIALIZE_UTEST_H
#define ELOS_EVENTFILTERENTRYINITIALIZE_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/eventprocessor/eventfiltermanager.h"

typedef struct elosTestState {
    elosEventFilterManager_t eventFilterManager;
    elosEventQueueManager_t eventQueueManager;
    elosIdManager_t idManager;
} elosTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterManagerDeleteMembersSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterManagerDeleteMembersErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterManagerDeleteMembersRemovalFailed)

#endif /* ELOS_EVENTFILTERENTRYINITIALIZE_UTEST_H */

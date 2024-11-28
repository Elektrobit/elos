// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTFILTERENTRYDELETMEMBERS_UTEST_H
#define ELOS_EVENTFILTERENTRYDELETMEMBERS_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/eventprocessor/eventfilternode.h"

typedef struct elosTestState {
    elosEventFilterNode_t efe;
    elosEventQueue_t eq;
} elosTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterNodeDeleteMembersErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterNodeDeleteMembersSuccess)

#endif /* ELOS_EVENTFILTERENTRYDELETMEMBERS_UTEST_H */

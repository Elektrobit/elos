// SPDX-License-Identifier: MIT
#ifndef ELOS_RPNFILTER_BUILDER_COMPILE_UTEST_H
#define ELOS_RPNFILTER_BUILDER_COMPILE_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/eventprocessor/eventqueuemanager.h"
#include "mock_eventqueuevector.h"
#include "safu/mock_safu.h"

typedef struct elosTestState {
    elosEventQueueManager_t eqm;
} elosTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventQueueManagerEntryNewErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventQueueManagerEntryNewErrPushByParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventQueueManagerEntryNewSuccess)

#endif /* ELOS_RPNFILTER_BUILDER_COMPILE_UTEST_H */

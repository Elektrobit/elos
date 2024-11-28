// SPDX-License-Identifier: MIT
#ifndef ELOS_RPNFILTER_BUILDER_COMPILE_UTEST_H
#define ELOS_RPNFILTER_BUILDER_COMPILE_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>
#include <safu/mock_safu.h>
#include <safu/vector.h>

#include "elos/eventprocessor/eventqueuemanager.h"

typedef struct elosTestState {
    elosEventQueueManager_t eqm;
    elosEventQueueId_t id;
} elosTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventQueueManagerEntryDeleteErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventQueueManagerEntryDeleteErrPushByParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventQueueManagerEntryDeleteErrWrongId)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventQueueManagerEntryDeleteSuccess)

#endif /* ELOS_RPNFILTER_BUILDER_COMPILE_UTEST_H */

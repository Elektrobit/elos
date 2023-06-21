// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTPROCESSORQUEUEREMOVE_UTEST_H
#define ELOS_EVENTPROCESSORQUEUEREMOVE_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/eventprocessor/eventprocessor.h"

typedef struct elosUnitTestState {
    elosEventProcessor_t eventProcessor;
    elosEventQueueId_t eventQueueId;
    samconfConfig_t config;
} elosUnitTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventProcessorQueueRemoveSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventProcessorQueueRemoveErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventProcessorQueueRemoveDeleteFailed)

#endif /* ELOS_EVENTPROCESSORQUEUEREMOVE_UTEST_H */

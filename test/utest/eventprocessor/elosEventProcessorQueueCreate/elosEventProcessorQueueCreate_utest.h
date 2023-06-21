// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTPROCESSOR_QUEUEREAD_UTEST_H
#define ELOS_EVENTPROCESSOR_QUEUEREAD_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/eventprocessor/eventprocessor.h"

typedef struct elosUnitTestState {
    elosEventProcessor_t eventProcessor;
    elosEventQueueId_t eventQueueId;
    samconfConfig_t config;
} elosUnitTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventProcessorQueueCreateSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventProcessorQueueCreateErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventProcessorQueueCreateNewQueueFailed)

#endif /* ELOS_EVENTPROCESSOR_QUEUEREAD_UTEST_H */

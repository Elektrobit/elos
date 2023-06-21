// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTPROCESSORQUEUEREMOVE_UTEST_H
#define ELOS_EVENTPROCESSORQUEUEREMOVE_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/eventprocessor/eventprocessor.h"

typedef struct elosUnitTestState {
    elosEventProcessor_t eventProcessor;
    samconfConfig_t config;
    elosEvent_t *event;
} elosUnitTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventProcessorPublishSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventProcessorPublishErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventProcessorPublishProcessFailed)

#endif /* ELOS_EVENTPROCESSORQUEUEREMOVE_UTEST_H */

// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTPROCESSORINITIALIZE_UTEST_H
#define ELOS_EVENTPROCESSORINITIALIZE_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/eventprocessor/eventprocessor.h"

typedef struct elosTestState {
    elosEventProcessor_t eventProcessor;
    samconfConfig_t config;
} elosTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventProcessorInitializeErrComponent)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventProcessorInitializeErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventProcessorInitializeSuccess)

#endif /* ELOS_EVENTPROCESSORINITIALIZE_UTEST_H */

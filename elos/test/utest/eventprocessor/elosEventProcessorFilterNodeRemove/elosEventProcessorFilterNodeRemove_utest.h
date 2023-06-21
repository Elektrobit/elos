// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTPROCESSORFILTERNODEREMOVE_UTEST_H
#define ELOS_EVENTPROCESSORFILTERNODEREMOVE_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/eventprocessor/eventprocessor.h"

typedef struct elosTestState {
    elosEventProcessor_t eventProcessor;
    samconfConfig_t config;
} elosTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventProcessorFilterNodeRemoveSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventProcessorFilterNodeRemoveErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventProcessorFilterNodeRemoveRemoveFailed)

#endif /* ELOS_EVENTPROCESSORFILTERNODEREMOVE_UTEST_H */

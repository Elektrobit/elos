// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTPROCESSORDELETEMEMBERS_UTEST_H
#define ELOS_EVENTPROCESSORDELETEMEMBERS_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/eventprocessor/eventprocessor.h"

typedef struct elosTestState {
    elosEventProcessor_t eventProcessor;
    samconfConfig_t config;
} elosTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventProcessorDeleteMembersErrComponent)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventProcessorDeleteMembersNullParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventProcessorDeleteMembersSuccess)

#endif /* ELOS_EVENTPROCESSORDELETEMEMBERS_UTEST_H */

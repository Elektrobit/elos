// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>

#include "mock_libelos_communication.h"

typedef struct elosUnitTestState {
    elosMessage_t *message;
} elosUnitTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestElosCreateMessageErrAllocation)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosCreateMessageErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosCreateMessageSuccess)

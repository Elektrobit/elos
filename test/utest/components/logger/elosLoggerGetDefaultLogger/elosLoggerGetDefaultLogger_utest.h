// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <safu/common.h>

#include "elos/logger/logger.h"

typedef struct elosUnitTestState {
    elosLogger_t logger;
} elosUnitTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestElosLoggerGetDefaultLoggerSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosLoggerGetDefaultLoggerErrFunc)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosLoggerGetDefaultLoggerGetAfterLoggerInit)

// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/libelos/libelos.h"

typedef struct elosUnitTestState {
    elosSession_t session;
} elosUnitTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestElosSessionValidSuccess)

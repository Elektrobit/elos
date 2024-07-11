// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <safu/vector_types.h>
#include <samconf/samconf.h>

typedef struct elosTestState {
    safuVec_t testFilter;
    samconfConfig_t mockConfig;
} elosTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestElosAuthorizedProcessCheckSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosAuthorizedProcessCheckAuthorizedprocessParamNull)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosAuthorizedProcessCheckProcessParamNull)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosAuthorizedProcessCheckSuccessNoMatch)

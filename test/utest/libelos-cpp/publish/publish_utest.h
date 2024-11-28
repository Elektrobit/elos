// SPDX-License-Identifier: MIT
#pragma once
#include <elos/libelos-cpp/libelos-cpp.h>
extern "C" {
#include <cmocka_extensions/cmocka_extensions.h>
}
#include "mock_libelos.h"

const elosEvent_t elosCTestEvent = {
    {25, 100},
    {strdup("appNameTest"), strdup("fileNameTest"), 0},
    ELOS_SEVERITY_DEBUG,
    strdup("localhost"),
    42,
    ELOS_MSG_CODE_NEW_SUBSCRIPTION,
    strdup("payload"),
};

TEST_CASE_FUNC_PROTOTYPES(elosTestPublishSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestPublishSuccessInsert)
TEST_CASE_FUNC_PROTOTYPES(elosTestPublishErrSession)

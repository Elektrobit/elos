// SPDX-License-Identifier: MIT
#pragma once
#include <elos/libelos-cpp/libelos-cpp.h>

extern "C" {
#include <cmocka_extensions/cmocka_extensions.h>
}

#define MOCK_NAME     "TEST 123"
#define MOCK_ID       "ID 123"
#define MOCK_TIME     42
#define MOCK_PID      0x0815
#define MOCK_SEVERITY ELOS_SEVERITY_FATAL
#define MOCK_MSG_CODE ELOS_MSG_CODE_LOGIN_FAIL
#define MOCK_CLASSIF  123
#define MOCK_PAYLOAD  "hello 123"

TEST_CASE_FUNC_PROTOTYPES(elosTestEventSuccess)

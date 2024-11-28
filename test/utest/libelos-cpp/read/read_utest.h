// SPDX-License-Identifier: MIT
#pragma once
#include <elos/libelos-cpp/libelos-cpp.h>
extern "C" {
#include <cmocka_extensions/cmocka_extensions.h>
#include <safu/mock_vector.h>

#include "../../mocks/libelos/mock_libelos.h"
#include "cmocka_mocks/mock_libc.h"
}

#define MOCK_PORT    1234
#define MOCK_IP_ADDR "123.4.5.6"

#define MOCK_NAME     "TEST 123"
#define MOCK_ID       "ID 123"
#define MOCK_PAYLOAD  "hello 123"
#define MOCK_SEVERITY ELOS_SEVERITY_FATAL

TEST_CASE_FUNC_PROTOTYPES(elosTestReadSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestReadError)

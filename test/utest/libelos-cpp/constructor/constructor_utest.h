// SPDX-License-Identifier: MIT
#pragma once
#include <elos/libelos-cpp/libelos-cpp.h>

extern "C" {
#include <cmocka_extensions/cmocka_extensions.h>

#include "../../mocks/libelos/mock_libelos.h"
}

#define MOCK_PORT    1234
#define MOCK_IP_ADDR "123.4.5.6"
#define MOCK_FD      789

TEST_CASE_FUNC_PROTOTYPES(elosTestConstructorSuccess)

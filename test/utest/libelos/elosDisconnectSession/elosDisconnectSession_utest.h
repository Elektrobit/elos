// SPDX-License-Identifier: MIT
#pragma once

#include <arpa/inet.h>
#include <cmocka_extensions/cmocka_extensions.h>
#include <netinet/in.h>

#include "elos/libelos/libelos.h"

#define MOCK_PORT    1234
#define MOCK_IP_ADDR "123.4.5.6"
#define MOCK_FD      789

TEST_CASE_FUNC_PROTOTYPES(elosTestElosDisconnectSessionSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosDisconnectSessionExtErrClose)

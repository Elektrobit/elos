// SPDX-License-Identifier: MIT
#pragma once

#include <arpa/inet.h>
#include <cmocka_extensions/cmocka_extensions.h>
#include <netinet/in.h>

#include "../../mocks/libelos/mock_libelos.h"
#include "elos/libelos/libelos.h"

#define MOCK_PORT    1234
#define MOCK_IP_ADDR "123.4.5.6"
#define MOCK_FD      789

elosSession_t *elosConnectTcpipCreateSession();

TEST_CASE_FUNC_PROTOTYPES(elosTestElosConnectTcpipErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosConnectTcpipExtErrAllocMem)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosConnectTcpipSuccess)

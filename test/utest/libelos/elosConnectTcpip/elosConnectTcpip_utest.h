// SPDX-License-Identifier: MIT
#ifndef __ELOS_ELOSCONNECTTCPIP_H__
#define __ELOS_ELOSCONNECTTCPIP_H__

#include <arpa/inet.h>
#include <cmocka_extensions/cmocka_extensions.h>
#include <netinet/in.h>

#include "elos/libelos/libelos.h"

#define MOCK_PORT    1234
#define MOCK_IP_ADDR "123.4.5.6"
#define MOCK_FD      789

elosSession_t *elosConnectTcpipCreateSession();

TEST_CASE_FUNC_PROTOTYPES(elosTestElosConnectTcpipErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosConnectTcpipExtErrAllocMem)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosConnectTcpipExtErrGetaddrinfo)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosConnectTcpipExtErrSocket)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosConnectTcpipExtErrConnect)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosConnectTcpipSuccess)

#endif /* __ELOS_ELOSCONNECTTCPIP_H__ */

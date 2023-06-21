// SPDX-License-Identifier: MIT
#ifndef __ELOS_ELOSDISCONNECT_H__
#define __ELOS_ELOSDISCONNECT_H__

#include <arpa/inet.h>
#include <cmocka_extensions/cmocka_extensions.h>
#include <netinet/in.h>

#include "elos/libelos/libelos.h"

#define MOCK_PORT    1234
#define MOCK_IP_ADDR "123.4.5.6"
#define MOCK_FD      789

typedef struct elosUnitTestState {
    elosSession_t *session;
} elosUnitTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestElosDisconnectSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosDisconnectExtErrClose)

#endif /* __ELOS_ELOSDISCONNECT_H__ */

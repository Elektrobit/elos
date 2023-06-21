// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <cmocka_mocks/mock_libc.h>
#include <samconf/mock_samconf.h>
#include <samconf/samconf.h>

#include "elos/client_manager/client_authorized_processes.h"

#ifndef LIBMNL_OK
#define LIBMNL_OK 0
#endif

#ifndef LIBMNL_ERROR
#define LIBMNL_ERROR 1
#endif

typedef struct {
    samconfConfig_t *childrenData;
} elosUteststateT_t;

int elosAuthorizedProcessInitUtestCleanUp(void **state);
int elosAuthorizedProcessInitUtestInit(void **state);

TEST_CASE_FUNC_PROTOTYPES(elosTestElosAuthorizedProcessInitializeSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosAuthorizedProcessInitializeAuthorizedprocessParamNull)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosAuthorizedProcessInitializeConfigParamNull)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosAuthorizedProcessInitializeExtErrConfigGet)

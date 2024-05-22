// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <cmocka_mocks/mock_libc.h>
#include <samconf/mock_samconf.h>
#include <samconf/samconf.h>

#include "connectionmanager/clientauthorizedprocesses.h"
#include "elos/processfilter/processfilter.h"

#define ELOS_CONFIG_ROOT "is deprecated for plugins and fix me"

#ifndef LIBMNL_OK
#define LIBMNL_OK 0
#endif

#ifndef LIBMNL_ERROR
#define LIBMNL_ERROR 1
#endif

typedef struct {
    samconfConfig_t *childrenData;
} elosUteststateT_t;

int elosAuthorizedProcessCheckUtestCleanUp(void **state);
int elosAuthorizedProcessCheckUtestInit(void **state);

TEST_CASE_FUNC_PROTOTYPES(elosTestElosAuthorizedProcessCheckSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosAuthorizedProcessCheckAuthorizedprocessParamNull)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosAuthorizedProcessCheckProcessParamNull)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosAuthorizedProcessCheckSuccessNoMatch)

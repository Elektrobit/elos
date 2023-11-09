// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <cmocka_mocks/mock_libc.h>
#include <samconf/mock_samconf.h>
#include <samconf/samconf.h>

#include "elos/connectionmanager/clientauthorizedprocesses.h"

#ifndef LIBMNL_OK
#define LIBMNL_OK 0
#endif

#ifndef LIBMNL_ERROR
#define LIBMNL_ERROR 1
#endif

typedef struct {
    samconfConfig_t *childrenData;
} elosUteststateT_t;

int elosAuthorizedProcessDeleteUtestCleanUp(void **state);
int elosAuthorizedProcessDeleteUtestInit(void **state);

TEST_CASE_FUNC_PROTOTYPES(elosTestElosAuthorizedProcessDeleteSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosAuthorizedProcessDeleteAuthorizedprocessParamNull)

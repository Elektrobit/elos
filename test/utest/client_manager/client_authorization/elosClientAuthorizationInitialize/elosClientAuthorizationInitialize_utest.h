// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <cmocka_mocks/mock_libc.h>

#include "elos/client_manager/client_authorization.h"

#ifndef LIBMNL_OK
#define LIBMNL_OK 0
#endif

#ifndef LIBMNL_ERROR
#define LIBMNL_ERROR 1
#endif

TEST_CASE_FUNC_PROTOTYPES(elosTestElosClientAuthorizationInitializeErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosClientAuthorizationInitializeExterrMnlSocketBind)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosClientAuthorizationInitializeExterrMnlSocketOpen)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosClientAuthorizationInitializeSuccess)

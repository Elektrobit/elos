// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <cmocka_mocks/mock_libc.h>

#include "elos/clientmanager/clientauthorization.h"

#ifndef LIBMNL_OK
#define LIBMNL_OK 0
#endif

#ifndef LIBMNL_ERROR
#define LIBMNL_ERROR 1
#endif

TEST_CASE_FUNC_PROTOTYPES(elosTestElosClientAuthorizationIsTrustedConnectionSuccess)

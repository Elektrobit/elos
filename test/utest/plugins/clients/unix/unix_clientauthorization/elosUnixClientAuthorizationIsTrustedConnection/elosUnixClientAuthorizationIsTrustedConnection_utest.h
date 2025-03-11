// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <cmocka_mocks/mock_libc.h>
#include <limits.h>
#include <mock_clientauthorizedprocesses.h>
#include <unistd.h>

#define MOCK_FD      42
#define MOCK_PID     43
#define MOCK_PATH    "/proc/43/exe"
#define MOCK_ADDRESS 0xdeadbeef

TEST_CASE_FUNC_PROTOTYPES(elosTestelosUnixClientAuthorizationIsTrustedConnectionSuccess)

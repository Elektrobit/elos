// SPDX-License-Identifier: MIT
#pragma once
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
extern "C" {
#include <cmocka.h>
}
#include <cmocka_extensions/cmocka_extensions.h>
#include <elos/libelos-cpp/libelos-cpp.h>

TEST_CASE_FUNC_PROTOTYPES(elosTestPublishSuccess)

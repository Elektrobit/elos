// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>

#include "elos/eloslog/eloslog.h"
#include "elos/eloslog/types.h"
#include "elos/event/event.h"
#include "mock_event.h"

TEST_CASE_FUNC_PROTOTYPES(elosLogCreateElosEventFromLogSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosLogCreateElosEventFromLogErrParamLogData)
TEST_CASE_FUNC_PROTOTYPES(elosLogCreateElosEventFromLogErrParamEvent)
TEST_CASE_FUNC_PROTOTYPES(elosLogCreateElosEventFromLogExtErrEvent)

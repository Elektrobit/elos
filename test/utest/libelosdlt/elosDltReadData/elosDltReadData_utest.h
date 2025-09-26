// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/libelosdlt/libelosdlt.h"

TEST_CASE_FUNC_PROTOTYPES(elosTestElosDltReadDataSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosDltReadDataErrorInvalidMsg)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosDltReadDataErrorIncompleteMsg)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosDltReadDataErrorIncompleteHdr)

// SPDX-License-Identifier: MIT
#ifndef __ELOS_BLACKLISTDELETE_UTEST_H__
#define __ELOS_BLACKLISTDELETE_UTEST_H__

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <cmocka_mocks/mock_libc.h>

#include "elos/client_manager/client_blacklist.h"

TEST_CASE_FUNC_PROTOTYPES(elosTestElosBlacklistDeleteSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosBlacklistDeleteErrBlacklistParameterNull)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosBlacklistDeleteErrBlacklistFilterDelete)

#endif /* __ELOS_BLACKLISTDELETE_UTEST_H__ */

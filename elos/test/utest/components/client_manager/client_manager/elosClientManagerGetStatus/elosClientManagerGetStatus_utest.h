// SPDX-License-Identifier: MIT
#ifndef __ELOS_CLIENTMANAGERGETSTATUS_UTEST_H__
#define __ELOS_CLIENTMANAGERGETSTATUS_UTEST_H__

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <cmocka_mocks/mock_libc.h>

#include "elos/client_manager/client_manager.h"

#define MOCK_FLAGS 42

TEST_CASE_FUNC_PROTOTYPES(elosTestElosClientManagerGetStatusExtErrMutexLock)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosClientManagerGetStatusExtErrMutexUnlock)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosClientManagerGetStatusSuccess)

#endif /* __ELOS_CLIENTMANAGERGETSTATUS_UTEST_H__ */

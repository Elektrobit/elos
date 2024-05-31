// SPDX-License-Identifier: MIT
#ifndef __ELOS_BLACKLISTINITIALIZE_UTEST_H__
#define __ELOS_BLACKLISTINITIALIZE_UTEST_H__

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <cmocka_mocks/mock_libc.h>
#include <elos/eventfilter/eventfilter_types.h>
#include <samconf/mock_samconf.h>

typedef struct elosTestState {
    elosEventFilter_t testFilter;
    samconfConfig_t mockConfig;
} elosTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestElosBlacklistInitializeSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosBlacklistInitializeExtErrFilterString)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosBlacklistInitializeExtErrConfigGetString)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosBlacklistInitializeErrBlacklistParameterNull)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosBlacklistInitializeErrConfigParameterNull)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosBlacklistInitializeErrFilterCreate)

#endif /* __ELOS_BLACKLISTINITIALIZE_UTEST_H__ */

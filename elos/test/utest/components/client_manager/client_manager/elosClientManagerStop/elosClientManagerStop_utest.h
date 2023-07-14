// SPDX-License-Identifier: MIT
#ifndef __ELOS_CLIENTMANAGERSTOP_UTEST_H__
#define __ELOS_CLIENTMANAGERSTOP_UTEST_H__

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>

#include "elos/client_manager/client_manager.h"

typedef struct {
    elosClientManagerContext_t *context;
    elosClientManagerParam_t *parameters;
} elosUteststateT_t;

void elosClientManagerStopUtestInitParameters(void **state);
void elosClientManagerStopUtestDeleteParameters(void **state);

TEST_CASE_FUNC_PROTOTYPES(elosTestElosClientManagerStopErrContextNull)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosClientManagerStopCloseConnections)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosClientManagerStopExtErrSemDestroy)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosClientManagerStopNoManagerStarted)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosClientManagerStopClientManagerActive)

#endif /* __ELOS_CLIENTMANAGERSTOP_UTEST_H__ */

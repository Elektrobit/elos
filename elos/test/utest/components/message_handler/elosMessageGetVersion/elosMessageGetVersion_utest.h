// SPDX-License-Identifier: MIT
#ifndef __ELOS_MESSAGEGETVERSION_H__
#define __ELOS_MESSAGEGETVERSION_H__

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/clientmanager/clientmanager.h"
#include "elos/common/message.h"
#include "mock_message_handler.h"

#define MOCK_DISPATCH_ID 42

elosClientConnection_t *elosMessageGetVersionCreateConnection();

TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageGetVersionExtErrNewObject)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageGetVersionExtErrAddNewString)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageGetVersionSuccess)

#endif /* __ELOS_MESSAGEGETVERSION_H__ */

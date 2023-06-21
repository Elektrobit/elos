// SPDX-License-Identifier: MIT
#ifndef ELOS_LIBELOS_ELOSSENDMESSAGE_UTEST_H
#define ELOS_LIBELOS_ELOSSENDMESSAGE_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>

#include "mock_libelos_communication.h"

typedef struct elosUnitTestState {
    elosMessage_t *message;
    elosMessage_t *messageNoJson;
    elosSession_t session;
} elosUnitTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestElosSendMessageErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosSendMessageErrSend)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosSendMessageSuccess)

#endif /* ELOS_LIBELOS_ELOSSENDMESSAGE_UTEST_H */

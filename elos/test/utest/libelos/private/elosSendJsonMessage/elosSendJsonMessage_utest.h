// SPDX-License-Identifier: MIT
#ifndef ELOS_LIBELOS_ELOSSENDJSONMESSAGE_UTEST_H
#define ELOS_LIBELOS_ELOSSENDJSONMESSAGE_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>

#include "mock_libelos_communication.h"

typedef struct elosTestSet {
    json_object *jsonObject;
    char const *jsonStr;
    elosMessage_t *message;
    size_t messageSize;
} elosTestSet_t;

typedef struct elosUnitTestState {
    elosSession_t session;
    elosTestSet_t normal;
    elosTestSet_t empty;
} elosUnitTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestElosSendJsonMessageErrCall)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosSendJsonMessageErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosSendJsonMessageSuccess)

#endif /* ELOS_LIBELOS_ELOSSENDJSONMESSAGE_UTEST_H */

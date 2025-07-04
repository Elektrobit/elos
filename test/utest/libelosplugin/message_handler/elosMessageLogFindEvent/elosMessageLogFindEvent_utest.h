// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <elos/common/message.h>
#include <elos/event/event_types.h>
#include <elos/event/event_vector.h>
#include <json-c/json_object.h>
#include <json-c/json_tokener.h>
#include <json-c/json_types.h>
#include <safu/common.h>
#include <safu/json.h>
#include <safu/result.h>
#include <safu/vector.h>
#include <string.h>

#include "elos/libelosplugin/clientconnection_types.h"

typedef struct elosUtestState {
    elosClientConnection_t *conn;
    elosMessage_t *msg;
    json_object *expectedJsonResponse;
} elosUtestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageLogFindEventUntruncatedSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageLogFindEventTruncatedSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageLogFindEventInvalidRequestFilterError)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageLogFindEventNoRequestTimeSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageLogFindEventError)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageLogFindEventNullConnectionError)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageLogFindEventNullMessageError)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageLogFindEventNullMessageLengthError)

int elosMessageLogFindEventVerifyResponse(const LargestIntegralType actual, const LargestIntegralType expected);
int elosMessageLogFindEventInjectEventList(const LargestIntegralType eventVector, const LargestIntegralType expected);
int elosMessageLogFindEventCheckJsonObject(const long unsigned int parameter, const long unsigned int compare);
int elosMessageLogFindEventCheckEvent(const elosEvent_t *expected, const elosEvent_t *actual);

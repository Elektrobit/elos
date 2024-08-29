// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <json-c/json.h>

#include "connectionmanager/clientconnection_types.h"
#include "elos/common/message.h"

typedef struct elosUtestState {
    elosClientConnection_t *conn;
    elosMessage_t *msg;
    json_object *expectedJsonResponse;
} elosUtestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageLogFindEventSuccess)

int elosMessageLogFindEventCheckJsonObject(const long unsigned int parameter, const long unsigned int compare);
int elosMessageLogFindEventCheckEvent(const elosEvent_t *expected, const elosEvent_t *actual);

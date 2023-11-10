// SPDX-License-Identifier: MIT
#ifndef __ELOS_MESSAGEEVENTSUBSCRIBE_H__
#define __ELOS_MESSAGEEVENTSUBSCRIBE_H__

#include <cmocka_extensions/cmocka_extensions.h>
#include <json-c/json.h>
#include <safu/common.h>

#include "elos/common/message.h"
#include "elos/connectionmanager/connectionmanager.h"

typedef struct elosArrayStringTestData {
    const char **strings;
    int count;
} elosArrayStringTestData_t;

int elosAssertStringArraysEqual(const long unsigned int valueInt, const long unsigned int checkValueDataInt);
int elosCheckJsonObject(const long unsigned int jsonParam, const long unsigned int jsonExpected);

typedef struct elosUtestState {
    elosMessage_t *msg;
    json_object *jobj;
    json_object *response;
    elosClientConnection_t *conn;
} elosUtestState_t;

extern safuResultE_t elosMessageEventSubscribe(elosClientConnection_t *conn, elosMessage_t const *const msg);

TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageEventSubscribeCompleteFailure)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageEventSubscribeFilterNodeNull)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageEventSubscribeFilterAllocateFailed)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageEventSubscribeNoFilter)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageEventSubscribeNoJson)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageEventSubscribeNoLoop)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageEventSubscribeNoString)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageEventSubscribeQueueIdNull)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageEventSubscribeSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageEventSubscribeStrdupFailed)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageEventSubscribeResponseNewArrayFailed)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageEventSubscribeResponseNewIntFailed)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageEventSubscribeResponseSendFailed)

#endif /* __ELOS_MESSAGEEVENTSUBSCRIBE_H__ */

// SPDX-License-Identifier: MIT
#ifndef __ELOS_MESSAGELOGFINDEVENT_H__
#define __ELOS_MESSAGELOGFINDEVENT_H__

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_mocks/mock_jsonc.h>

#include "elos/client_manager/client_manager.h"
#include "elos/common/message.h"
#include "safu/common.h"

typedef struct {
    elosClientManagerConnection_t *connection;
    elosMessage_t *msg;
    json_object *response;
} elosUteststateT_t;

void elosMessageLogFindEventUtestCreateConnection(void **state);
void elosMessageLogFindEventUtestFreeConnection(void **state);
void elosMessageLogFindEventUtestCreateMessage(void **state);
void elosMessageLogFindEventUtestFreeMessage(void **state);

TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageLogFindEventErrConnection)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageLogFindEventErrMessage)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageLogFindEventExtErrEventVectorInitialize)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageLogFindEventExtErrEventVectorToJsonArr)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageLogFindEventExtErrLogAggregatorFindEvents)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageLogFindEventExtErrResponseCreateFailed)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageLogFindEventExtErrRuleFrmMsgGetString)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageLogFindEventExtErrRuleFrmMsgStrdup)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageLogFindEventExtErrRuleFrmMsgTokenerParse)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosMessageLogFindEventSuccess)

#endif /* __ELOS_MESSAGELOGFINDEVENT_H__ */

// SPDX-License-Identifier: MIT
#ifndef __ELOS_MESSAGEHANDLERRESPONSECREATE_H__
#define __ELOS_MESSAGEHANDLERRESPONSECREATE_H__

#include <cmocka_extensions/cmocka_extensions.h>

#include "cmocka_mocks/mock_jsonc.h"
#include "messagehandler/message_handler.h"
#include "safu/mock_safu.h"

void elosTestElosMessageHandlerResponseCreateSuccessNullString(void **state);
void elosTestElosMessageHandlerResponseCreateSuccessErrString(void **state);
void elosTestElosMessageHandlerResponseCreateExterrJsonObject(void **state);
void elosTestElosMessageHandlerResponseCreateExterrAddObject(void **state);
void elosTestElosMessageHandlerResponseCreateExterrAddNewString(void **state);

#endif /* __ELOS_MESSAGEHANDLERRESPONSECREATE_H__ */

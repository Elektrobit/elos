// SPDX-License-Identifier: MIT
#ifndef __ELOS_MESSAGEHANDLERSEND_H__
#define __ELOS_MESSAGEHANDLERSEND_H__

#include <cmocka_extensions/cmocka_extensions.h>
#include <elos/common/message.h>

#include "elos/libelosplugin/message_handler.h"

void elosTestElosMessageHandlerSendSuccess(void **state);
void elosTestElosMessageHandlerSendExterrAllocMem(void **state);
void elosTestElosMessageHandlerSendErrBytesLtZero(void **state);
void elosTestElosMessageHandlerSendErrBytesLtMsgLen(void **state);

#endif /* __ELOS_MESSAGEHANDLERSEND_H__ */

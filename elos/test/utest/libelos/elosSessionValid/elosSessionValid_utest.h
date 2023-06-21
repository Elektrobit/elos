// SPDX-License-Identifier: MIT
#ifndef __ELOS_ELOSSESSIONVALID_H__
#define __ELOS_ELOSSESSIONVALID_H__

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/libelos/libelos.h"

typedef struct elosUnitTestState {
    elosSession_t session;
} elosUnitTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestElosSessionValidSuccess)

#endif /* __ELOS_ELOSSESSIONVALID_H__ */

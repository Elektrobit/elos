// SPDX-License-Identifier: MIT

#ifndef ELOS_SAFU_LOGFUNC_UTEST_H
#define ELOS_SAFU_LOGFUNC_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>
#include <stdlib.h>

#include "safu/log.h"

void safuTestSafuLogFuncSuccess(void **state);
void safuTestSafuLogFuncErrIntSize(void **state);

#endif /* ELOS_SAFU_LOGFUNC_UTEST_H */

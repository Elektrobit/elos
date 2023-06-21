// SPDX-License-Identifier: MIT

#ifndef ELOS_SAFU_LOGLEVEL_FROM_STRING_UTEST_H
#define ELOS_SAFU_LOGLEVEL_FROM_STRING_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>
#include <stdlib.h>

#include "safu/log.h"

void safuTestSafuLogLevelFromStringSuccess(void **state);
void safuTestSafuLogLevelFromStringErrInvalidLogLevel(void **state);

#endif /* ELOS_SAFU_LOGLEVEL_FROM_STRING_UTEST_H */

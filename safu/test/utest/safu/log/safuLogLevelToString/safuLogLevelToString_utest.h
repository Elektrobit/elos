// SPDX-License-Identifier: MIT

#ifndef ELOS_SAFU_LOGLEVEL_TO_STRING_UTEST_H
#define ELOS_SAFU_LOGLEVEL_TO_STRING_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>
#include <stdlib.h>

#include "safu/log.h"

void safuTestSafuLogLevelToStringSuccess(void **state);
void safuTestSafuLogLevelToStringErrInvalidLogLevel(void **state);

#endif /* ELOS_SAFU_LOGLEVEL_TO_STRING_UTEST_H */

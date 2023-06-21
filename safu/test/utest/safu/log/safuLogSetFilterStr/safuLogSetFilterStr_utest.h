// SPDX-License-Identifier: MIT

#ifndef ELOS_SAFU_LOGSET_FILTER_STR_UTEST_H
#define ELOS_SAFU_LOGSET_FILTER_STR_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>
#include <stdlib.h>

#include "safu/log.h"

void safuTestSafuLogSetFilterStringSuccess(void **state);
void safuTestSafuLogSetFilterStringErrToLongFilter(void **state);

#endif /* ELOS_SAFU_LOGSET_FILTER_STR_UTEST_H */

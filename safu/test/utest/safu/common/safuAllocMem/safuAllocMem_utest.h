// SPDX-License-Identifier: MIT

#ifndef SAFU_ALLOC_MEM_UTEST_H
#define SAFU_ALLOC_MEM_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>

#include "safu/common.h"

void safuTestSafuAllocMemSuccessNewMem(void **state);
void safuTestSafuAllocMemSuccessOldptrNull(void **state);
void safuTestSafuAllocMemSuccessNewlenZero(void **state);
void safuTestSafuAllocMemSuccessAllParamZero(void **state);
void safuTestSafuAllocMemExterrRealloc(void **state);

#endif /* SAFU_ALLOC_MEM_UTEST_H */

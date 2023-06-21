// SPDX-License-Identifier: MIT
#pragma once
#include "vector_utest.h"

void safuTestSafuVecInsertConsecutiveWithResize(void **state);
void safuTestSafuVecInsertConsecutiveWithoutResize(void **state);
void safuTestSafuVecInsertWithResize(void **state);
void safuTestSafuVecInsertWithoutResize(void **state);
void safuTestSafuVecInsertFrontWithResize(void **state);
void safuTestSafuVecInsertFrontWithoutResize(void **state);
void safuTestSafuVecInsertErrInvalidIndex(void **state);
void safuTestSafuVecInsertExterrRemalloc(void **state);
void safuTestSafuVecInsertErrNullVector(void **state);

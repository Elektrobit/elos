// SPDX-License-Identifier: MIT
#pragma once
#include "vector_utest.h"

void safuTestSafuVecFindRemoveErrUninitializedVec(void **state);
void safuTestSafuVecFindRemoveErrNoMatchElement(void **state);
void safuTestSafuVecFindRemoveErrNoMatchFunc(void **state);
void safuTestSafuVecFindRemoveElementNotFound(void **state);
void safuTestSafuVecFindRemoveSuccess(void **state);
void safuTestSafuVecFindRemoveSuccessMultiMatch(void **state);
void safuTestSafuVecFindRemoveEmptyVec(void **state);

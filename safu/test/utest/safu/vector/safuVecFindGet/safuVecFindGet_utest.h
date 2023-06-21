// SPDX-License-Identifier: MIT
#pragma once
#include "vector_utest.h"

void safuTestSafuVecFindGetErrUninitializedVec(void **state);
void safuTestSafuVecFindGetErrStartOutOfBounds(void **state);
void safuTestSafuVecFindGetErrNoResultMemory(void **state);
void safuTestSafuVecFindGetErrNoIndex(void **state);
void safuTestSafuVecFindGetErrNoMatchFunc(void **state);
void safuTestSafuVecFindGetErrNoMatchData(void **state);
void safuTestSafuVecFindGetElementNotFound(void **state);
void safuTestSafuVecFindGetSuccess(void **state);
void safuTestSafuVecFindGetEmptyVec(void **state);

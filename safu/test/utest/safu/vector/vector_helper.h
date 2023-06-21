// SPDX-License-Identifier: MIT
#pragma once
#include <stdint.h>
#include <stdlib.h>

#include "vector_utest.h"

int safuMatchUint32(const void *element, const void *data);
void safuFillVecUint32(safuVec_t *vec, uint32_t num);

int safuMatchSafuTdata(const void *element, const void *data);
safuTdata_t safuSafuTDataNumber(uint32_t n);
void safuFillVecSafuTData(safuVec_t *vec, uint32_t num);

// SPDX-License-Identifier: MIT

#ifndef __MOCK_ELOS_COMMON_VECTOR_H__
#define __MOCK_ELOS_COMMON_VECTOR_H__

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>

#ifdef MOCK_SAFU_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "safu/vector.h"

MOCK_FUNC_PROTOTYPE(safuVecCreate, int, safuVec_t *vec, uint32_t elements, uint32_t elementSize)
MOCK_FUNC_PROTOTYPE(safuVecFree, int, safuVec_t *vec)
MOCK_FUNC_PROTOTYPE(safuVecInsert, int, safuVec_t *vec, uint32_t idx, const void *element)
MOCK_FUNC_PROTOTYPE(safuVecRemove, int, safuVec_t *vec, uint32_t idx)
MOCK_FUNC_PROTOTYPE(safuVecPush, int, safuVec_t *vec, const void *element)
MOCK_FUNC_PROTOTYPE(safuVecPop, int, safuVec_t *vec)
MOCK_FUNC_PROTOTYPE(safuVecSet, int, safuVec_t *vec, uint32_t idx, const void *element)
MOCK_FUNC_PROTOTYPE(safuVecGet, void *, const safuVec_t *vec, uint32_t idx)
MOCK_FUNC_PROTOTYPE(safuVecGetLast, void *, const safuVec_t *vec)
MOCK_FUNC_PROTOTYPE(safuVecElements, uint32_t, const safuVec_t *vec)
MOCK_FUNC_PROTOTYPE(safuVecFind, int, const safuVec_t *vec, uint32_t startIdx, safuVecFunc_t matchFunc,
                    const void *matchData, uint32_t *resultIdx)
MOCK_FUNC_PROTOTYPE(safuVecFindGet, int, const safuVec_t *vec, void **element, uint32_t *idx, safuVecFunc_t matchFunc,
                    const void *matchData)
MOCK_FUNC_PROTOTYPE(safuVecFindRemove, int, safuVec_t *vec, safuVecFunc_t matchFunc, const void *matchData)

#endif /* __MOCK_ELOS_COMMON_VECTOR_H__ */

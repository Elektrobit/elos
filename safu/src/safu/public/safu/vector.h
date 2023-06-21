// SPDX-License-Identifier: MIT
#ifndef SAFU_VECTOR_H
#define SAFU_VECTOR_H

#include <safu/vector_types.h>

int safuVecCreate(safuVec_t *vec, uint32_t elements, uint32_t elementSize);
int safuVecFree(safuVec_t *vec);
int safuVecInsert(safuVec_t *vec, uint32_t idx, const void *element);
int safuVecRemove(safuVec_t *vec, uint32_t idx);
int safuVecPush(safuVec_t *vec, const void *element);
int safuVecPop(safuVec_t *vec);
int safuVecSet(safuVec_t *vec, uint32_t idx, const void *element);
void *safuVecGet(const safuVec_t *vec, uint32_t idx);
void *safuVecGetLast(const safuVec_t *vec);
uint32_t safuVecElements(const safuVec_t *vec);
int safuVecFind(const safuVec_t *vec, uint32_t startIdx, safuVecFunc_t matchFunc, const void *matchData,
                uint32_t *resultIdx);
int safuVecFindGet(const safuVec_t *vec, void **element, uint32_t *idx, safuVecFunc_t matchFunc, const void *matchData);
int safuVecFindRemove(safuVec_t *vec, safuVecFunc_t matchFunc, const void *matchData);

#define safuVecIterate(__v, __f, __d) safuVecFind(__v, 0, __f, __d, NULL)

#endif

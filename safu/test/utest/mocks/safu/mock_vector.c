// SPDX-License-Identifier: MIT

#include "mock_vector.h"

#include <stdbool.h>
#include <stdlib.h>

MOCK_FUNC_BODY(safuVecCreate, int, safuVec_t *vec, uint32_t elements, uint32_t elementSize) {
    if (MOCK_IS_ACTIVE(safuVecCreate)) {
        check_expected_ptr(vec);
        check_expected(elements);
        check_expected(elementSize);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(safuVecCreate)(vec, elements, elementSize);
}

MOCK_FUNC_BODY(safuVecFree, int, safuVec_t *vec) {
    if (MOCK_IS_ACTIVE(safuVecFree)) {
        check_expected_ptr(vec);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(safuVecFree)(vec);
}

MOCK_FUNC_BODY(safuVecInsert, int, safuVec_t *vec, uint32_t idx, const void *element) {
    if (MOCK_IS_ACTIVE(safuVecInsert)) {
        check_expected_ptr(vec);
        check_expected(idx);
        check_expected_ptr(element);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(safuVecInsert)(vec, idx, element);
}

MOCK_FUNC_BODY(safuVecRemove, int, safuVec_t *vec, uint32_t idx) {
    if (MOCK_IS_ACTIVE(safuVecRemove)) {
        check_expected_ptr(vec);
        check_expected(idx);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(safuVecRemove)(vec, idx);
}

MOCK_FUNC_BODY(safuVecPush, int, safuVec_t *vec, const void *element) {
    if (MOCK_IS_ACTIVE(safuVecPush)) {
        check_expected_ptr(vec);
        check_expected_ptr(element);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(safuVecPush)(vec, element);
}

MOCK_FUNC_BODY(safuVecPop, int, safuVec_t *vec) {
    if (MOCK_IS_ACTIVE(safuVecPop)) {
        check_expected_ptr(vec);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(safuVecPop)(vec);
}

MOCK_FUNC_BODY(safuVecSet, int, safuVec_t *vec, uint32_t idx, const void *element) {
    if (MOCK_IS_ACTIVE(safuVecSet)) {
        check_expected_ptr(vec);
        check_expected(idx);
        check_expected_ptr(element);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(safuVecSet)(vec, idx, element);
}

MOCK_FUNC_BODY(safuVecGet, void *, const safuVec_t *vec, uint32_t idx) {
    if (MOCK_IS_ACTIVE(safuVecGet)) {
        check_expected_ptr(vec);
        check_expected(idx);
        return mock_ptr_type(void *);
    }
    return MOCK_FUNC_REAL(safuVecGet)(vec, idx);
}

MOCK_FUNC_BODY(safuVecGetLast, void *, const safuVec_t *vec) {
    if (MOCK_IS_ACTIVE(safuVecGetLast)) {
        check_expected_ptr(vec);
        return mock_ptr_type(void *);
    }
    return MOCK_FUNC_REAL(safuVecGetLast)(vec);
}

MOCK_FUNC_BODY(safuVecElements, uint32_t, const safuVec_t *vec) {
    if (MOCK_IS_ACTIVE(safuVecElements)) {
        check_expected_ptr(vec);
        return mock_type(uint32_t);
    }
    return MOCK_FUNC_REAL(safuVecElements)(vec);
}

MOCK_FUNC_VAR_NEW(safuVecFind);
int MOCK_FUNC_WRAP(safuVecFind)(const safuVec_t *vec, uint32_t startIdx, safuVecFunc_t matchFunc, const void *matchData,
                                uint32_t *resultIdx) {
    if (MOCK_IS_ACTIVE(safuVecFind)) {
        check_expected_ptr(vec);
        check_expected(startIdx);
        check_expected_ptr(matchFunc);
        check_expected_ptr(matchData);
        check_expected_ptr(resultIdx);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(safuVecFind)(vec, startIdx, matchFunc, matchData, resultIdx);
}

MOCK_FUNC_VAR_NEW(safuVecFindGet);
int MOCK_FUNC_WRAP(safuVecFindGet)(const safuVec_t *vec, void **element, uint32_t *idx, safuVecFunc_t matchFunc,
                                   const void *matchData) {
    if (MOCK_IS_ACTIVE(safuVecFindGet)) {
        check_expected_ptr(vec);
        check_expected_ptr(element);
        check_expected_ptr(idx);
        check_expected_ptr(matchFunc);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(safuVecFindGet)(vec, element, idx, matchFunc, matchData);
}

MOCK_FUNC_VAR_NEW(safuVecFindRemove);
int MOCK_FUNC_WRAP(safuVecFindRemove)(safuVec_t *vec, safuVecFunc_t matchFunc, const void *matchData) {
    if (MOCK_IS_ACTIVE(safuVecFindRemove)) {
        check_expected_ptr(vec);
        check_expected_ptr(matchFunc);
        check_expected_ptr(matchData);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(safuVecFindRemove)(vec, matchFunc, matchData);
}

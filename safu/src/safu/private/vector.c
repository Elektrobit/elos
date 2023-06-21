// SPDX-License-Identifier: MIT

#include "safu/vector.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define _GET_ELEMENT_PTR(__vec, __idx) ((void *)&((uint8_t *)(__vec)->data)[(__vec)->elementSize * (__idx)])

// TODO: Improve allocating (and freeing?) memory during resize commands

static int safuVecCheck(const safuVec_t *vec) {
    int retval = 0;
    if ((vec == NULL) || (vec->data == NULL) || (vec->memorySize < 1) || (vec->elementSize < 1)) {
        retval = -1;
    }
    return retval;
}

int safuVecCreate(safuVec_t *vec, uint32_t elementCount, uint32_t elementSize) {
    void *ptr = NULL;
    size_t memorySize = 0;

    if ((vec == NULL) || (elementSize < 1)) {
        return -1;
    }

    if (elementCount > 0) {
        memorySize = elementSize * elementCount;
        ptr = malloc(memorySize);
        if (ptr == NULL) {
            return -1;
        }
    }

    vec->data = ptr;
    vec->elementCount = 0;
    vec->elementSize = elementSize;
    vec->memorySize = memorySize;
    return 0;
}

int safuVecFree(safuVec_t *vec) {
    if (vec == 0) {
        return -1;
    }

    free(vec->data);
    memset(vec, 0, sizeof(safuVec_t));

    return 0;
}

int safuVecInsert(safuVec_t *vec, uint32_t idx, const void *element) {
    uint8_t *ptr;
    size_t msize;
    size_t esize;
    uint32_t ecount;
    uint32_t newidx;
    uint32_t oldidx;

    if ((safuVecCheck(vec) < 0) || (idx > vec->elementCount)) {
        return -1;
    }

    ptr = (uint8_t *)vec->data;
    esize = vec->elementSize;
    ecount = vec->elementCount + 1;
    msize = vec->elementSize * ecount;

    if (msize > vec->memorySize) {
        void *newptr;
        newptr = realloc(ptr, msize);
        if (newptr == NULL) {
            return -1;
        }
        ptr = (uint8_t *)newptr;
        vec->memorySize = msize;
        vec->data = (void *)ptr;
    }

    if (idx == vec->elementCount) {
        idx = vec->elementSize * (ecount - 1);
        memmove(&ptr[idx], element, esize);
        vec->elementCount = ecount;
    } else {
        newidx = esize * (idx + 1);
        oldidx = esize * idx;
        memmove(&ptr[newidx], &ptr[oldidx], esize * (ecount - 1 - idx));
        memmove(&ptr[oldidx], element, esize);
        vec->elementCount = ecount;
    }

    return 0;
}

int safuVecRemove(safuVec_t *vec, uint32_t idx) {
    int result = 0;

    if ((safuVecCheck(vec) < 0) || (idx >= vec->elementCount)) {
        result = -1;
    } else {
        size_t const elementSize = vec->elementSize;
        uint32_t const lastIdx = vec->elementCount - 1;
        void *const lastElement = _GET_ELEMENT_PTR(vec, lastIdx);

        if (idx < lastIdx) {
            size_t const moveSize = elementSize * (lastIdx - idx);
            void *const thisElement = _GET_ELEMENT_PTR(vec, idx);
            void *const nextElement = _GET_ELEMENT_PTR(vec, idx + 1);

            memmove(thisElement, nextElement, moveSize);
        }

        memset(lastElement, 0, elementSize);
        vec->elementCount -= 1;
    }

    return result;
}

int safuVecPush(safuVec_t *vec, const void *element) {
    return safuVecInsert(vec, vec->elementCount, element);
}

int safuVecPop(safuVec_t *vec) {
    if (safuVecCheck(vec) < 0) {
        return -1;
    }

    if (vec->elementCount > 0) {
        vec->elementCount -= 1;
    }

    return 0;
}

int safuVecSet(safuVec_t *vec, uint32_t idx, const void *element) {
    uint8_t *ptr;
    size_t pidx;

    if (element == NULL) {
        return -1;
    }

    if ((safuVecCheck(vec) < 0) || (idx >= vec->elementCount)) {
        return -1;
    }

    ptr = vec->data;
    pidx = vec->elementSize * idx;
    memmove(&ptr[pidx], element, vec->elementSize);

    return 0;
}

void *safuVecGet(const safuVec_t *vec, uint32_t idx) {
    uint8_t *ptr;
    size_t pidx;

    if ((safuVecCheck(vec) < 0) || (idx >= vec->elementCount)) {
        return NULL;
    }

    ptr = (uint8_t *)vec->data;
    pidx = vec->elementSize * idx;

    return (void *)&ptr[pidx];
}

void *safuVecGetLast(const safuVec_t *vec) {
    if ((vec == NULL) || (vec->elementCount < 1)) {
        return NULL;
    }

    return safuVecGet(vec, (vec->elementCount - 1));
}

int safuVecFind(const safuVec_t *vec, uint32_t startIdx, safuVecFunc_t matchFunc, const void *matchData,
                uint32_t *resultIdx) {
    uint32_t idx;
    int found = 0;

    if (vec != NULL && vec->elementCount == 0) {
        return 0;
    }

    if ((safuVecCheck(vec) < 0) || ((vec->elementCount > 0) && (startIdx >= vec->elementCount)) ||
        (matchFunc == NULL)) {
        return -1;
    }

    for (idx = startIdx; idx < vec->elementCount; idx++) {
        void *element = safuVecGet(vec, idx);
        int match = matchFunc(element, matchData);
        if (match) {
            if (resultIdx != NULL) {
                *resultIdx = idx;
            }
            found = 1;
            break;
        }
    }

    return found;
}

int safuVecFindGet(const safuVec_t *vec, void **element, uint32_t *idx, safuVecFunc_t matchFunc,
                   const void *matchData) {
    int result = -1;

    if (element != NULL) {
        uint32_t vecIdx = 0;

        if (idx != NULL) {
            vecIdx = *idx;
        }

        result = safuVecFind(vec, vecIdx, matchFunc, matchData, &vecIdx);
        if (result > 0) {
            *element = safuVecGet(vec, vecIdx);
            if (element == NULL) {
                result = -1;
            } else if (idx != NULL) {
                *idx = vecIdx;
            }
        }
    }

    return result;
}

int safuVecFindRemove(safuVec_t *vec, safuVecFunc_t matchFunc, const void *matchData) {
    uint32_t ridx;
    uint32_t widx;

    if ((safuVecCheck(vec) < 0) || (matchFunc == NULL)) {
        return -1;
    }

    if (vec->elementCount == 0) {
        return 0;
    }

    for (ridx = widx = 0; ridx < vec->elementCount; ridx++) {
        void *element = safuVecGet(vec, ridx);
        int match = matchFunc(element, matchData);
        if (!match) {
            if (ridx != widx) {
                safuVecSet(vec, widx, element);
            }
            widx++;
        }
    }

    while (vec->elementCount > widx) {
        safuVecPop(vec);
    }

    return ridx - widx;
}

uint32_t safuVecElements(const safuVec_t *vec) {
    if (safuVecCheck(vec) < 0) {
        return 0;
    }

    return vec->elementCount;
}

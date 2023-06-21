// SPDX-License-Identifier: MIT
#include "vector_helper.h"

#include <stdio.h>
#include <string.h>

int safuMatchUint32(const void *element, const void *data) {
    if (data != NULL && element != NULL) {
        if (*((const uint32_t *)element) == *((const uint32_t *)data)) {
            return 1;
        }
    }
    return 0;
}

void safuFillVecUint32(safuVec_t *vec, uint32_t num) {
    for (uint32_t i = 0; i < num; i++) {
        safuVecPush(vec, &i);
    }
}

int safuMatchSafuTdata(const void *element, const void *data) {
    if (data != NULL && element != NULL) {
        const safuTdata_t *elmnt = element;
        const safuTdata_t *dt = data;
        if (elmnt->val == dt->val && 0 == strncmp(elmnt->str, dt->str, 32)) {
            return 1;
        }
    }
    return 0;
}

safuTdata_t safuSafuTDataNumber(uint32_t n) {
    safuTdata_t elmnt;
    elmnt.val = n;
    snprintf(elmnt.str, 32, "str %d", n);
    return elmnt;
}

void safuFillVecSafuTData(safuVec_t *vec, uint32_t num) {
    for (uint32_t i = 0; i < num; i++) {
        safuTdata_t elmnt = safuSafuTDataNumber(i);
        safuVecPush(vec, &elmnt);
    }
}

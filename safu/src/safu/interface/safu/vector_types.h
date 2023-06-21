// SPDX-License-Identifier: MIT
#pragma once

#include <stddef.h>
#include <stdint.h>

typedef int(safuVecFunc_t)(const void *, const void *);

typedef struct safuVec {
    size_t memorySize;
    size_t elementSize;
    uint32_t elementCount;
    void *data;
} safuVec_t;

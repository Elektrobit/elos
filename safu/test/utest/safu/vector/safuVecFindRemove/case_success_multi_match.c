// SPDX-License-Identifier: MIT
#include <stdio.h>

#include "safuVecFindRemove_utest.h"
#include "vector_helper.h"

#define TEST_PATTERN(input, expectedResult, value)                                                    \
    test_pattern_uint32(input, ARRAY_SIZE(input), expectedResult, ARRAY_SIZE(expectedResult), value); \
    test_pattern_TData(input, ARRAY_SIZE(input), expectedResult, ARRAY_SIZE(expectedResult), value);

void test_pattern_uint32(uint32_t *input, size_t inputSize, uint32_t *expectedResult, size_t resultSize,
                         uint32_t value) {
    size_t i = 0;
    safuVec_t vec = {0};

    safuVecCreate(&vec, inputSize, sizeof(uint32_t));
    for (i = 0; i < inputSize; i++) {
        safuVecPush(&vec, &input[i]);
    }

    safuVecFindRemove(&vec, safuMatchUint32, &value);

    for (i = 0; i < resultSize; i++) {
        void *vecElement = safuVecGet(&vec, i);
        assert_int_equal(*(uint32_t *)vecElement, expectedResult[i]);
    }

    safuVecFree(&vec);
}

void test_pattern_TData(uint32_t *input, size_t inputSize, uint32_t *expectedResult, size_t resultSize,
                        uint32_t valueInt) {
    size_t i = 0;
    safuVec_t vec = {0};
    const safuTdata_t value = safuSafuTDataNumber(valueInt);

    safuVecCreate(&vec, inputSize, sizeof(safuTdata_t));
    for (i = 0; i < inputSize; i++) {
        const safuTdata_t element = safuSafuTDataNumber(input[i]);
        safuVecPush(&vec, &element);
    }

    safuVecFindRemove(&vec, safuMatchSafuTdata, &value);

    for (i = 0; i < resultSize; i++) {
        void *vecElement = safuVecGet(&vec, i);
        const safuTdata_t result = safuSafuTDataNumber(expectedResult[i]);
        int match = safuMatchSafuTdata(vecElement, &result);
        assert_true(match);
    }

    safuVecFree(&vec);
}

void safuTestSafuVecFindRemoveSuccessMultiMatch(UNUSED void **state) {
    uint32_t pattern1[] = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
    uint32_t expectedResult1[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint32_t element = 1;
    PARAM("switching");
    TEST_PATTERN(pattern1, expectedResult1, element)

    PARAM("0,1,0,0,1,0,0");
    uint32_t pattern2[] = {0, 1, 0, 0, 1, 0, 0};
    uint32_t expectedResult2[] = {0, 0, 0, 0, 0};
    TEST_PATTERN(pattern2, expectedResult2, element)

    PARAM("0,1,1,0,0,0");
    uint32_t pattern3[] = {0, 1, 1, 0, 0, 0};
    uint32_t expectedResult3[] = {0, 0, 0, 0};
    TEST_PATTERN(pattern3, expectedResult3, element)

    PARAM("1,0,0,2,3,0,4,5");
    uint32_t pattern4[] = {1, 0, 0, 2, 3, 0, 4, 5};
    uint32_t expectedResult4[] = {1, 2, 3, 4, 5};
    element = 0;
    TEST_PATTERN(pattern4, expectedResult4, element)
}

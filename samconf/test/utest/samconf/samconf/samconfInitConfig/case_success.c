// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfInitConfig_utest.h"

void samconfTestSamconfInitConfigSuccess(UNUSED void **state) {
    int retVal;

    retVal = samconfInitConfig();
    assert_int_equal(retVal, 0);
}

// SPDX-License-Identifier: MIT
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "elos/rpnfilter/rpnfilter.h"
#include "elosLoglineMapperInit_utest.h"
#include "logline_mapper.h"
#include "safu/common.h"

int elosTestElosLoglineMapperInitNullConfigSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosLoglineMapperInitNullConfigTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosLoglineMapperInitNullConfig(UNUSED void **state) {
    elosLoglineMapper_t mapper;
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosLoglineMapper");
    SHOULD("%s", "initialize an mapper filter with 0 since config is null");

    result = elosLoglineMapperInit(&mapper, NULL);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}

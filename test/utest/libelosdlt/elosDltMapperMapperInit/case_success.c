// SPDX-License-Identifier: MIT

#include <bits/types/struct_iovec.h>
#include <safu/defines.h>

#include "elos/libelosdlt/dltmapper.h"
#include "elosDltMapperInit_utest.h"

int elosTestelosDltMapperInitSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestelosDltMapperInitSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestelosDltMapperInitSuccess(UNUSED void **state) {
    safuResultE_t result;

    TEST("elosDltMapperInit");
    SHOULD("%s", "sets defaults to elosDltMapper_t successfully");

    PARAM("Empty defaults");

    elosDltMapper_t testMapperEmpty = {0};
    result = elosDltMapperInit(&testMapperEmpty, NULL, NULL, NULL, 0);
    assert_int_equal(result, SAFU_RESULT_OK);
    result = elosDltMapperDeleteMembers(&testMapperEmpty);
    assert_int_equal(result, SAFU_RESULT_OK);

    PARAM("Non empty defaults");

    elosDltMapper_t testMapper = {0};
    result = elosDltMapperInit(&testMapper, "testFile", "testApp", "testhwid", 1);
    assert_string_equal(testMapper.fileName, "testFile");
    assert_string_equal(testMapper.appId, "testApp");
    assert_string_equal(testMapper.hardwareid, "testhwid");
    assert_int_equal(testMapper.pid, 1);
    assert_int_equal(result, SAFU_RESULT_OK);
    result = elosDltMapperDeleteMembers(&testMapper);
    assert_int_equal(result, SAFU_RESULT_OK);
}

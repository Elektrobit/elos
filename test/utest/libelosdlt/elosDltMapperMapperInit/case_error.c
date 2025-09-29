// SPDX-License-Identifier: MIT

#include <bits/types/struct_iovec.h>
#include <safu/defines.h>
#include <safu/result.h>

#include "elos/libelosdlt/dltmapper.h"
#include "elosDltMapperInit_utest.h"

int elosTestelosDltMapperInitErrorSetup(UNUSED void **state) {
    return 0;
}

int elosTestelosDltMapperInitErrorTeardown(UNUSED void **state) {
    return 0;
}

void elosTestelosDltMapperInitError(UNUSED void **state) {
    safuResultE_t result;

    TEST("elosDltMapperInit");
    SHOULD("%s", "convert dlt data into elos event fails");

    PARAM("Mapper Is NULL");
    result = elosDltMapperInit(NULL, NULL, NULL, NULL, 0);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}

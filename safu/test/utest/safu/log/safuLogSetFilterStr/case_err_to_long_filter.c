// SPDX-License-Identifier: MIT

#include <stdbool.h>
#include <stdio.h>

#include "safuLogSetFilterStr_utest.h"

void safuTestSafuLogSetFilterStringErrToLongFilter(UNUSED void **state) {
    TEST("safuLogSetFilterStr");
    SHOULD("%s", "fail to set a to long filter string");

    const char *toLongFilterStr =
        "example.c;mock_extref_dependency.c;mock_extref_dependency.h;mock_selfref.h;mock_selfref.c;example.c;example.c;"
        "library.c;selfref.c;weak_mock_dependency.c;dependency.c;dependency.h;samconf.c;signature.c;crypto_utils.c;uri."
        "c;samconf.pem;config_backend.c;json_backend.c;run_benchmarks.sh;samconfBase64Decode_utest.h;smconf"
        "JsonBackendClose_utest.c;case_fopenerror.c";

    safuLogStatusE_t res = safuLogSetFilterStr(toLongFilterStr);
    assert_int_equal(res, SAFU_LOG_STATUS_ERROR);
}

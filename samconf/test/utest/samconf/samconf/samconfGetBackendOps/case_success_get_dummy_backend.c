// SPDX-License-Identifier: MIT
#include <cmocka_extensions/cmocka_extensions.h>
#include <stdint.h>

#include "samconf/config_backend.h"
#include "samconf/samconf.h"
#include "samconfGetBackendOps_utest.h"

void samconfTestSamconfGetBackendOpsSuccessGetDummyBackend(UNUSED void **state) {
    const samconfConfigBackendOps_t *opsDummy;

    TEST("samconfGetBackendOps with index of dummy Ops");
    SHOULD("%s", "return dummy Ops");

    opsDummy = samconfGetBackendOps(1);

    assert_int_equal(opsDummy->supports("", NULL), SAMCONF_CONFIG_NOT_IMPLEMENTED);
    assert_int_equal(opsDummy->open(NULL, NULL), SAMCONF_CONFIG_NOT_IMPLEMENTED);
    assert_int_equal(opsDummy->load(NULL, false, NULL), SAMCONF_CONFIG_NOT_IMPLEMENTED);
    assert_int_equal(opsDummy->close(NULL), SAMCONF_CONFIG_NOT_IMPLEMENTED);
}

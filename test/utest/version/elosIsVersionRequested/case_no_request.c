// SPDX-License-Identifier: MIT
#include "elosIsVersionRequested_utest.h"

void elosTestElossVersionRequestedNoRequest(UNUSED void **state) {
    const char *versionNotRequested[] = {"abcd", "defg"};
    assert_false(elosIsVersionRequested(versionNotRequested, 2));
}

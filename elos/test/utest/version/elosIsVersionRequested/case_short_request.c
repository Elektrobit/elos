// SPDX-License-Identifier: MIT
#include "elosIsVersionRequested_utest.h"

void elosTestElossVersionRequestedShortRequest(UNUSED void **state) {
    const char *versionRequestedShortSingle[] = {"-v"};
    const char *versionRequestedShort[] = {"abcd", "defg", "-v", "hijk"};

    assert_true(elosIsVersionRequested(versionRequestedShortSingle, 1));
    assert_true(elosIsVersionRequested(versionRequestedShort, 4));
    assert_true(elosIsVersionRequested(versionRequestedShort, 3));
}

// SPDX-License-Identifier: MIT
#include "elosIsVersionRequested_utest.h"

void elosTestElossVersionRequestedLongRequest(UNUSED void **state) {
    const char *versionRequestedLongSingle[] = {"--version"};
    const char *versionRequestedLong[] = {"abcd", "defg", "--version", "hijk"};

    assert_true(elosIsVersionRequested(versionRequestedLongSingle, 1));
    assert_true(elosIsVersionRequested(versionRequestedLong, 4));
    assert_true(elosIsVersionRequested(versionRequestedLong, 3));
}

// SPDX-License-Identifier: MIT
#include "elosIsVersionRequested_utest.h"

void elosTestElossVersionRequestedEmptyRequest(UNUSED void **state) {
    const char *versionRequestIsEmpty[] = {0};
    assert_false(elosIsVersionRequested(versionRequestIsEmpty, 0));
}

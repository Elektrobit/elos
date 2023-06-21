// SPDX-License-Identifier: MIT
#include "elosIsVersionRequested_utest.h"

void elosTestElossVersionRequestedNullParam(UNUSED void **state) {
    assert_false(elosIsVersionRequested(NULL, 0));
}

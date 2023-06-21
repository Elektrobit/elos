// SPDX-License-Identifier: MIT

#include "elosEventVectorDelete_utest.h"

int elosTestElosEventVectorDeleteErrEventVectorSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventVectorDeleteErrEventVectorTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventVectorDeleteErrEventVector(UNUSED void **state) {
    TEST("elosEventVectorDelete");
    SHOULD("%s", "do nothing when eventVector is null");
    elosEventVectorDelete(NULL);
}

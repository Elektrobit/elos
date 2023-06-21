// SPDX-License-Identifier: MIT
#include "safuAllocMem_utest.h"

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(safuTestSafuAllocMemSuccessNewMem), cmocka_unit_test(safuTestSafuAllocMemSuccessOldptrNull),
        cmocka_unit_test(safuTestSafuAllocMemSuccessNewlenZero),
        cmocka_unit_test(safuTestSafuAllocMemSuccessAllParamZero), cmocka_unit_test(safuTestSafuAllocMemExterrRealloc)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}

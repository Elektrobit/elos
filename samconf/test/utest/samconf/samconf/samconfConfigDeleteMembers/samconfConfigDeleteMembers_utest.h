// SPDX-License-Identifier: MIT
#ifndef SAMCONF_CONFIGDELETEMEMBERS_UTEST_H
#define SAMCONF_CONFIGDELETEMEMBERS_UTEST_H

#include <cmocka_extensions/mock_extensions.h>

#include "samconf_utest.h"

#define CHILD_COUNT 2

samconfConfig_t *samconfTestSamconfConfigDeleteMembersConfigInit(size_t childCount, samconfConfigValueTypeE_t type);

TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigDeleteMembersSuccess)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigDeleteMembersNullConfiguration)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfConfigDeleteMembersExtErrConfigDelete)

#endif /*SAMCONF_CONFIGDELETEMEMBERS_UTEST_H */

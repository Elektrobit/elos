// SPDX-License-Identifier: MIT
#include "samconfConfigDeleteMembers_utest.h"

#include <string.h>

TEST_SUITE_FUNC_PROTOTYPES(samconfConfigDeleteMembersUtest)

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(samconfTestSamconfConfigDeleteMembersSuccess),
                                       TEST_CASE(samconfTestSamconfConfigDeleteMembersNullConfiguration),
                                       TEST_CASE(samconfTestSamconfConfigDeleteMembersExtErrConfigDelete)};

    return RUN_TEST_SUITE(tests, samconfConfigDeleteMembersUtest);
}

samconfConfig_t *samconfTestSamconfConfigDeleteMembersConfigInit(size_t childCount, samconfConfigValueTypeE_t type) {
    char *testType = "String Type";

    samconfConfig_t *config = malloc(sizeof(*config));
    assert_non_null_msg(config, "failed to allocate config");

    config->key = malloc(sizeof(*config->key));
    assert_non_null_msg(config->key, "failed to allocate config key");

    config->type = type;

    if (type == SAMCONF_CONFIG_VALUE_STRING) {
        config->value.string = strdup(testType);
    } else if (type == SAMCONF_CONFIG_VALUE_INT) {
        config->value.integer = 0;
    } else {
        config->value.boolean = false;
    }

    config->childCount = childCount;

    if (childCount > 0) {
        config->children = malloc(sizeof(*config) * childCount);
        assert_non_null_msg(config->children, "failed to allocate config children");
    }

    return config;
}

static int samconfConfigDeleteMembersUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfConfigDeleteMembersUtestTeardown(UNUSED void **state) {
    return 0;
}

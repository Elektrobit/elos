// SPDX-License-Identifier: MIT
#include "elosPluginFilterLoaderLoad_utest.h"

#include <samconf/samconf.h>
#include <string.h>

TEST_SUITE_FUNC_PROTOTYPES(elosPluginFilterLoaderLoadUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosPluginFilterLoaderLoadErrInvalidParameter),
        TEST_CASE(elosTestElosPluginFilterLoaderLoadErrNoFiltersDefined),
        TEST_CASE(elosTestElosPluginFilterLoaderLoadErrNoFilterSectionDefined),
        TEST_CASE(elosTestElosPluginFilterLoaderLoadSuccess),
    };
    return RUN_TEST_SUITE(tests, elosPluginFilterLoaderLoadUtest);
}

static int elosPluginFilterLoaderLoadUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosPluginFilterLoaderLoadUtestTeardown(UNUSED void **state) {
    return 0;
}

samconfConfig_t *elosCreateConfigNode(samconfConfig_t *parent, const char *name) {
    samconfConfig_t *newConfig = NULL;
    samconfConfigNew(&newConfig);
    if (parent != NULL) {
        samconfConfigAdd(parent, newConfig);
    } else {
        newConfig->parent = NULL;
    }
    if (name != NULL) {
        newConfig->key = strdup(name);
    } else {
        newConfig->key = NULL;
    }
    return newConfig;
}

samconfConfig_t *elosCreateConfigString(samconfConfig_t *parent, const char *name, const char *value) {
    samconfConfig_t *newConfig = elosCreateConfigNode(parent, name);
    newConfig->value.string = strdup(value);
    newConfig->type = SAMCONF_CONFIG_VALUE_STRING;
    return newConfig;
}

// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <safu/result.h>
#include <samconf/samconf.h>
#include <samconf/samconf_types.h>
#include <sys/stat.h>

#include "elosConfigLoad_utest.h"

int elosTestElosConfigLoadEnvVariableConfigErrorSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosConfigLoadEnvVariableConfigErrorTeardown(UNUSED void **state) {
    return 0;
}

int elosTestDummyEnvStat(UNUSED const char *pathname, struct stat *statbuf) {
    statbuf->st_mode = S_IFDIR;
    return 0;
}

static int _testLocations(const LargestIntegralType value, const LargestIntegralType checkValueData) {
    int ret = 0;
    int err = 0;
    const char **checkData = (const char **)checkValueData;
    samconfConfigLocation_t *testLocations = (samconfConfigLocation_t *)value;

    for (size_t i = 0; checkData[i] != NULL; i++) {
        ret = strcmp(checkData[i], testLocations[i].path);
        if (ret != 0) {
            print_message("Invalid location %s != %s", checkData[i], testLocations[i].path);
            ++err;
        }
    }

    return (err == 0 ? true : false);
}

void elosTestElosConfigLoadEnvVariableConfigError(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_OK;
    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;
    samconfConfig_t *testConfig = NULL;
    const char *testData[] = {"/etc/elos/elos.d/", "/etc/elos/client.d/", "/etc/elos/scanner.d/",
                              "/etc/elos/eventlogging.d/", NULL};
    const char *testEnvData[] = {"env://?envPrefix=elos&", NULL};

    TEST("elosConfigLoad");
    SHOULD("fail as env config load and merge fails");

    status = samconfConfigNew(&testConfig);
    assert_int_equal(status, SAMCONF_CONFIG_OK);

    MOCK_FUNC_AFTER_CALL(safuGetEnvOr, 0);
    expect_string(__wrap_safuGetEnvOr, key, "ELOS_CONFIG_PATH");
    expect_any(__wrap_safuGetEnvOr, defaultValue);
    will_return(__wrap_safuGetEnvOr, "/etc/elos");

    MOCK_FUNC_AFTER_CALL_WITH(stat, 0, elosTestDummyEnvStat, NULL);

    MOCK_FUNC_AFTER_CALL(samconfLoad, 0);
    expect_any(__wrap_samconfLoad, location);
    expect_any(__wrap_samconfLoad, enforceSignature);
    expect_any(__wrap_samconfLoad, config);
    will_set_parameter(__wrap_samconfLoad, config, testConfig);
    will_return(__wrap_samconfLoad, SAMCONF_CONFIG_OK);

    MOCK_FUNC_AFTER_CALL(samconfConfigGetBool, 0);
    expect_any(__wrap_samconfConfigGetBool, root);
    expect_any(__wrap_samconfConfigGetBool, path);
    expect_any(__wrap_samconfConfigGetBool, result);
    will_set_parameter(__wrap_samconfConfigGetBool, result, true);
    will_return(__wrap_samconfConfigGetBool, SAMCONF_CONFIG_OK);

    MOCK_FUNC_ALWAYS(samconfLoadAndMerge);
    expect_check(__wrap_samconfLoadAndMerge, locations, _testLocations, testData);
    expect_value(__wrap_samconfLoadAndMerge, locationsSize, 4);
    expect_value(__wrap_samconfLoadAndMerge, config, &testConfig);
    will_set_parameter(__wrap_samconfLoadAndMerge, config, testConfig);
    will_return(__wrap_samconfLoadAndMerge, SAMCONF_CONFIG_OK);

    expect_check(__wrap_samconfLoadAndMerge, locations, _testLocations, testEnvData);
    expect_value(__wrap_samconfLoadAndMerge, locationsSize, 1);
    expect_value(__wrap_samconfLoadAndMerge, config, &testConfig);
    will_set_parameter(__wrap_samconfLoadAndMerge, config, testConfig);
    will_return(__wrap_samconfLoadAndMerge, SAMCONF_CONFIG_NOT_FOUND);

    result = elosConfigLoad(&testConfig);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    MOCK_FUNC_DISABLE(samconfLoadAndMerge);
}

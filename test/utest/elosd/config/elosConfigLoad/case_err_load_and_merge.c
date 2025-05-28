// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <safu/result.h>
#include <samconf/samconf_types.h>
#include <sys/stat.h>

#include "elosConfigLoad_utest.h"

int elosTestElosConfigLoadMultipleConfigsErrorSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosConfigLoadMultipleConfigsErrorTeardown(UNUSED void **state) {
    return 0;
}

int elosTestDummyMultiConfigStat(UNUSED const char *pathname, struct stat *statbuf) {
    statbuf->st_mode = S_IFREG;
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
            print_message("Invalid location %s != %s\n", checkData[i], testLocations[i].path);
            ++err;
        }
    }

    return (err == 0 ? true : false);
}

void elosTestElosConfigLoadMultipleConfigsError(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_OK;
    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;
    samconfConfig_t *testConfig = NULL;
    const char *testData[] = {"/etc/elos/elos.d/", "/etc/elos/client.d/", "/etc/elos/scanner.d/",
                              "/etc/elos/eventlogging.d/", NULL};

    TEST("elosConfigLoad");
    SHOULD("not load multiple config as merge fails");

    status = samconfConfigNew(&testConfig);
    assert_int_equal(status, SAMCONF_CONFIG_OK);

    MOCK_FUNC_AFTER_CALL(safuGetEnvOr, 0);
    expect_string(__wrap_safuGetEnvOr, key, "ELOS_CONFIG_PATH");
    expect_any(__wrap_safuGetEnvOr, defaultValue);
    will_return(__wrap_safuGetEnvOr, "/etc/elos/elosd.json");

    MOCK_FUNC_AFTER_CALL_WITH(stat, 0, elosTestDummyMultiConfigStat, NULL);

    MOCK_FUNC_AFTER_CALL(samconfLoad, 0);
    expect_any(__wrap_samconfLoad, location);
    expect_any(__wrap_samconfLoad, enforceSignature);
    expect_any(__wrap_samconfLoad, config);
    will_set_parameter(__wrap_samconfLoad, config, testConfig);
    will_return(__wrap_samconfLoad, SAMCONF_CONFIG_OK);

    MOCK_FUNC_AFTER_CALL(dirname, 0);
    expect_any(__wrap_dirname, path);
    will_return(__wrap_dirname, "/etc/elos");

    MOCK_FUNC_AFTER_CALL(samconfConfigGetBool, 0);
    expect_any(__wrap_samconfConfigGetBool, root);
    expect_any(__wrap_samconfConfigGetBool, path);
    expect_any(__wrap_samconfConfigGetBool, result);
    will_set_parameter(__wrap_samconfConfigGetBool, result, false);
    will_return(__wrap_samconfConfigGetBool, SAMCONF_CONFIG_OK);

    MOCK_FUNC_AFTER_CALL(samconfLoadAndMerge, 0);
    expect_check(__wrap_samconfLoadAndMerge, locations, _testLocations, testData);
    expect_value(__wrap_samconfLoadAndMerge, locationsSize, 4);
    expect_value(__wrap_samconfLoadAndMerge, config, &testConfig);
    will_set_parameter(__wrap_samconfLoadAndMerge, config, testConfig);
    will_return(__wrap_samconfLoadAndMerge, SAMCONF_CONFIG_NOT_FOUND);

    result = elosConfigLoad(&testConfig);
    assert_int_equal(result, SAFU_RESULT_OK);

    samconfConfigDelete(testConfig);
}

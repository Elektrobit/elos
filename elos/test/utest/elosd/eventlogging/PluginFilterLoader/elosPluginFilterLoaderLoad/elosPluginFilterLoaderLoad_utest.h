// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <elos/eventlogging/PluginFilterLoader.h>

TEST_CASE_FUNC_PROTOTYPES(elosTestElosPluginFilterLoaderLoadErrInvalidParameter)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosPluginFilterLoaderLoadErrNoFiltersDefined)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosPluginFilterLoaderLoadErrNoFilterSectionDefined)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosPluginFilterLoaderLoadSuccess)

samconfConfig_t *elosCreateConfigNode(samconfConfig_t *parent, const char *name);
samconfConfig_t *elosCreateConfigString(samconfConfig_t *parent, const char *name, const char *value);

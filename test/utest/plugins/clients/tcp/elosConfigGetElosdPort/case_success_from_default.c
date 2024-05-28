// SPDX-License-Identifier: MIT

#include <samconf/test_utils.h>

#include "elosConfigGetElosdPort_utest.h"

int elosTestElosConfigGetElosdPortSuccessFromDefaultSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosConfigGetElosdPortSuccessFromDefaultTeardown(UNUSED void **state) {
    return 0;
}

#define TCP_CONFIG \
    "\
  \"LocalTcpClient\": {\
      \"Config\": {\
      }\
  }\
"

void elosTestElosConfigGetElosdPortSuccessFromDefault(UNUSED void **state) {
    TEST("elosTcpConfigGetPort");
    SHOULD("%s", "get the elos port default option");

    samconfConfig_t mockConfig = {0};
    samconfUtilCreateMockConfigFromStr("{\"Config\":{}}", false, &mockConfig);
    elosPlugin_t plugin = {.useEnv = false, .config = &mockConfig};

    int port = elosTcpConfigGetPort(&plugin);
    assert_int_equal(ELOSD_PORT, port);

    samconfConfigDeleteMembers(&mockConfig);
}

// SPDX-License-Identifier: MIT

#include "mock_unix_config.h"

#include <cmocka_extensions/cmocka_extensions.h>

MOCK_FUNC_BODY(elosUnixConfigGetPath, const char *, elosPlugin_t const *plugin) {
    const char *result;

    switch (MOCK_GET_TYPE(elosUnixConfigGetPath)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(elosUnixConfigGetPath)(plugin);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(plugin);
            result = mock_type(const char *);
            break;
        default:
            result = MOCK_FUNC_REAL(elosUnixConfigGetPath)(plugin);
            break;
    }

    return result;
}

MOCK_FUNC_BODY(elosUnixConfigGetSocketAddress, safuResultE_t, elosPlugin_t const *plugin, struct sockaddr_un *addr) {
    safuResultE_t result;

    switch (MOCK_GET_TYPE(elosUnixConfigGetSocketAddress)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(elosUnixConfigGetSocketAddress)(plugin, addr);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(plugin);
            check_expected_ptr(addr);
            result = mock_type(safuResultE_t);
            break;
        default:
            result = MOCK_FUNC_REAL(elosUnixConfigGetSocketAddress)(plugin, addr);
            break;
    }

    return result;
}

MOCK_FUNC_BODY(elosUnixConfigGetConnectionLimit, int, elosPlugin_t const *plugin) {
    int result;

    switch (MOCK_GET_TYPE(elosUnixConfigGetConnectionLimit)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(elosUnixConfigGetConnectionLimit)(plugin);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(plugin);
            result = mock_type(int);
            break;
        default:
            result = MOCK_FUNC_REAL(elosUnixConfigGetConnectionLimit)(plugin);
            break;
    }

    return result;
}

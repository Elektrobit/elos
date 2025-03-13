// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>

#ifdef MOCK_UNIX_CONFIG_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "unix_config/config.h"

MOCK_FUNC_PROTOTYPE(elosUnixConfigGetPath, const char *, elosPlugin_t const *plugin)
MOCK_FUNC_PROTOTYPE(elosUnixConfigGetSocketAddress, safuResultE_t, elosPlugin_t const *plugin, struct sockaddr_un *addr)
MOCK_FUNC_PROTOTYPE(elosUnixConfigGetConnectionLimit, int, elosPlugin_t const *plugin)

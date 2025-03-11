// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/mock_extensions.h>
#ifdef MOCK_ELOS_LIBELOSPLUGIN_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "mock_plugincontrol.h"
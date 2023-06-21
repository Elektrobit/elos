// SPDX-License-Identifier: MIT
#pragma once

#ifndef UNUSED
#define UNUSED __attribute__((unused))
#endif

#ifndef WEAK
#define WEAK __attribute((weak))
#endif

#ifndef PACKED
#define PACKED __attribute__((packed))
#endif

#ifndef NOINLINE
#define NOINLINE __attribute__((noinline))
#endif

#ifndef VISIBLE
#define VISIBLE __attribute__((visibility("default")))
#endif

#ifndef HIDDEN
#define HIDDEN __attribute__((visibility("hidden")))
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))
#endif

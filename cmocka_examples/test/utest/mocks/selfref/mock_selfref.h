#ifndef _MOCK_EXAMPLE_SELFREF_H
#define _MOCK_EXAMPLE_SELFREF_H

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>

#ifdef MOCK_EXAMPLE_SELFREF_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "selfref.h"

MOCK_FUNC_PROTOTYPE(function, int, int i)
MOCK_FUNC_PROTOTYPE(dependency, int, int a, int b)

#endif /* _MOCK_EXAMPLE_SELFREF_H */

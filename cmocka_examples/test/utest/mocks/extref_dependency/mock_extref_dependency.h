#ifndef _MOCK_EXAMPLE_EXTREF_DEPENDENCY_H
#define _MOCK_EXAMPLE_EXTREF_DEPENDENCY_H

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>

#ifdef MOCK_EXAMPLE_EXTREF_DEPENDENCY_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "dependency.h"

MOCK_FUNC_PROTOTYPE(dependency, int, int a, int b)

#endif /* _MOCK_EXAMPLE_EXTREF_DEPENDENCY_H */

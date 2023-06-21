// SPDX-License-Identifier: MIT

// There are no include guards for this header as we may need
// to (re)define the macros below multiple times for
// a compilation unit that uses more than one mocking library

#undef MOCK_FUNC_WRAP
#undef MOCK_FUNC_REAL
#undef MOCK_FUNC_PROTOTYPE
#undef MOCK_FUNC_BODY

#define MOCK_FUNC_WRAP(__mockf) __wrap##_##__mockf
#define MOCK_FUNC_REAL(__mockf) __real##_##__mockf
#define MOCK_FUNC_PROTOTYPE(__fn, __fr, ...) \
    MOCK_FUNC_VAR_EXTERN(__fn);              \
    __fr MOCK_FUNC_WRAP(__fn)(__VA_ARGS__);  \
    __fr MOCK_FUNC_REAL(__fn)(__VA_ARGS__);

#define MOCK_FUNC_BODY(__fn, __fr, ...) \
    MOCK_FUNC_VAR_NEW(__fn);            \
    __fr MOCK_FUNC_WRAP(__fn)(__VA_ARGS__)

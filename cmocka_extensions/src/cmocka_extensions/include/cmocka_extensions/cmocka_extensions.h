// SPDX-License-Identifier: MIT
#ifndef __CMOCKA_EXTENSIONS_H__
#define __CMOCKA_EXTENSIONS_H__

// clang-format off
// because this order is a cmocka requirement
#include <stdlib.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <float.h>
#include <cmocka.h>
// clang-format on

#define UNUSED __attribute__((unused))

#define SKIP(fmt, ...)                                            \
    print_error("# skip %s: " fmt "\n", __func__, ##__VA_ARGS__); \
    skip()

#define TEST(method)     print_message("# test " method "\n")
#define SHOULD(fmt, ...) print_message("# should " fmt "\n", ##__VA_ARGS__)
#define PARAM(fmt, ...)  print_message("# test param: " fmt "\n", ##__VA_ARGS__)

#define TEST_CASE(__name) cmocka_unit_test_setup_teardown(__name, __name##Setup, __name##Teardown)
#define TEST_CASE_FUNC_PROTOTYPES(__name) \
    void __name(void **state);            \
    int __name##Setup(void **state);      \
    int __name##Teardown(void **state);

#define TEST_SUITE_FUNC_PROTOTYPES(__name)  \
    static int __name##Setup(void **state); \
    static int __name##Teardown(void **state);

#define RUN_TEST_SUITE(__testsuite, __name) cmocka_run_group_tests(__testsuite, __name##Setup, __name##Teardown)

#define ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))

/* Used by clang analyser to check if allocated memory is freed */
#ifdef __clang_analyzer__
#define FREE_FOR_CLANG_ANALYSER(NAME, CONFIG) free(CONFIG);
#else
#define FREE_FOR_CLANG_ANALYSER(NAME, CONFIG)
#endif

#define will_set_errno(__mockf, __value) __mockf##_errno = __value
#define will_return_and_set_errno(__mockf, __retval, __errno) \
    do {                                                      \
        __mockf##_errno = __errno;                            \
        will_set_errno(__mockf, __errno);                     \
        will_return(__mockf, __retval);                       \
    } while (0)

#define will_return_success(__mockf, __retval) \
    do {                                       \
        will_return(__mockf, 0);               \
        will_return(__mockf, __retval);        \
    } while (0)

#define will_return_error(__mockf) will_return(__mockf, -1)

static inline void assert_non_null_msg(void *ptr, const char *msg) {  // NOLINT
    if (ptr == NULL) {
        free(ptr);
        fail_msg("%s", msg);
        __builtin_unreachable();
    }
}

#ifndef assert_double_equal
/*
 * provide an implementation to compare double for cmocka prior to 1.7
 */
static inline void assert_double_equal(const double left, const double right, const double epsilon) {  // NOLINT
    double absLeft;
    double absRight;
    double largest;
    double relDiff;

    double diff = left - right;
    diff = (diff >= 0.f) ? diff : -diff;

    if (diff > epsilon) {
        absLeft = (left >= 0.f) ? left : -left;
        absRight = (right >= 0.f) ? right : -right;

        largest = (absRight > absLeft) ? absRight : absLeft;
        relDiff = largest * FLT_EPSILON;

        if (diff > relDiff) {
            fail_msg("%f != %f", left, right);
        }
    }
}
#endif

#define will_set_parameter(METHOD, PARAM_NAME, VALUE) will_return(METHOD, VALUE);
#endif /* __CMOCKA_EXTENSIONS_H__ */

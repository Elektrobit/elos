// SPDX-License-Identifier: MIT
#ifndef __MOCK_EXTENSIONS_H__
#define __MOCK_EXTENSIONS_H__

#include <stdbool.h>
#include <stddef.h>

typedef void cmockaMockFunc_t(void);

typedef struct {
    int afterCall;
    cmockaMockFunc_t *funcPtr;
    void *funcData;
} cmockaMock_t;

typedef enum {
    CMOCKA_MOCK_DISABLED = 0,
    CMOCKA_MOCK_ENABLED,
    CMOCKA_MOCK_ENABLED_WITH_FUNC,
} cmockaMockTypeE_t;

#define CMOCKA_MOCK_INIT \
    { .afterCall = -1, .funcPtr = NULL, .funcData = NULL }

#define MOCK_FUNC_AFTER_CALL_WITH(__mockf, __mocka, __mockfp, __mockfd) \
    {                                                                   \
        __mock##_##__mockf.afterCall = __mocka;                         \
        __mock##_##__mockf.funcPtr = (cmockaMockFunc_t *)__mockfp;      \
        __mock##_##__mockf.funcData = (void *)__mockfd;                 \
    }

#define MOCK_FUNC_AFTER_CALL(__mockf, __mocka) MOCK_FUNC_AFTER_CALL_WITH(__mockf, __mocka, NULL, NULL)
#define MOCK_FUNC_ENABLE(__mockf)              MOCK_FUNC_AFTER_CALL_WITH(__mockf, 0, NULL, NULL)
#define MOCK_FUNC_DISABLE(__mockf)             MOCK_FUNC_AFTER_CALL_WITH(__mockf, -1, NULL, NULL)

/*  If using MOCK_FUNC_ALWAYS in a test case make sure to call MOCK_FUNC_DISABLE on
    cleanup or other test cases would be effected by MOCK_FUNC_ALWAYS.
*/
#define MOCK_FUNC_ALWAYS_WITH(__mockf, __mockfp, __mockfd) MOCK_FUNC_AFTER_CALL_WITH(__mockf, -2, __mockfp, __mockfd);
#define MOCK_FUNC_ALWAYS(__mockf)                          MOCK_FUNC_AFTER_CALL_WITH(__mockf, -2, NULL, NULL)
#define MOCK_FUNC_NEVER(__mockf)                           MOCK_FUNC_DISABLE(__mockf)

#define MOCK_FUNC_TYPEDEF_NEW(__name, __result, ...) typedef __result __mock##_##__name##_##t(__VA_ARGS__)
#define MOCK_FUNC_TYPEDEF(__name)                    __mock##_##__name##_##t
#define MOCK_FUNC_VAR_NEW(__name)                    cmockaMock_t __mock##_##__name = CMOCKA_MOCK_INIT
#define MOCK_FUNC_VAR_EXTERN(__name)                 extern cmockaMock_t __mock##_##__name
#define MOCK_FUNC_VAR(__name)                        __mock##_##__name
#define MOCK_FUNC_PTR(__name)                        __mock##_##__name.funcPtr
#define MOCK_FUNC_DATA(__name)                       __mock##_##__name.funcData
#define MOCK_FUNC_DATA_NEW(__name, __type, __func)   __type *__name = (__type *)MOCK_FUNC_DATA(__func)

static inline cmockaMockTypeE_t _cmockaMockGetType(cmockaMock_t *mock) {
    cmockaMockTypeE_t result = CMOCKA_MOCK_DISABLED;
    if (mock->afterCall == -2) {
        result = CMOCKA_MOCK_ENABLED;
    } else if (mock->afterCall == 0) {
        mock->afterCall = -1;
        result = CMOCKA_MOCK_ENABLED;
    } else if (mock->afterCall > 0) {
        mock->afterCall -= 1;
    }
    if ((result == CMOCKA_MOCK_ENABLED) && (mock->funcPtr != NULL)) {
        result = CMOCKA_MOCK_ENABLED_WITH_FUNC;
    }
    return result;
}

#define MOCK_GET_TYPE(__mfunc)  _cmockaMockGetType(&__mock##_##__mfunc)
#define MOCK_IS_ACTIVE(__mfunc) (bool)MOCK_GET_TYPE(__mfunc)

#endif /* __MOCK_EXTENSIONS_H__ */

// SPDX-License-Identifier: MIT
#ifndef __MOCK_EXTENSIONS_H__
#define __MOCK_EXTENSIONS_H__

#include <stdbool.h>

typedef struct {
    int afterCall;
} cmockaMock_t;

#define CMOCKA_MOCK_INIT \
    { .afterCall = -1 }

/*  If using MOCK_FUNC_ALWAYS in a test case make sure to call MOCK_FUNC_DISABLE on
    cleanup or other test cases would be effected by MOCK_FUNC_ALWAYS.
*/
#define MOCK_FUNC_AFTER_CALL(__mockf, __mocka) __mock##_##__mockf.afterCall = __mocka;
#define MOCK_FUNC_ENABLE(__mockf)              MOCK_FUNC_AFTER_CALL(__mockf, 0)
#define MOCK_FUNC_DISABLE(__mockf)             MOCK_FUNC_AFTER_CALL(__mockf, -1)
#define MOCK_FUNC_ALWAYS(__mockf)              MOCK_FUNC_AFTER_CALL(__mockf, -2)
#define MOCK_FUNC_NEVER(__mockf)               MOCK_FUNC_DISABLE(__mockf)

#define MOCK_FUNC_VAR_NEW(__name)    cmockaMock_t __mock##_##__name = CMOCKA_MOCK_INIT
#define MOCK_FUNC_VAR_EXTERN(__name) extern cmockaMock_t __mock##_##__name
#define MOCK_FUNC_VAR(__name)        __mock##_##__name

#define MOCK_IS_ACTIVE(__mfunc) _doMock(&__mock##_##__mfunc)
static inline bool _doMock(cmockaMock_t *mock) {
    if (mock->afterCall > 0) {
        mock->afterCall -= 1;
    } else if (mock->afterCall == 0) {
        mock->afterCall = -1;
        return true;
    } else if (mock->afterCall == -2) {
        return true;
    }
    return false;
}

#endif /* __MOCK_EXTENSIONS_H__ */

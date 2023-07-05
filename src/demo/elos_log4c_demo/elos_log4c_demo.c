// SPDX-License-Identifier: MIT

#include <log4c/category.h>
#include <log4c/init.h>
#include <log4c/priority.h>
#include <stdio.h>

#include "log4c.h"

extern int elosInitElosLog4cLib(void);

int main() {
    int ret = 0;
    log4c_category_t *elosCategory = NULL;

    elosInitElosLog4cLib();

    if (log4c_init()) {
        printf("log4c_init failed");
        ret = 1;
    } else {
        elosCategory = log4c_category_get("log4c.elos.demo.log4cdemo");

        log4c_category_log(elosCategory, LOG4C_PRIORITY_ERROR, "log4c error demo in elos");
        log4c_category_log(elosCategory, LOG4C_PRIORITY_DEBUG, "log4c debug demo in elos");

        if (log4c_fini()) {
            printf("log4c fini failed");
        }
    }
    return ret;
}

// SPDX-License-Identifier: MIT

#include <log4c.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNUSED __attribute__((unused))

extern int elosInitElosAppender(void);

static const char *elosFormat(UNUSED const log4c_layout_t *aLayout, const log4c_logging_event_t *aEvent) {
    static char buffer[4096];

    sprintf(buffer, "[ELOSD][%s][LINE:%d][FILE:%s] %s\n", aEvent->evt_category, aEvent->evt_loc->loc_line,
            aEvent->evt_loc->loc_file, aEvent->evt_msg);

    return buffer;
}

const log4c_layout_type_t elosLog4cLayoutTypeElos = {
    "elos_formatter",
    elosFormat,
};

static const log4c_layout_type_t *const elosLayoutTypes[] = {
    &elosLog4cLayoutTypeElos,
};

static int elosNlayoutTypes = (int)(sizeof(elosLayoutTypes) / sizeof(elosLayoutTypes[0]));

int elosInitElosFormatter() {
    int rc = 0;
    int i = 0;

    for (i = 0; i < elosNlayoutTypes; i++) {
        log4c_layout_type_set(elosLayoutTypes[i]);
    }
    return (rc);
}

int elosInitElosLog4cLib() {
    elosInitElosFormatter();
    elosInitElosAppender();

    return (0);
}

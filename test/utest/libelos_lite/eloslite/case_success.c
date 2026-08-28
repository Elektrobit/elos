// SPDX-License-Identifier: MIT

#include <elos/event/event.h>
#include <elos/libelos_lite.h>
#include <elos/lite/event.h>
#include <string.h>

#include "eloslite_utest.h"

static const elosSeverityE_t elosliteElosSeverity = ELOS_SEVERITY_INFO;
static const char *elosliteElosCompareString = "This is a test. Param1: AA, Param2: 1234";

bool __wrap_eloslitePublish(elosliteSession_t *session, elosliteEvent_t *event) {
    assert_non_null(session);
    assert_non_null(event);

    assert_int_equal(event->severity, elosliteElosSeverity);
    assert_string_equal(event->payload, elosliteElosCompareString);

    return true;
}

void elosliteTestElosliteLogMessageFormatedSuccess(UNUSED void **state) {
    elosliteSession_t session = {0x00};

    assert_true(elosliteLogMessageFormated(&session, elosliteElosSeverity, "This is a test. Param1: %s, Param2: %d",
                                           "AA", 1234));
}

int elosliteTestElosliteLogMessageFormatedSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosliteTestElosliteLogMessageFormatedSuccessTeardown(UNUSED void **state) {
    return 0;
}

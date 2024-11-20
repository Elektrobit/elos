// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENT_SEVERITY
#define ELOS_EVENT_SEVERITY

/*******************************************************************
 * The possible severities for an :c:expr:`elosEvent_t *` can have
 ******************************************************************/
typedef enum elosSeverityE {
    /**/  // this comment is needet so that the enumerators are listet in the documentation!
    ELOS_SEVERITY_OFF = 0,
    ELOS_SEVERITY_FATAL,
    ELOS_SEVERITY_ERROR,
    ELOS_SEVERITY_WARN,
    ELOS_SEVERITY_INFO,
    ELOS_SEVERITY_DEBUG,
    ELOS_SEVERITY_VERBOSE,
} elosSeverityE_t;

#endif /* ELOS_EVENT_SEVERITY */

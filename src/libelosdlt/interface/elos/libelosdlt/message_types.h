// SPDX-License-Identifier: MIT
#pragma once

#include <stdint.h>

__BEGIN_DECLS

/*******************************************************************
 * The message types in dlt protocol
 ******************************************************************/
typedef enum elosDltMessageTypeE {
    ELOS_DLT_TYPE_LOG = 0x0,
    ELOS_DLT_TYPE_APP_TRACE = 0x1,
    ELOS_DLT_TYPE_NW_TRACE = 0x2,
    ELOS_DLT_TYPE_CONTROL = 0x3,
} elosDltMessageTypeE_t;

/*******************************************************************
 * The message sub type in dlt message is of type LOG
 ******************************************************************/
typedef enum elosDltMessageLogInfoE {
    ELOS_DLT_LOG_FATAL = 0x1,
    ELOS_DLT_LOG_ERROR = 0x2,
    ELOS_DLT_LOG_WARN = 0x3,
    ELOS_DLT_LOG_INFO = 0x4,
    ELOS_DLT_LOG_DEBUG = 0x5,
    ELOS_DLT_LOG_VERBOSE = 0x6,
} elosDltMessageLogInfoE_t;

/*******************************************************************
 * The message sub type in dlt message is of type APP TRACE
 ******************************************************************/
typedef enum elosDltMessageTraceInfoE {
    ELOS_DLT_TRACE_VARIABLE = 0x1,
    ELOS_DLT_TRACE_FUNCTION_IN = 0x2,
    ELOS_DLT_TRACE_FUNCTION_OUT = 0x3,
    ELOS_DLT_TRACE_STATE = 0x4,
    ELOS_DLT_TRACE_VFB = 0x5,
} elosDltMessageTraceInfoE_t;

/*******************************************************************
 * The message sub type in dlt message is of type NETWORK TRACE
 ******************************************************************/
typedef enum elosDltMessageNetworkInfoE {
    ELOS_DLT_NW_TRACE_IPC = 0x1,
    ELOS_DLT_NW_TRACE_CAN = 0x2,
    ELOS_DLT_NW_TRACE_FLEXRAY = 0x3,
    ELOS_DLT_NW_TRACE_MOST = 0x4,
    ELOS_DLT_NW_TRACE_ETHERNET = 0x5,
    ELOS_DLT_NW_TRACE_SOMEIP = 0x6,
} elosDltMessageNetworkInfoE_t;

/*******************************************************************
 * The message sub type in dlt message is of type CONTROL
 ******************************************************************/
typedef enum elosDltMessageControlInfoE {
    ELOS_DLT_CONTROL_REQUEST = 0x1,
    ELOS_DLT_CONTROL_RESPONSE = 0x2,
    ELOS_DLT_CONTROL_TIME = 0x3,
} elosDltMessageControlInfoE_t;

__END_DECLS

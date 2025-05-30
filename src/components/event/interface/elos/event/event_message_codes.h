// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENT_MESSAGE_CODES_H
#define ELOS_EVENT_MESSAGE_CODES_H

/*******************************************************************
 * The message Code used in an :c:type:`elosEvent_t`
 ******************************************************************/
typedef enum elosEventMessageCodeE {
    /**/  // this comment is needet so that the enumerators are listet in the documentation!
    // 0 - 9999 elos events
    ELOS_MSG_CODE_UNDEFINED = 0,
    ELOS_MSG_CODE_NEW_SUBSCRIPTION = 200,
    ELOS_MSG_CODE_REMOVED_SUBSCRIPTION = 202,
    ELOS_MSG_CODE_FAILED_TO_CREATE_SUBCRIPTION = 400,
    ELOS_MSG_CODE_FAILED_TO_REMOVE_SUBCRIPTION = 401,
    ELOS_MSG_CODE_EVENT_BLACKLIST_FAILED = 501,

    // 1000 - 1999 Informational
    ELOS_MSG_CODE_WILDCARD_1XXX = 1000,
    ELOS_MSG_CODE_DEBUG_LOG = 1101,
    ELOS_MSG_CODE_INFO_LOG = 1102,
    ELOS_MSG_CODE_TRACE_LOG = 1103,
    ELOS_MSG_CODE_KMSG = 1111,

    // 2000 - 2999 Program Status
    ELOS_MSG_CODE_WILDCARD_2XXX = 2000,
    ELOS_MSG_CODE_PROCESS_CREATED = 2001,
    ELOS_MSG_CODE_PROCESS_EXITED = 2002,
    ELOS_MSG_CODE_FILE_OPENED = 2003,
    ELOS_MSG_CODE_FILE_CLOSED = 2004,
    ELOS_MSG_CODE_LOCK_ACQUIRED = 2005,
    ELOS_MSG_CODE_LOCK_RELEASED = 2006,
    ELOS_MSG_CODE_SOCKET_OPENEND = 2007,
    ELOS_MSG_CODE_SOCKET_CLOSED = 2008,

    // 3000 - 3999 Program Resource error
    ELOS_MSG_CODE_WILDCARD_3XXX = 3000,
    ELOS_MSG_CODE_NOT_AUTHORIZED = 3001,
    ELOS_MSG_CODE_FORBIDDEN = 3003,
    ELOS_MSG_CODE_FILE_NOT_FOUND = 3004,
    ELOS_MSG_CODE_READ_ERROR = 3005,
    ELOS_MSG_CODE_WRITE_ERROR = 3006,
    ELOS_MSG_CODE_RESOURCE_LOCKED = 3023,
    ELOS_MSG_CODE_MESSAGE_NOT_UNDERSTOOD = 3422,

    // 4000 - 4999 Program IPC error
    ELOS_MSG_CODE_WILDCARD_4XXX = 4000,
    ELOS_MSG_CODE_IPC_NOT_AUTHORIZED = 4001,
    ELOS_MSG_CODE_IPC_MALFORMED_REQUEST = 4002,
    ELOS_MSG_CODE_IPC_METHOD_NOT_ALLOWED = 4005,
    ELOS_MSG_CODE_IPC_RESOURCE_ERROR = 4006,

    // 5000 - 5999 Program Execution error
    ELOS_MSG_CODE_WILDCARD_5XXX = 5000,
    ELOS_MSG_CODE_SIGSYS = 5001,
    ELOS_MSG_CODE_SIGSEGV = 5002,
    ELOS_MSG_CODE_SIGILL = 5003,
    ELOS_MSG_CODE_SIGFPE = 5004,
    ELOS_MSG_CODE_CORE_DUMPED = 5005,
    ELOS_MSG_CODE_EXIT_FAILURE = 5006,
    ELOS_MSG_CODE_OOM_KILLER_INVOKED = 5020,
    ELOS_MSG_CODE_CORE_DUMP_CREATED = 5100,
    ELOS_MSG_CODE_CORE_DUMP_DELETED = 5101,
    ELOS_MSG_CODE_CORE_DUMP_DISCARDED = 5102,

    // 6000 - 6999 Hardware Fault
    ELOS_MSG_CODE_WILDCARD_6XXX = 6000,
    ELOS_MSG_CODE_DEVICE_READ_ERROR = 6001,
    ELOS_MSG_CODE_DEVICE_WRITE_ERROR = 6002,
    ELOS_MSG_CODE_DEVICE_CRITICAL_TEMPERATURE = 6003,
    ELOS_MSG_CODE_DEVICE_CRITICAL_POWER = 6004,
    ELOS_MSG_CODE_DEVICE_SETUP_ERROR = 6005,

    // 7000 - 7999 Hardware Status Change
    ELOS_MSG_CODE_WILDCARD_7XXX = 7000,
    ELOS_MSG_CODE_DEVICE_POWER_ON = 7001,
    ELOS_MSG_CODE_DEVICE_POWER_OFF = 7002,
    ELOS_MSG_CODE_DEVICE_PLUGGED = 7003,
    ELOS_MSG_CODE_DEVICE_UNPLUGGED = 7004,
    ELOS_MSG_CODE_READY = 7005,

    // 71xx - Environmental Hardware Events
    ELOS_MSG_CODE_HEAVY_IMPACT = 7125,
    ELOS_MSG_CODE_OVER_TEMPERATURE = 7126,
    ELOS_MSG_CODE_NORMAL_TEMPERATURE = 7127,

    // 72xx - Network Hardware Events
    ELOS_MSG_CODE_NET_INTERFACE_ADDED = 7200,
    ELOS_MSG_CODE_NET_INTERFACE_REMOVED = 7201,
    ELOS_MSG_CODE_NET_INTERFACE_UP = 7202,
    ELOS_MSG_CODE_NET_INTERFACE_DOWN = 7203,
    ELOS_MSG_CODE_NET_INTERFACE_LINK = 7204,
    ELOS_MSG_CODE_NET_INTERFACE_LINK_LOST = 7205,

    // 8000 - 8999 Security Audit
    ELOS_MSG_CODE_WILDCARD_8XXX = 8000,
    ELOS_MSG_CODE_USER_ADDED = 8001,
    ELOS_MSG_CODE_USER_REMOVED = 8002,
    ELOS_MSG_CODE_PASSWORD_CHANGED = 8003,
    ELOS_MSG_CODE_LOGIN_FAIL = 8004,
    ELOS_MSG_CODE_LOGIN_OK = 8005,
    ELOS_MSG_CODE_RESOURCE_PERMISSION_CHANGED = 8006,
    ELOS_MSG_CODE_UNAUTHORIZED_PUBLISHING = 8007,
} elosEventMessageCodeE_t;
#endif

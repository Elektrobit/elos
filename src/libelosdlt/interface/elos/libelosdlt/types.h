// SPDX-License-Identifier: MIT
#pragma once

#include <stdint.h>
#include <sys/cdefs.h>
#include <time.h>

#define DLT_ID_SIZE 4

__BEGIN_DECLS

/*******************************************************************
 * The header fragment of an DLT message.
 *
 * Note: This assumes to always use the extended header
 *
 * Members:
 *     headerTyp: Bit field to define the anatomy of the header
 *     messageCount: sequence number of messages
 *     length: overall length of a message starting and including this header
 *     ecuId: used as ECU id in DLT log
 *     sessionId: the process id of the process sending this messages
 *     monotonicTimestamp: time in nano sec since system start
 *     messageInfo: Bit field to define kind of a message (Request/Response,
 *                  message type, verbose)
 *     argCount: Number of arguments attached to this message
 *     appId: used as application id in DLT log
 *     contextId: used as context id in DLT log
 ******************************************************************/
typedef struct elosDltMessage {
    struct {
        uint8_t useExtendedHeader : 1;
        uint8_t msbFirst : 1;
        uint8_t withEcuId : 1;
        uint8_t withSessionID : 1;
        uint8_t withTimestamp : 1;
        uint8_t protocolVersion1 : 1;
        uint8_t reserved : 2;
    } headerTyp;
    uint8_t messageCount;
    uint16_t length;
    char ecuId[DLT_ID_SIZE];
    uint32_t sessionId;
    uint32_t monotonicTimestamp;
    uint8_t messageInfo;
    uint8_t argCount;
    char appId[DLT_ID_SIZE];
    char contextId[DLT_ID_SIZE];
} __attribute__((aligned(1), packed)) elosDltMessage_t;

/*******************************************************************
 * Types of DLT user messages currently supported.
 *
 ******************************************************************/
typedef enum elosDltUserMessageTypeE {
    ELOS_USER_MESSAGE_TYPE_UNDEFINED = 0,
    ELOS_USER_MESSAGE_TYPE_LOG = 1,
    ELOS_USER_MESSAGE_TYPE_REGISTER_APPLICATION = 2,
    ELOS_USER_MESSAGE_TYPE_UNREGISTER_APPLICATION = 3,
    ELOS_USER_MESSAGE_TYPE_REGISTER_CONTEXT = 4,
    ELOS_USER_MESSAGE_TYPE_UNREGISTER_CONTEXT = 5,
} elosDltUserMessageTypeE_t;

/*******************************************************************
 * The protocol fragment introducing a DLT control message.
 *
 * Note: only supported for connection types TCP and UNIX socket?
 *
 * Members:
 *     serviceID: define the remote service to call
 ******************************************************************/
typedef struct elosDltControlMessage {
    uint32_t serviceID;
    uint32_t length;
} __attribute__((aligned(1), packed)) elosDltControlMessage_t;

/*******************************************************************
 * The protocol fragment introducing a DLT user message.
 *
 * Note: only supported for connection type pipe?
 *
 * Members:
 *     magicNumber: magic identifier to determine message start, shall be
 *                 `['D', 'U', 'H', 0x01]`
 *     type: the message type which follows.
 ******************************************************************/
typedef struct elosDltUserMessage {
    char magicNumber[DLT_ID_SIZE];
    elosDltUserMessageTypeE_t type;
} __attribute__((aligned(1), packed)) elosDltUserMessage_t;

/*******************************************************************
 * A DLT protocol fragment to define an serialized value to be logged
 *
 * Members:
 *     typeInfo: define the type of value
 *     argSize: define the size of the value in bytes
 *     nameSize: the size of the name string for this value
 *     name: the value name or key.
 ******************************************************************/
typedef struct elosDltLogArgument {
    uint32_t typeInfo;
    uint16_t argSize;
    uint16_t nameSize;
    char name[20];
} __attribute__((aligned(1), packed)) elosDltLogArgument_t;

/*******************************************************************
 * A DLT message to register a new application context.
 *
 * Members:
 *     header: The user message header
 *     appId: used as new application id to be register
 *     pid: the process id of the process sending this messages
 *     length: the length of the application description.
 ******************************************************************/
typedef struct elosDltgRegisterApplication_t {
    elosDltUserMessage_t header;
    char appId[DLT_ID_SIZE];
    int32_t pid;
    uint32_t length;
} __attribute__((aligned(1), packed)) elosDltRegisterApplication_t;

/*******************************************************************
 * A DLT message to unregister an existing application context.
 *
 * Members:
 *     header: The user message header
 *     appId: used as application id to be unregistered
 *     pid: the process id of the process sending this messages
 ******************************************************************/
typedef struct elosDltgUnregisterApplication_t {
    elosDltUserMessage_t header;
    char appId[DLT_ID_SIZE];
    int32_t pid;
} __attribute__((aligned(1), packed)) elosDltUnregisterApplication_t;

/*******************************************************************
 * A DLT message to register a context for an existing application context.
 *
 * Members:
 *     header: The user message header
 *     appId: used as application id to register a new context for
 *     contextId: used as new context id to be registered
 *     logLevelPosition: purpose currently unknown and unused for now
 *     logLevel: initial log level for this context
 *     traceStatus: set to 0; purpose currently unknown and unused
 *     pid: the process id of the process sending this messages
 *     length: the length of the context description.
 ******************************************************************/
typedef struct elosDltgRegisterContext {
    elosDltUserMessage_t header;
    char appId[DLT_ID_SIZE];
    char contextId[DLT_ID_SIZE];
    int32_t logLevelPosition;
    int8_t logLevel;
    int8_t traceStatus;
    int32_t pid;
    uint32_t length;
} __attribute__((aligned(1), packed)) elosDltRegisterContext_t;

/*******************************************************************
 * A DLT message to unregister a context for an existing application context.
 *
 * Members:
 *     header: The user message header
 *     appId: used as application id to unregister a new context for
 *     contextId: used as new context id to be unregistered
 *     pid: the process id of the process sending this messages
 ******************************************************************/
typedef struct elosDltgUnregisterContext {
    elosDltUserMessage_t header;
    char appId[DLT_ID_SIZE];
    char contextId[DLT_ID_SIZE];
    int32_t pid;
} __attribute__((aligned(1), packed)) elosDltUnregisterContext_t;

/*******************************************************************
 * Parameter struct used to configure a new DLT connection.
 *
 * Members:
 *     socketPath: path to unix domain socket of DLT-Daemon
 *     host: IP address or name of DLT-Daemon
 *     pipePath: path to pipe of DLT-Daemon
 *     port: port the DLT-Daemon to connect to
 *     socketFd: file descriptor of active connection or -1 if not connected
 *     messageCount: count of messages transmitted by this connection
 *     ecuId: used as ECU id in DLT log
 *     appId: used as application id in DLT log
 *     contextId: used as context id in DLT log
 ******************************************************************/
typedef struct elosDltConnection {
    char *socketPath;
    char *host;
    char *pipePath;
    int port;
    int socketFd;
    size_t messageCount;
    char ecuId[DLT_ID_SIZE];
    char appId[DLT_ID_SIZE];
    char contextId[DLT_ID_SIZE];
} elosDltConnection_t;

/*******************************************************************
 * Parameter struct used to configure a new DLT connection.
 *
 * Members:
 *     ecuId: used as ECU id in DLT log
 *     appId: used as application id in DLT log
 *     contextId: used as context id in DLT log
 ******************************************************************/
typedef struct elosDltConnectionParam {
    const char *ecuId;
    const char *contextId;
    const char *appId;
} elosDltConnectionParam_t;

__END_DECLS

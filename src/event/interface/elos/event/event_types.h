// SPDX-License-Identifier: MIT
#pragma once

#include <safu/vector_types.h>
#include <stdint.h>
#include <time.h>

#include "elos/event/event_message_codes.h"

#define ELOS_CLASSIFICATION_ELOS_MASK      0x00000000FFFFFFFFULL
#define ELOS_CLASSIFICATION_USER_MASK      0x000000FF00000000ULL
#define ELOS_CLASSIFICATION_RESERVED_MASK  0xFFFFFF0000000000ULL
#define ELOS_CLASSIFICATION_UNDEFINED      0x0000000000000000ULL
#define ELOS_CLASSIFICATION_KERNEL         0x0000000000000001ULL
#define ELOS_CLASSIFICATION_NETWORK        0x0000000000000002ULL
#define ELOS_CLASSIFICATION_SECURITY       0x0000000000000004ULL
#define ELOS_CLASSIFICATION_POWER          0x0000000000000008ULL
#define ELOS_CLASSIFICATION_STORAGE        0x0000000000000010ULL
#define ELOS_CLASSIFICATION_PROCESS        0x0000000000000020ULL
#define ELOS_CLASSIFICATION_IPC            0x0000000000000040ULL
#define ELOS_CLASSIFICATION_HARDWARE       0x0000000000000080ULL
#define ELOS_CLASSIFICATION_ELOS           0x0000000000000100ULL
#define ELOS_CLASSIFICATION_PROCESS_ERRORS 0x0000000000000200ULL
#define ELOS_CLASSIFICATION_USER_0         0x0000000100000000ULL
#define ELOS_CLASSIFICATION_USER_1         0x0000000200000000ULL
#define ELOS_CLASSIFICATION_USER_2         0x0000000400000000ULL
#define ELOS_CLASSIFICATION_USER_3         0x0000000800000000ULL
#define ELOS_CLASSIFICATION_USER_4         0x0000001000000000ULL
#define ELOS_CLASSIFICATION_USER_5         0x0000002000000000ULL
#define ELOS_CLASSIFICATION_USER_6         0x0000004000000000ULL
#define ELOS_CLASSIFICATION_USER_7         0x0000008000000000ULL

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

/*******************************************************************
 * Source of an :c:type:`elosEvent_t`
 *
 * Members:
 *      appName: a string containing the name of the application the event originated from
 *      fileName: a string containing the filename of the application the event originated from
 *      pid: an integeter containing the process id of the application the event originated from
 ******************************************************************/
typedef struct elosEventSource {
    char *appName;
    char *fileName;
    pid_t pid;
} elosEventSource_t;

/*******************************************************************
 * Representation of an elos event
 *
 * Members:
 *      date: unix timestamp in nano seconds resolution.
 *      source: struct containing informations about where the event originated from
 *      severity: when the message gets classified it is mapped to a severity value defined by this project.
 *      hardwareid: an unique identifier for the hardware which produced this particular information.
 *      classification:
 *          The classification attribute is used to categorize events
 *          to be related to a certain context of system information.
 *          The classification value is a 64 unsigned integer and is splitted into 3 sections.
 *
 *          - bit 1 to 32  (0x00000000FFFFFFFF) reserved by elos (never change their meaning)
 *          - bit 33 to 41 (0x000000FF00000000) can be defined and used by the user
 *            (something like User_1-User_7)
 *          - bit 42 to 64 (0xFFFFFF0000000000) reserved for future use
 *
 *          The following table describes the lower 32 bit classification bit mask and their meaning:
 *
 *          ==================  ==============  ===================================================================
 *                Value         Name            Description
 *          ==================  ==============  ===================================================================
 *          0x0000000000000000  undefined       used to indicate not available classification information
 *          0x0000000000000001  Kernel          all events which are related to some kernel functionality
 *          0x0000000000000002  Network         all network related information
 *          0x0000000000000004  Security        all events that are security related
 *          0x0000000000000008  Power           all events that is relevant for power management
 *          0x0000000000000010  Storage         all events which are related to filesystem and non volatile storage
 *          0x0000000000000020  Process         all events about the livecycle of a process
 *          0x0000000000000040  IPC             all events about IPC
 *          0x0000000000000080  Hardware        all events about hardware
 *          0x0000000000000100  elos            all events about elos internals
 *          0x0000000000000200  Process Errors  all events about faulty or misbehaving processes
 *          ==================  ==============  ===================================================================
 *
 *          An event can have multiple classification flags, for example a chassis
 *          intrusion detection event can be classified as `Security|Hardware`.
 *
 *      messageCode: used to hide information (from security perspective),
 *          thus an information has a meaning without providing a payload (text).
 *
 *      payload: the actual payload of the information (text, number, structs)).
 ******************************************************************/
typedef struct elosEvent {
    struct timespec date;
    elosEventSource_t source;
    elosSeverityE_t severity;
    char *hardwareid;
    uint64_t classification;
    elosEventMessageCodeE_t messageCode;
    char *payload;
} elosEvent_t;

/*******************************************************************
 * typedef :c:type:`safuVec_t` to :c:type:`elosEventVector_t`.
 ******************************************************************/
typedef safuVec_t elosEventVector_t;

// SPDX-License-Identifier: MIT
#pragma once

#include <safu/vector_types.h>
#include <stdint.h>
#include <time.h>

#include "elos/event/event_classification.h"
#include "elos/event/event_message_codes.h"
#include "elos/event/event_severity.h"

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

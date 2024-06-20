Syslog – Scanner
================

Serves as a syslog daemon and converts syslog lines into the canonical
event format.

.. figure:: /doc/images/scanner_syslog_component.png
   :alt: Syslog scanner component diagram

   Syslog scanner

The format of syslog lines is defined in \* `The Syslog
Protocol <https://datatracker.ietf.org/doc/rfc5424/>`__ (most recent) \*
`The BSD syslog
Protocol <https://datatracker.ietf.org/doc/html/rfc3164>`__ (obsolet but
used by libc)

The current implementation can parse syslog lines according to the
schema produced by the glibc ``syslog`` function. (see `“%h %e
%T” <https://sourceware.org/git/?p=glibc.git;a=blob;f=misc/syslog.c;h=554089bfc45244e87c08eb0482e61c35dd0fa3ca;hb=HEAD>`__)
The syslog socket is configured as an unix domain socket with UDP and
listen on ``/dev/log``.

Configuration
-------------

Compile time:
~~~~~~~~~~~~~

-  ``ELOSD_SYSLOG_PATH`` –> the path where to create a unix UDP socket
   to listen on, default is ``/dev/log``

Environment:
~~~~~~~~~~~~

-  ``ELOS_SYSLOG_PATH`` –> the path where to create a unix UDP socket to
   listen on, default is ``ELOSD_SYSLOG_PATH``

Configuration structure
~~~~~~~~~~~~~~~~~~~~~~~

The mapping rules are provided through the configuration. The
configuration (samconf) allows to lookup single options by a path like
notation. So the syslog scanner expect the following config structure:

.. code:: bash

   SyslogScanner
   ├── File
   ├── Run
   └── Config
       ├── SyslogPath
       ├── MappingRules
       │   ├── MessageCode
       │   │   ├── 4000
       │   │   ├── 4001
       │   │   └── 2001
       │   └── ... (other like Severity, classification)
       └── FilterRules (Not implemented)
           ├── empty payload
           ├── all from appName X
           └── containing secrets

The implementation for configuration files is still in progress so the
format for configuration files is not defined now.

Published events
----------------

-  format is:

.. code:: json

   [
     {
       "date": [
         284,
         0
       ],
       "source": {
         "appName": "sshd",
         "pid": 100
       },
       "severity": 3,
       "hardwareid": "ba9202e00b98474a892ca45f1bfb3624",
       "classification": 4,
       "payload": ": error: kex_exchange_identification: client sent invalid protocol identifier \"uhuhuhuhuh\""
     },
     {
       "date": [
         284,
         0
       ],
       "source": {
         "appName": "sshd",
         "pid": 100
       },
       "severity": 3,
       "hardwareid": "ba9202e00b98474a892ca45f1bfb3624",
       "classification": 4,
       "payload": ": error: send_error: write: Broken pipe"
     },
     {
       "date": [
         284,
         0
       ],
       "source": {
         "appName": "sshd",
         "pid": 100
       },
       "severity": 6,
       "hardwareid": "ba9202e00b98474a892ca45f1bfb3624",
       "classification": 4,
       "payload": ": banner exchange: Connection from 192.168.7.1 port 33720: invalid format"
     }
   ]

Implementation details
----------------------

The syslog scanner shall preprocess an incoming syslog line according to
the expected standard.

-  `The Syslog Protocol <https://datatracker.ietf.org/doc/rfc5424/>`__
   (most recent)
-  `The BSD syslog
   Protocol <https://datatracker.ietf.org/doc/html/rfc3164>`__ (obsolete
   but used by libc)
-  libc - syslog implementation of the target system

After the preprocessing the event have at least filled the following
fields:

-  date
-  severity
-  classification
-  source (appName, pid)
-  payload

The prepared event is then provided to all mapping rules found in the
configuration for
``/elos/Scanner/SyslogScanner/MappingRules/MessageCode/<appName>``. The
key of the first rule that matches will be applied as messageCode and
the event will be dispatched as intended.

If no rule matches we have to decide :

ADR - How to handle events without messageCode mapping
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The problem to solve is, how to deal with events for which elos was
unable to determine a messageCode.

Without a messageCode the event is probably not interpretable or
meaningful for a machine without parsing the payload.

*A) Use default mapping for messageCode*

If no messageCode mapping rule applies, the event can be tagged as
``Informational`` (1000) or more specific with 1102 (Information). This
way an event will not get lost, the events can still be received with by
a subscription to the source identifier and/or severity and
classification. The messageCode indicates to the subscriber that elos
was unable to map the event to a specific messageCode and therefor to a
meaning. So it is left to the subscriber if he either tries to do some
interpretation of the payload by himself or to simply ignore the event.

No event will be lost in this scenario. But even broken messages or for
some special reason not mapped events will always be published.

*B) Drop event*

In case a messageCode can’t determined by the mapping rules the event
will be dropped. This means the event is lost and will not be further
processed, no logging and no publishing occurs.

*C) Drop event and create a ``drop event``*

An option to trace this behavior is to generate specific “Drop”-events.
This event shall have a messageCode for the elos reserved events (0-999)
contain the original source and payload of the dropped event. If
available also the severity and classification of the dropped event.

*D) omit the messageCode*

It could also be possible through the nature of the event structure,
that the messageCode is simply omitted. A messageCode equals to zero has
currently the meaning of “not provided”. So omitting the messageCode
could have a clear meaning to the subscribers, “elos can’t provide a
messageCode for this event, use it or leave it”.

This way such events are not lost and the subscriber has to decide on
their own if they can deal with an event thats missing a messageCode.

Conclusion/Decision
^^^^^^^^^^^^^^^^^^^

severity - Mapping
~~~~~~~~~~~~~~~~~~

The severity for the syslog protocol is defined as follows:

===== ========================================
Value Description
===== ========================================
0     Emergency: system is unusable
1     Alert: action must be taken immediately
2     Critical: critical conditions
3     Error: error conditions
4     Warning: warning conditions
5     Notice: normal but significant condition
6     Informational: informational messages
7     Debug: debug-level messages
===== ========================================

The severity value can be obtained from the PRI value by calculating
``pri % 8``.

The following table shows the mapping from syslog-severity to
elos-severity values.

+------------+---------------------------------------------------------+
| Syslo      | elos-severity                                           |
| g-severity |                                                         |
+============+=========================================================+
| 0          | Fatal = 0x01 Fatal error, not recoverable.              |
+------------+---------------------------------------------------------+
| 1          | Error = 0x02 Error with impact to correct               |
|            | functionality.                                          |
+------------+---------------------------------------------------------+
| 2          | Error = 0x02 Error with impact to correct               |
|            | functionality.                                          |
+------------+---------------------------------------------------------+
| 3          | Warn = 0x03 Warning if correct behavior cannot be       |
|            | ensured.                                                |
+------------+---------------------------------------------------------+
| 4          | Warn = 0x03 Warning if correct behavior cannot be       |
|            | ensured.                                                |
+------------+---------------------------------------------------------+
| 5          | Info = 0x04 Informational, providing high level         |
|            | understanding.                                          |
+------------+---------------------------------------------------------+
| 6          | Info = 0x04 Informational, providing high level         |
|            | understanding.                                          |
+------------+---------------------------------------------------------+
| 7          | Debug = 0x05 Detailed information for programmers.      |
+------------+---------------------------------------------------------+
| -          | Verbose = 0x06 Extra-verbose debug messages (highest    |
|            | grade of information)                                   |
+------------+---------------------------------------------------------+
| -          | Off = 0x00 No logging                                   |
+------------+---------------------------------------------------------+

classification - Mapping
~~~~~~~~~~~~~~~~~~~~~~~~

The syslog protocol defines a value to group the log messages by
facility. The following table list them and the corresponding elos
classification value.

+-----------------+------------------------+------------------------+
| Syslog-facility | Description            | elos classifaction     |
|                 |                        | (TBD)                  |
+=================+========================+========================+
| 0               | kernel messages        | 0x0000000000000001     |
+-----------------+------------------------+------------------------+
| 1               | user-level messages    | 0x0000000000000000     |
+-----------------+------------------------+------------------------+
| 2               | mail system            | 0x0000000000000002     |
+-----------------+------------------------+------------------------+
| 3               | system daemons         | 0x0000000000000020     |
+-----------------+------------------------+------------------------+
| 4               | security/authorization | 0x0000000000000004     |
|                 | messages               |                        |
+-----------------+------------------------+------------------------+
| 5               | messages generated     | 0x0000000000000000     |
|                 | internally by syslogd  |                        |
+-----------------+------------------------+------------------------+
| 6               | line printer subsystem | 0x0000000000000000     |
+-----------------+------------------------+------------------------+
| 7               | network news subsystem | 0x0000000000000001     |
+-----------------+------------------------+------------------------+
| 8               | UUCP subsystem         | 0x0000000000000042     |
+-----------------+------------------------+------------------------+
| 9               | clock daemon           | 0x0000000000000000     |
+-----------------+------------------------+------------------------+
| 10              | security/authorization | 0x0000000000000004     |
|                 | messages               |                        |
+-----------------+------------------------+------------------------+
| 11              | FTP daemon             | 0x0000000000000002     |
+-----------------+------------------------+------------------------+
| 12              | NTP subsystem          | 0x0000000000000002     |
+-----------------+------------------------+------------------------+
| 13              | log audit              | 0x0000000000000004     |
+-----------------+------------------------+------------------------+
| 14              | log alert              | 0x0000000000000000     |
+-----------------+------------------------+------------------------+
| 15              | clock daemon (note 2)  | 0x0000000000000000     |
+-----------------+------------------------+------------------------+
| 16              | local use 0 (local0)   | 0x0000000100000000     |
+-----------------+------------------------+------------------------+
| 17              | local use 1 (local1)   | 0x0000000200000000     |
+-----------------+------------------------+------------------------+
| 18              | local use 2 (local2)   | 0x0000000400000000     |
+-----------------+------------------------+------------------------+
| 19              | local use 3 (local3)   | 0x0000000800000000     |
+-----------------+------------------------+------------------------+
| 20              | local use 4 (local4)   | 0x0000001000000000     |
+-----------------+------------------------+------------------------+
| 21              | local use 5 (local5)   | 0x0000002000000000     |
+-----------------+------------------------+------------------------+
| 22              | local use 6 (local6)   | 0x0000004000000000     |
+-----------------+------------------------+------------------------+
| 23              | local use 7 (local7)   | 0x0000008000000000     |
+-----------------+------------------------+------------------------+

date - Mapping
~~~~~~~~~~~~~~

In older versions of the syslog protocol the date field doesn’t provide
a year nor a timezone. The same is the case for the current development
environment. To solve this the syslog scanner assumes always the UTC
timezone and applies the current year to the date from the syslogline.

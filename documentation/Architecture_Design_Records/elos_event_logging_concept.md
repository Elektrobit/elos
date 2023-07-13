# Architecture Design Record - elos event logging concept **DRAFT**

## Problem

Need to establish an unified method of logging events instead of messages.
We want to propagate logging of events which can be identified by
`messageCodes`. Conventional logging like

```
 logErr("Failed to open File /x/y/z")
 logDebug("open File /x/y/z")
 logInfo("file sucessfully opened")
 logWarn("unauthorized message publishing attempt")
```
relies on encoding the information into a human readable string. To convert
such event into events like we already do for syslog and kmsg, log lines must
be parsed and mapped again.

At the moment elos itself relies for logging on this pattern with a basic
logging facility located in libsafu/log.h. The core concept of this pattern is
to categorize log messages into severity groups and encode the `event` in a
custom string.

We want to log events occurring in elos also as elos-events. Therefore we need a
logging facility, which makes it convenient to encode classical logging as an
elos-event. For example instead of writing:

```C
  int fd = open("/my/file",O_RDWR,0);
  if (fd != -1) {
     // operate on file
  } else {
     perror("Failed to open file");
     fprintf(stderr, "Failed to open file");
     customLoggerErr("Failed to open file");
     exit(EXIT_FAILURE);
  }
```

It shall be directly converted in an elos-event and preserving all contextual
information machine readable:
```C
  int fd = open("/my/file",O_RDWR,0);
  if (fd != -1) {
     // operate on file
  } else {
     elosLog(ELOS_MSG_CODE_FILE_NOT_FOUND, ELOS_SEVERITY_FATAL, "/my/file");
     exit(EXIT_FAILURE);
  }
```

While a function like above to publish logs as elos eventis is quite easy
implemented, there are several sever issues which should be discussed before.

1. How to proceed if publishing the elos-event failed, what to use as fallback
   (i.e. stderr)?
2. How to avoid endless logging cycles ? 
```C
    void elosLog(ELOS_MSG_CODE_FILE_NOT_FOUND, ELOS_SEVERITY_FATAL, "/my/file"){
        if(elosPublishEvent(...) == FAILED ) {
            elosLog(ELOS_MSG_CODE_FAILED_TO_PUBLISH_EVENT, orignalEvent.toString)
        }
    }
```
3. How should the API look like to be convenient and enforce all necessary
   information to generate meaningful events.
4. Should be every thing logged as event , is every log string an event ?
5. How to implement the elos-logging API for elosd and for elos clients? What can be shared ?


## Influencing factors

* Event logging must be fast as possible, to reduce influence on the runtime
  behavior of the calling code
* The fall back shall not introduce a new log storage
* Elsod itself shall be able to directly publish the events internally via a
  callback and do not a detour over the client interface.
* For development or in DEBUG build each event log shall be  available as text log somehow

## Assumptions

## Considered Alternatives



## Decision

### Rationale

### Open Points

# ADR - Feature set of elos "fetch"  **DRAFT**


## Problem

The elos IPC protocol has a powerful feature called "fetch" to grab old events from the storages.
It can be used via elosc and argument "-f":
```
elosc -f "1 1 EQ"
elosc -f ".event.source.appName 'sshd' STRCMP .event.payload r'Accept' REGEX AND"
```

This is very handy if someone might have missed an important event that happened in the past.
This is also useful to readout old events from the storages similar to reading a log-file.

(Hint: backend in the following always means storage-backends)

## Influencing factors

The following constraints have to be taken into account for a suitable solution:
* efforts of development
* meaningful feature set
* efficient execution time

## Assumption

When using the fetch backend the user expects to just get a set of recent events that it might have missed.
e.g.: If "A" waits for "sshd" to start, but "sshd" was already started just milliseconds before "A", the fetch-feature is very handy to avoid waiting of "A" for an event that has already occurred.

It is assumed that a full log-analysis like auditing etc. is done using tools specifically designed for the storage. e.g. SQL querys are used to analyse logs written to SQL databases.


## Considered Alternatives

### 1) Option - full featured "fetch"

fetch takes all storage backends (json, sql, nosql, ...) into account and collects events from there

*pros*
* the returned events are almost complete

*cons*
* elos needs support read-data from the backends
* elos needs to convert RPN to data-selection-fromat of backend (e.g. RPN to SQL)
* event duplication might occur if one event is stored in multiple backends.
* if events were stored to no backend at all, they will not show-up.

### 2) Option - limited "fetch" only on selected backend (e.g. JSON)

During fetch elos just reads from selected backends and thus returns only a subset of the events. In particular the subset of events that were stored in the backend.

e.g.: Just the json based backends are queried and only the currently used file and its predecessor are read-in.

*pros*
* no read support of all backends needed
* with a well configured rotation all relevant events can be collected.

*cons*
* elos returns incomplete events (only recent ones)
* depending on file rotation the amount of events varies (e.g. if the files were rotated very recently only the events from the predecessor file are used, if the files are about to be rotated soon two full files of events are used.)

### 3) Option - "fetch" only from in-memory database

An in-memory (RAM) database just stored some events in a ringbuffer-style. Old events are discarded if new events arrive and the ring is already full.

This in-memory buffer could be statically sized in Bytes (e.g store only 10MB of events) or dynamically (e.g. store last 10sec of events)

Option 2 is pretty similar to this, if the JSON file is stored in a ram-disk.

*pros*
* no read support on any backend needed

*cons*
* elos returns only events still in the in-memory database.
* read of stored events needs to interface directly to storage. (e.g. read from the SQL database)

### 4) Option - no "fetch" at all

elos only forwards events from the inputs. Old events are lost and gone.

*pros*
* reduces elos to its basics (KISS)

*cons*
* the feature is already there and became very handy.

## Decision

(none yet)

### Rationale

### Open Points

if something was left open after this decision

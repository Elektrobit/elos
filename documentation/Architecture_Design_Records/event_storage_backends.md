# Architecture Design Record - Event Storage Backend

## Problem

An event storage backend for elos has the task to persist and retrieve events
utilizing techniques to fulfill a set of characteristics required to store and retrieve one
or more classes of events ([event storage classes](event_storage_class.md)).



## Influencing factors

* retention policy
* assurance about write integrity
* assurance about data integrity
* write speed
* read speed
* flash usage per event persistence operation
* space requirements per event (compression)

The task is now to define possible event storage backend implementation and
define there characteristics. These could then be used to choose an
implementation which fits best to store a given class of events.


## Assumptions

## Considered Alternatives

### 1) RDBMS - SQLite

SQLite is not a conventional RDBMS as it comes as a library and not as classical client-/server based approach. SQLite targets embedded systems and intended to provide an commonly used and known SQL-Interface to manage data.

*TBD: evaluation, measurements, PoC*

*pros*
* optimized for embedded system by design
* extension API to add custom driver to store data

*cons*
* probably less effective on big complex data sets as common full featured RDBMSs
    * But: Is it intentional to manage data quantities and complex data structures
   that a classical RDBMS representative should be considered

### 2) NoSQL – MongoDB

MongoDB is a representative of the document orientated NoSQL-Databases. Each
event can be considered as a document in in the NoSql context. NoSql databases
are designed to search for through and for particular attributes of documents.

*TBD: evaluation, measurements, PoC*

*pros*
* simplicity, straight forward take the event and store it without further processing

*cons*
* needs a mongoDB server which comes with additional dependencies like python

### 2) Custom File Storage – Json File

To address the special requirements on storing events a sequential approach to
store events serialized as newline Json separated strings is possible.

To reduce the writes techniques like preallocating a file on the backing
filesystem and storage is used. To address the atomic write dependency specific
flags for writing like O_SYNC and O_DSYNC can be used. More details on this
approach can be obtained from the corresponding design decision.

*TBD: evaluation, measurements, PoC*

*pros*
* simplicity, straight forward take the event and store it without further processing
* an implementation from scratch can be highly customized to the specific needs
  and abilities of the used target system

*cons*
* probably high development effort
* danger of reinventing some other stream or file storage system over time, as more and more "lessons learned"


### 3) systemd like storage of logs

https://systemd.io/JOURNAL_FILE_FORMAT/
https://github.com/systemd/systemd
https://www.freedesktop.org/software/systemd/man/sd-journal.html

systemds journald subsystem is a logging system not too different from syslog.
It is, effectively, a block-based protocol, writing its logs to a socket.


## Decision

Systemds journald will not be used.
If the decision is reached to implement a completly new logging mechanism,
the data storage format from journald is a good reference on how to write
a logging format that is easily searchable.

### Rationale

The API of journald does not support writing to a custom file/location,
which means that we can not simply use the API for logging.
It is possible to change the location of the logging directory by setting an
enviroment variable and giving it to the journald server.
However, Using the journald server requires to start all of systemd.
From investigations it seems no functions related to the journald server are
available for other programs via a shared library.
Additionally, due to systemds design, it seems unlikely that a separate
journald sever would run without the rest of systemd available on the machine.

Furthermore, it is unsure if using the journald protocol would satisfy our
requirements for a logging protocol.
According to the official documentation, it is to be assumed, that we need to
write at least 3 block when creating an entry.
One block for the header that needs updating, one block to update the entry
array element which will contain the new entry, and one for the entry.
When the current entry array is full, we might only need to write two blocks,
since the entry array struct and the entry itself should fit into a single block.
Additionally, sometimers a tag struct will be written for corruption protection,
but this can fit into the same block as the entry as well.
S the best case scenario is two block for a single entry write, and worst case
is 4.
While an new log entry is not necessarily written to the disc instantly, current
code research indicates that every write does schedule a sync with the disc.
This means that multiple log entries can pile up before the sync actually
occures. This would reduce unnecessary the amount of times the file and list
headers need to be updated. If the amount of log entries pilung up is
sufficiently large, the overhead from those header writes would become relativly
small.

The focus of the protocol is corruption optimisation, to ensure that as little
data is corrupted and as much of the data is still useable after a corruption
is detected. To achieve this, every read checks for data consistency while
reading, as well as writing tags after an amount of entries. The first
protection mechansims is highly dependent on the amount of actual reads that
happen.
The other focus of the protocol is to make it easily searchable, with having an
search efficiency of O(n) in the worst case for n total entries, even when
searching by multiple parameters.

The compatibility between the protocols data storage and our even storage is
rather good. The format stores the date, as well as a "priority" data field,
which we can sue to store our data and severity data. Additionally, the protocol
does not have strong requirements for the name of its data fields, meaning we
can store the rest of our event data fields in plain text, with an appropriate
encoding of our field names. Combining that with the efficient search with
field names as search parameters would make lookup pretty efficient.


### Open Points
It is unclear if, should we be able to create a shared library for the journald
server, how much of systemds other sources we would need to install as well to
enable the server to run.
It was not possible for our engineers to create a shared library from those.
This does not necessarily mean that a more skilled engineer could not make that
possible.
It is unclear how long the time between a sync scheduling and the actual disc
sync is and how many logs would accumulate in that time.
It is unclear how good the corruption protection would work for elos, depending
on how many lookups actually happen.

### 4) Apache Avro Storage of logs

https://avro.apache.org/docs/1.11.1/api/c/

Avro supports storing of binary data in an easy way.

# Decision

Creating a code poc is necessary to determine how the api performs in regards
to writing blocks.
During the creation of the poc, further development was halted and avro was
abandoned as a possible logging backend.

## Rationale

It is certain that we can store an event fully in the data structures available
from Avro.

During the development of the poc, an issue with the locally available avro
dev library was found, in which the software contained a bug, making the library
unable to open a file it previously created. This made it impossible to reopen
a file that was previously written to, which makes closing the file during
operation impossible and would require a new log file after each start of the
application. Additionally, and more importantly, it is impossible to open old
log files for reading.

Trying to build avro locally in order to patch it by ourself proved difficult as
as well, due to the amount of dependencies. Some dependencies are not available
in the necessary version locally, which would require building them as well.

When trying to build avro locally, while supplying the necessary dependencies,
The build failed to varying reasons, even with the same setup.

## Open Points

The amount of actual writes that happen when storing an event is unclear,
but at least from the poc development, it seems reasonable to assume that it
is possible to cache multiple events before actually writing them to file.

### 5) Time-Series Databases

As a representative for Time-Series Databases, InfluxDb was chosen.

https://www.influxdata.com/products/influxdb-overview/.

# Decision

Creating a code poc is necessary to determine how the api performs in regards.
Due to the unavailability of InfluxDBv2 for yocto, the poc was implemented
against the API of InfluxDB  in version 1.8. The code does work with version 2
as well, since version two is backwards compatible with the version 1 API.

As of development of this ADR, the version 3 of InfluxDB was already released,
but storing was only possible in an amazon cloud, which is incompatible with
the local storing we need for elos.

Further development has not been decided as of yet.

# Rational

It is confirmed that we can store an elos event to an InfluxDb table and
read it again.

Preliminary performance tests have given slight indication that InfluxDb might
performance worse then Json or SQL, but currently the testing system performs
not reliably enough to make certain statements.

## Open Points
Version 2 of InfluxDb uses a different storage format. The assumption
is, that it could perform better in writes then the previous Storage formats.

It is also unclear how the write performance changes should we decide to cache
events and write multiple at once, which is easily possible with the InfluxDb
API, in both versions.

Lastly it is unclear how the size of the payload would influence the write
performance, since time-series databases usually have small-sized variable data,
while elosd's event payload can easily get rather big, depending on the message
that is logged.

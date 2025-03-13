# Design Decisions - Ensuring that the kmsg scanner stores all messages once

## Problem

The kmsg scanner should be an replacement for dmesg, therefore it should not just publish new messages but
all messages in the kmsg buffer. But it should be ensured to publish events only once even on a restart of
elosd.

## Influencing factors

The following constraints have to be taken into account for a suitable solution:
* elosd could get restarted
* kmsg events could appear between elosd stop and the restart


## Assumptions

The following assumptions have been made in the decision process:
* The kmsg buffer only contains messages from this boot cycle
* /run gets reset on reboot (see https://refspecs.linuxfoundation.org/FHS_3.0/fhs/ch03s15.html)


## Considered Alternatives

### 1) Publish the whole buffer on each start of elosd

Read the whole kmsg buffer on start and publish and store all events.
Continue with publishing every new event.

*pros*
* simple to implement
* no managing of state

*cons*
* Event duplication on every elosd restart
  + pollution of log storage
  + unnecessary traffic and storage usage


### 2) Publish buffer backlog only once per boot

Check if /run/elosd/kmsg.state exists and only if it doesn't continue.
Read the whole kmsg buffer on start and publish and store all events.
Then write a state file to /run/elosd/kmsg.state.
If the File already exists, assume a elosd restart and publish and store only new incoming events.


*pros*
* simple to implement
* no duplication of events

*cons*
* events occurring between elosd stop and restart would not be published or stored


### 3) record each kmsg event into the state file when it is stored

When a kmsg event is published and stored also record it in the state file (/run/elosd/kmsg.state)
On elosd startup check for /run/elosd/kmsg.state and publish and store every event after the line recorded in there.
Or everything in the kmsg buffer if the state file doesn't exist jet.

*pros*
* it is ensured that every event gets published and stored

*cons*
* more complicated to implement
* more runtime overhead
  + the events need to be recorded into the state file
  + *But*: That overhead can be somewhat mitigated by using a memory mapped file


### 4) record the last kmsg event into the state file at plugin shutdown

When the plugin gets shutdown record the last kmsg event that was stored into the state file (/run/elosd/kmsg.state)
On elosd startup check for /run/elosd/kmsg.state and publish and store every event after the line recorded in there.
Or everything in the kmsg buffer if the state file doesn't exist jet.

*pros*
* it is ensured that every event gets published and stored
* the only added work is at plugin load and plugin stop so no runtime overhead

*cons*
* similarly complex to implement as 3)
* if for some reason elosd or the plugin isn't stopped gracefully the event wouldn't be written to the state file and therefore the contents of kmsg-buffer will be published again on a restart of elsod


## Decision

Alternative 3 is taken.


# Architecture Design Record - Event Retention Policy

## Problem

In short: how to deal with events that become old.

A retention policy must describe :

* when to consider an event as old
* what to do with old events
* when to apply the retention policy
* consists of multiple retention policies itself



## Influencing factors

## Assumptions

## Considered Alternatives

## 1) event Aggregation - combine repeating events

Repeating events to a single new one "event type occurred n-times from to".

*TBD: evaluation, measurements, PoC*

*pros*
* restore storage capacity
* less dependencies – only one storage technology involved

*cons*
* additional writes after deletes
    * But: <if applicable react to the above with a but...>

## 2) event Aggregation - drop fields

After a certain amount of time it is not necessary to know exactly what happened
only that a specific event has occurred. This can be used to save storage
capacity and shrink events by deleting fields like payload or other fields.

*TBD: evaluation, measurements, PoC*

*pros*
* restore storage capacity
* for string
    * But: <if applicable react to the above with a but...>
* less dependencies – only one storage technology involved

*cons*
* loss of event details

## 3) event Aggregation - keep recent only one

Group events by comparing a set of attributes if they can be treated as equal.
If the case drop all but the most recent one. This can be applied to all events
where it is not necessary to count the occurrences and only the event it self
is important. This policy reduce redundant information of multiple events.

i.e.: Device defect event occurs for 5 hours every 10 seconds only the last event is important as it is still defect.

*TBD: evaluation, measurements, PoC*

*pros*
* restore storage capacity
* simple to implement

*cons*
* needs to search the storage for specific events
* either lock the storage or accept to miss events meanwhile the retention runs

## 4) event deletion depending on filter rules

the existing filter rules based on rpn is used to delete the matching events.

e.g.
` delete ".event.time '654321' LT .event.payload 'Hugo' STRCMP .event.messageCode 43 EQ AND AND"`
This deletes all event with payload="Hugo" and messageCode=43 occuring before timecode "654321".

*pros*
* reuse of existing concept

*cons*
* ".event.time" needs to be added

## Decision

We select the following policies:

* ...TBD...

### Rationale


### Open Points


# Architecture Design Record - Event Storage Class

## Problem

As event storage class should be understood categorization by requirements on
the storage availability of en event. The storage availability or storage class
shall answer the following questions:

* Has the event an expiration date?
* Must all information of the event be stored?
* How long must this event at least available?
    * *never:* the event can be stored but no guarantees have to be given
    * *period:* the event has to be available at least until date X
    * *as long as possible:* the event can be deleted in favor of more important events
    * *forever:* the event has to be persisted under any circumstances any thing else are critical errors
* Which retention policy is allowed to be applied?
    * Can be aggregated with others of the same kind
    * Can be deleted on certain condition
    * Can be loose information (i.e. all syslog message with an mapped message code can drop there payload field after a year, as it is not longer necessary what only that it happens)
    * See Design Decision about retention policies
* Is an event strip able to reduce storage size?
    * Which fields can be omitted or dropped? (i.e. is a PID or a custom payload string really helpful in any type of message?)
* Has the event special storage qualifiers?
    * The event has the classification field which can be used to store some additional storage bits
    * Additional storage information are passed somehow aside the storage request




## Influencing factors

## Assumptions

## Considered Alternatives

### 1) Event Storage Class I


The attributes of the Event Storage Class shall be:

* Retention policy [none, <aggregation function>, delete if, squash]
* Expiration [never, long_as_possible, date, immediate]
* Strip able [none, Payload|<...other fields like PID >]

*TBD: evaluation, measurements, PoC*

*pros*
* ...

*cons*
* ...

### 2) Event Storage Class II – extend the classification field of the canonical event format

The definition of the classification field can be extended to define the use 3 bits of the upper 24 bits.

* 000 store as long as possible
* 001 never store, drop the event
* 010 forever – store and keep on all coasts
* 011-111 reserved for further classifications

*TBD: evaluation, measurements, PoC*

*pros*
* ...

*cons*
* ...

### 3) use the infos in the canonical format to classify

Stored events are dropped if they match a pattern formulated in rpn format see:
[## 4) event deletion depending on filter rules](event_retention_policies.md)

## Decision

We choose the first approach

### Rationale

### Open Points


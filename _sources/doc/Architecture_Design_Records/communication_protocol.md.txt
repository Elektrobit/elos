# Design Decision – communication protocol


## Requirements:

- Easy to extend and human readable
- Message producer do not need to know anything about message consumers
- "scale able" and therefore it works for a view as good for many participants,
  which means the number of participants can change constantly during runtime


## Possible approaches :

A) Binary protocol

* More size efficient then text based
* Difficult to use i.e. on command line for shell scripting without dedicated utilities
* Not human readable
* BSON could be a possible solution
  * Extendable
  * More efficient then text based, due to no text parsing overhead


B) text based protocol

* Easy to use i.e. on command line
* Human readable
* Somewhat laborious to use in low level languages like C
* `json`
  * Readable but compact in size
  * Well known / supported
  * Structured and type support
  * Not as cycle efficient as binary protocols due to parsing overhead


## Decision:

Use a JSON based protocol as:
* Is easy to extend , important for future feature requests
* Easy to use on command line, many tools/libraries available and proven in use
* Basic type support: possible to encode integer, floating points strings, objects/maps and arrays

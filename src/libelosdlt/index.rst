libelosdlt
=============

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   private/dlttoelosConverter/index


This library aims to implement the *Diagnostic Log and Trace* (DLT) protocol. The
implementation is tested using the https://github.com/COVESA/dlt-daemon.

Currently it supports :

* (un-)register a application
* (un-)register a context
* sending a log message


The following communication channels are implemented:

* Pipe
* TCP-Socket
* Unix Domain Socket

However logging is currently only possible using a pipe connection. This is the
default if using `libelosDlt.c::elosDltConnect`.

Example
-------

In :file:`src/demos/elosDlt.c` you can find an example program using libelosDlt
to send a DLT log message.

.. literalinclude:: /src/demos/elosDlt.c
   :language: C

API
---

The API description can be found here :file:`/src/libelosdlt/public/elos/libelosdlt/libelosdlt.h`

.. literalinclude:: /src/libelosdlt/public/elos/libelosdlt/libelosdlt.h
   :language: C

The protocol definition and the types can be found here
:file:`/src/libelosdlt/interface/elos/libelosdlt/types.h`:

.. literalinclude:: /src/libelosdlt/interface/elos/libelosdlt/types.h
   :language: C

Known Issues
------------

Currently it doesn't work to publish log message via UNIX and TCP sockets. It
is possible to create an injection message (`elosDltSendControlMessage`)
including the log message, but it fails because it is not possible to create an
application and session context. This only works via pipe connections. May be with
a mixed approach it is feasible.

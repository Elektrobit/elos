libelos-lite
============

This library is a static library with no dependencies and a very limited number of syscalls for resource constrained systems to send events to elosd.

Build
-----

To build libelos-lite the `ELOS_LIBRARY_LITE=on` option needs to be set in cmake.
Additionally the buffer size for publish can be set with `ELOSLITE_SEND_BUFFER_SIZE`.

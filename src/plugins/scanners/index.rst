
# elos

## Scanner

This interface is intended for internal use only.

### Scanner Interface

The scanner interface is based on two parts:

- Functions that need to be implemented by the scanner and are called by the server (init, start, exit)
- Functions passed in form of pointer from the server to the scanner (register event, push event)


#### scanner_initialize

Initialize the components needed for event scanning (e.g. check if temperature
sensor interface exists and is accessible) and register one or more event types
with the server.

Parameters:
1. `struct scanner_callback* callback`   --   contains function pointers for registering/pushing events to the event logging and management system
2. `const void* config`   --   TBD: data type
3. `struct scanner_session** session`   --   Output value

Returns: 0 for success or errorcode


#### scanner_start

Start scanning for events. This function will be executed inside a thread on
the server side, so blocking reads and similiar features can be used.

Parameters:
1. `struct scanner_session* session`

Returns: 0 for success or errorcode

Note: This function won't exit until an error occurs or the function
is exited with the scanner_stop() call.


#### scanner_stop

Stops scanning for events. Will be called from outside the thread the
_start functions runs in.

Parameters:
1. `struct scanner_session* session`

Returns: 0 for success or errorcode


#### scanner_free

Stop scanning for events (if not already stopped),
free resources allocated during scanner_initialize.

Parameters:
1. `struct scanner_session* session`

Returns: 0 for success or errorcode

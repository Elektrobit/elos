
# logger

The component contains code for the internal logger. The internal logger converts a log message along with its properties like message code, severity, classification into log event. This log event is then stored in the elos backend. Internal events when logged in such a way help in analysing and debugging the state of elosd.

The logger needs to be first initialized, then registered to an event dispatcher and then at the end be deleted. The logger stores the log events in a event buffer which is essentially a ring buffer which is initialized with an event count of `128` i.e the logger can store up to 128 events before the oldest ones are removed.

## elosLoggerInitialize
Initializes the logger variable of type `elosLogger_t`. The logger is initialized by creating a new event buffer for the logger and setting the logger flag to initialized. It is required that the logger is initialized only once during the eintire time elos is running.


## elosLoggerDeleteMembers
Deletes the logger variable of type `elosLogger_t` in the sense that the logger's internal event buffer is deleted and the flag are reset to being not initalized.


## elosLoggerGetDefaultLogger
This function initalizes the internal logger. It checks if the the if the flags member of the logger is set, when not set the flags member is set. Once the flags member is set the elosLoggerInitialze function is called to initialize the event buffer of the internal logger.


## elosLog
The elood log function retrievs the internal logger of elos and the creates an log event from the given parameters. It checks if the internal logger is initialized, when initialized it tries to publishe the created event using the elos logger. If publishing of the log event fails the the it calls the fallback function to log the log event. The fallback event is currently the `elosLogSafuFallback` which passes to event to be logged vis `safu log` component.


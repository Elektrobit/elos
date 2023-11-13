eloslog
=======

This component contains common code as a part of the elos log API. The common code converts the given individual
log parameters into an elos event. These log events are then stored in the elos backend and can be retrieved using
event filters for various purposes.

CreateElosEventFromLog
----------------------

The log parameters passed to this function is converted into an elos event. The event parameters are set as follows :

- The event date is set using the clock_gettime function.
- The event pid is set using the getpid glibc function.
- The event source application file name is set by reading the link path
  present in `proc/self/exe` using the readlink function.
- The event source application name is set by extracting the application name from application filename using strrchr function.
- The event severity is set from the passed severity parameter.
- Hardwareid will ideally be set using the `safuGetHardwareId` but for now is set using the gethostname function.
- The event classification is set from the passed classification parameter.
- The event messageCode is set from the passed messageCode parameter.
- The event payload is set from the passed logMessage parameter.

LogSafuFallback
---------------

The LogSafuFallback is the fallback function when event creation for logging fails. When event creation fails this fallback function is called which logs the log message using the safulog API functions. The safulog function logs the message normally on the console, which is then visible as logs based on the loglevel filter set on the elos config. The safulog API functions are mapped according to the `severity` parameter so that the message is logged with th correct log level. The mapping is as follows :

+---------------------+--------------------+
|  Severity           |  Safulog Function  |
+=====================+====================+
| ELOS_SEVERITY_FATAL |   safuLogCrit      |
+---------------------+--------------------+
| ELOS_SEVERITY_ERROR |   safuLogErr       |
+---------------------+--------------------+
| ELOS_SEVERITY_WARN  |   safuLogWarn      |
+---------------------+--------------------+
| ELOS_SEVERITY_INFO  |   safuLogInfo      |
+---------------------+--------------------+
| ELOS_SEVERITY_DEBUG |   safuLogDebug     |
+---------------------+--------------------+

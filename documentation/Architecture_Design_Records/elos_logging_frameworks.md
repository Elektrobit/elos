# Architecture Design Record - Evaluation Of Logging Frameworks

## Problem

Elos at present has a simple logging framework. The logging framework must be more robust and extensive.
This can be achieved using :

a) Taking an existing third party logging framework and extending it by adding a adapter for elos. This 
   adapter helps in achieving the goals given below. The API's from these logging framework is directly
   available for applications for publishing events via the aforementioned adapters.

b) Write an elos log API that acts as an wrapper over the logging API of a third party logging framework.
   This elos log API is then exposed to the end user for logging purposes and fulfills the requirements
   given below.

## Goals

The current framework should be reworked upon or existing framework changed to another framework so 
that the requirements given below are satisfied.

1. Expandable to publish elos events
   * The logging frame shall provide a way to create elos events whenever a log is created, with the
     log message as the payload. The event should be generated when an application uses the elos API 
     as well as by elos within itself. 
2. Support for fallback strategy
   * When an event creation fails due to various reasons, then an fallback strategy is provided. For e.g.
     log message diverted to stderr when event creation fails.
3. Thread safety
   * The logging framework should be thread safe.
4. Supports contextual logging
   * Contextual logging is logging the same log message under different log level based upon context in which
     it is logged. E.g. Failure to open a file to write a log is not as critical as failure to open a file
     to read out elos configuration which is critical. Although both failure logs are same the logging level is
     different. The logging level is then detemined using the the log context level.

In case it is determined that a third party logging framework is needed then the following additional requirements
are needed 

5. Plain C interface
   * Its is written in plain C
6. License 
   * Logging frame work should be open-source with corresponding open-source license like [L]GPL/BSD/MIT
7. Maturity Grade
   * The framework is well maintained and regularly updated with bug fixes.
8. Performance
   * Is not too big or too complex to integrate into the existing code base, and is fast to build and execute.
9. Configuration
   * Easily customizable to suit our requirements with the help of configurations. The configuration can be 
     either be file or be done programtically.


## Solution

The following logging frameworks were analyzed to check if they fulfill the requirements stated above 

* safu log
    * The safu log is a logging library developed as part of elos safe utilities. Elosd application and API's 
      use this library for logging. It is thread safe and written in C. It is configurable within the scope 
      of elos. 

* log4c
    * Log4c is a library of C for flexible logging to files, syslog and other destinations. It is modeled after 
      the Log for Java library (http://jakarta.apache.org/log4j/), staying as close to their API as is reasonable. 

* slog
    * SLog is cross platform and thread safe logging library for C/C++ with possibilities to easily control verbosity 
      levels, tag and colorize output, log to file, on the fly change configuration parameters, optional log callback
      for log collection and many more. More information can be found here (https://github.com/kala13x/slogx)

* zlog
    * zlog is a reliable, high-performance, thread safe, flexible, clear-model, pure C logging library. More information
      can be found in (http://hardysimpson.github.io/zlog)

* zf\_log
    * zf\_log is a core logging library for C, Objective-C and C++. Following the Unix way, this library provides the 
      logging core which can be used directly or extended. In essence, it's a thin wrapper around snprintf() function. By
      implementing less than 20% of functionality found in more sophisticated and feature reach libraries, it covers more
      than 80% of common use cases. Found to be particularly useful in cross-platform applications and on mobile/embedded 
      platforms. Focus is made on simplicity, ease of use and performance (to be more precise - low overhead). More information
      can be found in (https://github.com/wonder-mice/zf\_log)


The results of the analysis are given in the table below:

| Requirements                         | safu log    | log4c       | slog        | zlog                   | zf\_log                                               |
| :----------------------------------- | :---------: | :---------: | :---------: | :--------------------: | :---------------------------------------------------: |
| Expandable to publish elos events    | No          | Yes         | Yes         | Yes                    | Yes                                                   |
| Support for fallback strategy        | No          | Yes         | Yes         | Yes                    | Yes                                                   |  
| Thread safety                        | Yes         | No          | Yes         | Yes                    | Yes                                                   |
| Plain C interface                    | Yes         | Yes         | Yes         | Yes                    | Yes                                                   |
| Supports contextual logging          | No          | \*No        | \*No        | \*No                   | \*No                                                  |
| License Type                         | MIT         | LGPLv2      | MIT         | LGPLv2.1               | MIT                                                   |
| Maturity Grade (Code last updated)   | Regular     | 01.2016     | 05.2023     | 07.2023                | 03.2021                                               |
| Performance                          | NA          | NA          | NA          | NA                     | [Performance](https://github.com/wonder-mice/zf_log#comparison) |
| Configuration                        | NA          | configured  | configured  | configured during init | no config file required, configuration is done ad-hoc |
|                                      |             | during init | dynamically | with config file but   |                                                       |
|                                      |             | with config | no config   | can be re-configured   |                                                       |
|                                      |             | file        | file reqd.  | dynamically            |                                                       |

\* : can be implemented by user


## Conclusion

a) In all the logging frameworks analyzed, it is possible to add an adapter using the provided callback function.

b) All frameworks have similar API functions which can be wrapped in an elos API function.


## Decision 

a) In all analyzed logging frameworks, it is possible to add an adapter for elos. This can be achieved via a call back function
   provided in their respective API's while logging. In this method however new api function from the logging library needs to be 
   incorporated in the code which leads to much refactoring since logging is used extensively in elos source. So this solution 
   will be avoided.

b) We will write a elos logging library. The API's from this library will act as a wrapper function for a selected third party
   logging framework. In this way we ensure that logging functions can remain as close to existing logging function as possible
   in the elos source. Moreover with the wrapper function we can modularize output logging, meaning we can design the elos log API
   in such a way that it can support any logging framework fulfills the above criteria and has the required function that can be 
   mapped to the elos API wrapper. In this method the  requirement event generation is then compartmentalized inside elos log api 
   which makes event publishing more secure.

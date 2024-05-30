# Out of memory process killer

## Problem 

Elos should trigger an event when a OOM (Out of memory) killer is invoked and a
process is terminated as a result.


## Goals
When an oom-killer has terminated a process an event corresponding to this is
published so that it is simpler to analyse why this has happened.


## Assumptions

The system has an OOM-killer configured and when system memory is too low for
system to function effectively, the OOM-killer is triggered to clean up
processes that are using up system memory. This is then logged in the system 
somewhere.


## Solutions

Upon OOM-killer invocation a log is created and it is found to logged as kernel
log message and we already have a `Kmsg Scanner` which publishes kernel log
messages as event, so having made this clear we have the following option to publish a 
separate event for OOM

1) **Extend existing Kmsg Scanner** : The configuration for `Kmsg Scanner` does not 
   contain a 'MappingRules' option, to map an event based on a given `MessageCode` filter.
   The configuration can be extended by adding a `MappingRules` option as given below:


   ```json
        "KmsgScanner": {
            "KmsgFile": "/dev/kmsg"
            "MappingRules": {
                "MessageCodes": {
                    "5020": ".event.payload '.*Out of memory: Killed process*' REGEX"
                }
            }
        }
   ```

  When this is done, the `Kmsg Scanner` shall be expanded similar to `Syslog Scanner` to 
  include an `logline mapper` where the `Kernel log` is parsed to check for `OOM-killer`
  log message and when a match is found an event is published.

  **pros**
  
   - Expansion to filter other messages like OOPs can be easily implemented by adding an
     appropriate filter and parsing the log line for the same.
   - Use of existing code. Implementation will quicker since we can copy log line mapper
     code from `Syslog Scanner`
  
  
  **cons**
  
   - Kernel logs are not in caononical form, needs to be analysed and parsed, and 
     `log line mapper` from  `Syslog Scanner` can not be used as is. Complex to implement.


2) **Implement a new OOM-killer scanner** : 

    a) Implement a new oom-killer scanner from scratch. Since the `Kmsg Scanner` already
       publishes kernel log messages as events, a new scanner is implemented that subscribes 
       to the events published by the `Kmsg Scanner` and when an event payload has an oom-killer
       message, then the oom-killer invoked event is published.

    b) Implement a new scanner from scratch and directly link to kernel using netlink and
       scan incoming messages for oom-killer activity.

  **pros**
  
    a) Subscribing to existing `Kmsg Scanner` event :

         - Event already exists in canonical form, no need parse log lines, only 
           subscription to publishing client and generating new event needs to be done.
           Simple implementation.
         - Expand subscription to other events like OOP's easily.

    b) Connect to kernel using nelink socket :

         - Directly access to kernel logs to check for invocation of oom-killer, which means
           this is quicker. 
  
  **cons**
  
    a) Takes time, since the subscribing client needs to wait for the `Kmsg Scanner` 
       to publish a event before creating a new event.
  
    b) Netlink library needs to be analysed to check if netlink provides necessary 
       protocols for interacting with kernel logs directly. Upon analysing netlink
       protocols to interact with kernel, it is found that netlink provides the 
       following protocols
  
       NETLINK_KOBJECT_UEVENT : This protocol is used for recieving communication from
                                 the kernel related to device and driver management.
  
       NETLINK_CONNECTOR : This protocol is used for generic communication with the kernel
                            used for communicating, process, network and filesystem based events.
  
       Programming a simple monitor to check for oom-killer using the above protocols did not provide
       the expected result. On further reading, a user defined protocol can be defined to implement the
       above test, but this is not tested yet. There is a lot of over head here in implementing a custom
       protocol for this purpose.


3) **Implement a Scanner from another source**
 
   Instead of the kernel log another source where an indication of oom-killer invocation is
   present can be used. The following interfaces can be checked for oom-killer occurence:
   
   * check `/proc/vmstat/` for oom\_kill\_count, if greater than 0 then publish an event
   * read `/sys/kernel/debug/oom\_kill` to get information about oom-killer
   * read `/sys/kernel/debug/oom\_kill\_allocating\_task`  to get information about oom-killer
   
   A scanner can be implemented to scann the above interfaces to check for oom-killer activity.

  **pros**

   - This is similar to the existing `coredump` client and can be implemented relatively easily.

  **cons**

   - Depends heavily on kernel debug configuration and `proc` interfaces, which might always not be
     available. The kernel needs to be  configured to include `sys/kernel/debug` interface. This is 
     not a client requirement and hence not implemented, therefore this method is not feasible.


## Decision

Having taken into account all pros and cons of all the provided solutions given above it is decided that a separate
scanner that subscribes to `Kmsg Scanner` events for oom-killer invocation will be implemented.


## Open Points

When parsing a Kernel log event for oom killer invocation, it is possible to retrieve the process name and its pid,
but it is not possible to retrieve the path. This is because the `proc` interface with the pid is not available after
the process is killed by the oom killer. To predetermine which process will be killed by oom killer is to much of an
overhead. The oom killer process path will therefore be set to empty "". 

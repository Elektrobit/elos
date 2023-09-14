# Architecture Design Record - Blacklist filtering of published events

## Problem
a) Handling error while setting, creating and executing blacklist filters due to


   * Set blacklist filter strings in config
     - blacklist filter attribute is available but not set in elos config.
     - blacklist filter attribute is not available in elos config.
     - elos is started with a empty config


   * Create blacklist filters from filter strings.
     - The given filter string is invalid
     - Filter creation fails due to external error like malloc fails.


   * Executing a created blacklist filter.
     -elosEventFilterExecute returns error status


   * Security event creation fails
     -security event member allocation fails.


b) How to setup complex filter rules which filter on multiple event attributes.

## Goals

* Use blacklist filters to prevent unauthorized clients from publishing critical events. Ensure
  that safe guards are in place to handle events if blacklist filter fails due to reasons given
  above.

## Assumptions

   Blacklist filtering of events are carried out only for events published from unauthorized clients.

## Solution

a) In case of error while setup, creation and execution of blacklist filter due to various reasons given in [Problem](#problem),  a blacklist filter is not created, which means events from unauthorized clients can not be properly classified and filtered. In this case events from unauthorized clients can be classified as :


   1) all events from unauthorized clients are classified as critical and shall not be published a security event is published instead.


      ```json
      {
        "date": [42,0],
        "source": {
          "appName": "elosd",
          "pid": 1234
        },
        "severity": 3,
        "hardwareid": "<hardwareid>",
        "classification": 324,
        "messageCode": 8007
      }

      ```
      The value for `classification` is a concatenation of the flags
      `elos|Security|IPC`.

      If a blacklist filter cannot be created then an error message is published and the client is notified.

      ```json
      {
        "date": [42,0],
        "source": {
          "appName": "elosd",
          "pid": 1234
        },
        "severity": 2,
        "hardwareid": "<hardwareid>",
        "classification": 260,
        "messageCode": 501
      }

      ```
      The value for `classification` is a concatenation of the flags
      `elos|Security`.
      In this state elos will allow only authorized clients to publish events.


   2) all events from unauthorized clients are classified as not critical and shall be published. In this state elos will allow any event from unauthorized clients will be published, which can include possible critical events.

   3) events from unauthorized clients shall pass through a default blacklist filter.


   Since security events are set up manually, there is no possibility of failure in event creation or event member allocation.


b) If a filter is used to match more than one event attribute or has more then one RPN-term then it is deemed as a complex filter.
   Such a complex filter can be implemented in two ways


   - define black list as a list of multiple `OR` linked simple RPN-filters as
     given below


     ```Json
     "EventBlacklist": [
         ".event.messageCode 2000 EQ",
         ".event.messageCode 3000 EQ"
     ]
     ```


  - define black list attribute in config as a simple JSON string and use RPN-filter notations to express complex filter rules
    as given below

    ```Json
    "EventBlacklist": ".event.messageCode 2000 EQ .event.messageCode 3000 EQ OR"
    ```

## Decision:
a) We chose solution 1 because of the following reasons


   1) We shall classify all events from an unauthorized clients as critical so that they are not published, in the case of a failure during setup, creation or execution of blacklist filters. If an event from unauthorized client matches the blacklist filter, then a security event is published instead of the event from the client. If there is a failure in the creation of the blacklist filter then a error event is published. Unauthorized clients receive a response status, informing that they are unauthorized to publish critical events. Client would receive a response status from elos which indicates a failure, if the blacklist check fails due to an elos internal error.

   2) Classifying events from unauthorized clients as not critical is not safe and can be exploited by bad faith actors and therefore will be avoided.

   3) A default blacklist needs to be robust and exhaustive so that all possible critical events that could emerge from an unauthorized client are accounted for. Since this is not feasible at the moment this will be avoided.


b) We use single JSON string along with RPN-Filter notations to implement complex filters, because it is easy to implement and it is not expected to have a negative performance impact than applying multiple filters. Using lists in blacklist filter attribute needs a event filter to be created for every filter string listed in the attribute and each created filter needs to be executed, which in turn impacts performance.

## Open Points:

a) Complex filters are open for performance consideration and need to be benchmarked in the future.

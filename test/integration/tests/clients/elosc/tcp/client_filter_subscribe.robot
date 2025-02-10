*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to check if client subscription to an event
...                  function as expected i.e.
...                 client subscription fails to log event when event filter
...                 is invalid
...                 Invalid Filter string of one client does not affect other
...                 clients with valid filter strings.

Resource            resources/keywords.resource
Library             String
Library             SSHLibrary
Library             libraries/ElosKeywords.py

Suite Setup         Run Keywords    Connect To Target And Log In
...                 AND             Ensure Elosd Is Started
Suite Teardown      Close All Connections


*** Variables ***
@{INVALID_FILTER}           dsp9imjrewwnufxp98jrewuuuuuuwrevgf
...                         .event.Source.appName 'hugo' STRCMP
...                         .event.Source.appName 'hugo' EQ
...                         .event.Source.appName 'hugo' LT
...                         .event.Source.appName hugo STRCMP
...                         .event.definitely.not.existing.field 'hugo' STRCMP
@{MESSAGES}                 {"messageCode": 1004,"payload":"testEventFiltering"}
...                         {"messageCode": 1040,"payload":"testEventFiltering"}
...                         {"messageCode": 1400,"payload":"testEventFiltering"}
${VALID_FILTER}             .event.messageCode 1400 EQ
@{PUBLISH_LOG}              @{EMPTY}


*** Test Cases ***
Event Not Logged By Client With Invalid Filter
    [Documentation]    A published event is not logged by a client
    ...    with invalid filter string

    FOR    ${filter}    IN    @{INVALID_FILTER}
        When A Client Subscribes With Invalid Filter    ${filter}
        Then Subscription Should Fail
    END

Client With Invalid Filter Does Not Affect Other Clients
    [Documentation]    A client with invalid filter does not affect other clients

    Given A Client Subscribes With Invalid Filter    ${INVALID_FILTER}[1]
    AND A Client Subscribes With Valid Filter
    When A Subscribed Event Is Published
    Then Client Receives Event Subscribed To


*** Keywords ***
A Client Subscribes With Invalid Filter
    [Documentation]    Start elos client with an invalid filter string
    [Arguments]    ${invalid_filter}

    Subscribe To '${invalid_filter}'

A Subscribed Event Is Published
    [Documentation]    Publish Created Messages

    FOR    ${message}    IN    @{MESSAGES}
        ${publish_output}=  Publish '${message}'
        Append To List    ${PUBLISH_LOG}    ${publish_output}
    END
    Log List    ${PUBLISH_LOG}
    Sleep    1s

A Client Subscribes With Valid Filter
    [Documentation]    Start elos client with a valid filter string

    Subscribe To '${VALID_FILTER}'
